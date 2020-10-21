"""
The dueling DQN
"""
import random
from collections import namedtuple, deque
import numpy as np
import torch
import torch.optim as optim
import torch.nn as nn
import torch.nn.functional as F




class Net(nn.Module):

    def __init__(self, state_dim, action_dim):

        super(Net, self).__init__()
        self.fc1 = nn.Linear(in_features=state_dim, out_features=128)
        self.fc2 = nn.Linear(in_features=128, out_features=64)
        self.fc3 = nn.Linear(in_features=64, out_features=16)
        self.Adan = nn.Linear(in_features=16, out_features=action_dim)
        self.V = nn.Linear(in_features=16, out_features=1)

    def forward(self, x):
        # dueling DQN
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = F.relu(self.fc3(x))
        adv = self.Adan(x)
        v = self.V(x)
        adv_average = torch.mean(adv, dim=-1, keepdim=True)
        return v + (adv-adv_average)


class Dueling_DQN():
    def __init__(
            self,
            n_actions,
            n_features,
            memory_size=10000,
            learning_rate=0.01,
            gamma=0.99,
            replace_target_iter=5000,
            batch_size=128
    ):
        self.n_actions = n_actions
        self.n_features = n_features
        self.lr = learning_rate
        self.gamma = gamma
        self.replace_target_iter = replace_target_iter
        self.memory_size = memory_size
        self.batch_size = batch_size

        # --- model --- #
        self.device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')
        self.q_model = Net(self.n_features, self.n_actions).to(self.device)
        self.optimizer = optim.Adam(self.q_model.parameters(), lr=self.lr)
        # target_model
        self.target_q_model = Net(self.n_features, self.n_actions).to(self.device)
        # copy the q model weights to target model
        self.target_q_model.load_state_dict(self.q_model.state_dict())
        self.memory = Replay_Buffer(self.memory_size, self.batch_size)

    def choose_action(self, observation, epsilon):
        if np.random.uniform() > epsilon:
            # model prediction
            state = torch.from_numpy(np.array(observation)).to(device=self.device, dtype=torch.float).unsqueeze(dim=0)
            action = self.q_model(state).detach().argmax(dim=-1).item()
        else:
            action = np.random.randint(0, self.n_actions)
        return action

    def learn(self, steps):
        # mini-batch sample
        states, actions, rewards, next_states, dones = self.memory.sample()
        # --- double dqn --- #
        # 1. current q value
        q_value = self.q_model(states).gather(1, actions)
        # --- dqn target --- #
        # 2. double dqn target q value
        next_q_action = self.q_model(next_states).detach().argmax(dim=-1).unsqueeze(dim=-1)
        target_q = self.target_q_model(next_states).gather(1, next_q_action)
        target_q_value = rewards + self.gamma * target_q * (1-dones)
        # loss - gradient - update
        loss = F.mse_loss(q_value, target_q_value)
        # grad zero
        self.optimizer.zero_grad()
        # cal gradient
        loss.backward()
        # update the weights
        self.optimizer.step()
        # update the target q model
        if steps % self.replace_target_iter == 0:
            self.target_q_model.load_state_dict(self.q_model.state_dict())
        return loss.item()


class Replay_Buffer():
    def __init__(self,buffer_size,batch_size):

        self.memory=deque(maxlen=buffer_size)
        self.batch_size=batch_size
        self.experience = namedtuple("Experience", field_names=["state", "action", "reward", "next_state", "done"])

    def add(self,state,action,reward,next_state,done):

        exp=self.experience(state,action,reward,next_state,done)
        self.memory.append(exp)

    def sample(self):

        device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')
        experiences = random.sample(self.memory, k=self.batch_size)
        states=torch.from_numpy(np.vstack([e.state for e in experiences if e is not None])).float().to(device)
        actions=torch.from_numpy(np.vstack([e.action for e in experiences if e is not None])).long().to(device)
        rewards=torch.from_numpy(np.vstack([e.reward for e in experiences if e is not None])).float().to(device)
        next_states=torch.from_numpy(np.vstack([e.next_state for e in experiences if e is not None])).float().to(device)
        dones=torch.from_numpy(np.vstack([e.done for e in experiences if e is not None]).astype(np.uint8)).float().to(device)

        return (states,actions,rewards,next_states,dones)

    def __len__(self):
        """Return the current size of internal memory."""
        return len(self.memory)