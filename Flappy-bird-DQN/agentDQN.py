import torch
import random
import torch.nn as nn
from params import *
from model import CNNNet
import torch.optim as optim
import torch.nn.functional as F
from replay_buffer import ReplayBuffer

class AgentDQN():

    def __init__(self):

        if METHOD == 'DoubleDQN':
            # q model
            self.q_model = CNNNet()
            self.q_model.to(DEVICE)
            self.optimizer = optim.Adam(self.q_model.parameters(), lr=LR)
            # target q model
            self.target_q_model = CNNNet()
            self.target_q_model.to(DEVICE)
        elif METHOD == 'DuelingDQN':
            pass
        elif METHOD == 'DRQN':
            pass
        # copy the q model weights to target model
        self.target_q_model.load_state_dict(self.q_model.state_dict())
        self.target_update_counter = 0
        # replay buffer
        self.replay_buffer = ReplayBuffer(MEMORY_SIZE)


    def choose_action(self, state, epsilon):
        prediction = self.q_model(state)[0]
        # Exploration or exploitation
        if random.random()<epsilon:
            action = random.randint(0, 1)
        else:
            action = torch.argmax(prediction).item()
        return action

    def update_model(self, steps):
        if steps % 10 != 0:
            return

        # --- mini batch update --- #
        # state, action, rew, next_state, done
        state_b, action_b, reward_b, next_state_b, done_b = self.replay_buffer.sample(BATCH_SIZE)
        # --- double dqn --- #
        # 1. current q value
        q_value = self.q_model(state_b)
        max_q_value = q_value.gather(1, action_b)
        # --- dqn target --- #
        next_q_action = self.q_model(next_state_b).argmax(dim=-1).unsqueeze(dim=-1)
        target_q = self.target_q_model(next_state_b).gather(1, next_q_action)
        target_q_value = reward_b + GAMMA * target_q * done_b
        # loss - gradient - update
        loss = F.smooth_l1_loss(max_q_value, target_q_value)
        # grad zero
        self.optimizer.zero_grad()
        # cal gradient
        loss.backward()
        # clip
        nn.utils.clip_grad_norm_(self.q_model.parameters(), max_norm=0.5)
        # update
        self.optimizer.step()
        self.target_update_counter += 1
        if self.target_update_counter > TARGET_MODEL_UPDATE_RATE:
            self.target_q_model.load_state_dict(self.q_model.state_dict())
            self.target_update_counter = 0
        return loss.item()

