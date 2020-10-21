"""
The DQN improvement: Prioritized Experience Replay (based on https://arxiv.org/abs/1511.05952)
View more on my tutorial page: https://morvanzhou.github.io/tutorials/
Using:
Tensorflow: 1.0
gym: 0.8.0
"""
import random
from collections import namedtuple, deque
import numpy as np
import torch
import torch.optim as optim
import torch.nn as nn
import torch.nn.functional as F

np.random.seed(1)
torch.manual_seed(1)


class Net(nn.Module):

    def __init__(self, state_dim, action_dim):

        super(Net, self).__init__()
        self.fc1 = nn.Linear(in_features=state_dim, out_features=128)
        self.fc2 = nn.Linear(in_features=128, out_features=64)
        self.fc3 = nn.Linear(in_features=64, out_features=action_dim)

    def forward(self, x):
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        y_pred = self.fc3(x)
        return y_pred


class DQNFixTarget():
    def __init__(
            self,
            n_actions,
            n_features,
            memory_size,
            learning_rate=0.01,
            gamma=0.99,
            batch_size=128,
            update_target_rate = 5000,

    ):
        self.n_actions = n_actions
        self.n_features = n_features
        self.lr = learning_rate
        self.gamma = gamma
        self.memory_size = memory_size
        self.batch_size = batch_size
        self.update_target_rate = update_target_rate

        # --- model --- #
        self.device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')
        self.q_model = Net(self.n_features, self.n_actions).to(self.device)
        self.optimizer = optim.Adam(self.q_model.parameters(), lr=self.lr)
        # target model
        self.target_q_model = Net(self.n_features, self.n_actions).to(self.device)
        # copy the model parameters
        self.target_q_model.load_state_dict(self.q_model.state_dict())
        # replay buffer
        self.memory = Replay_Buffer(self.memory_size, self.batch_size)

    def choose_action(self, observation, epsilon):
        if np.random.uniform() > epsilon:
            # model prediction
            state = torch.from_numpy(np.array(observation)).to(device=self.device, dtype=torch.float).unsqueeze(dim=0)
            action = self.q_model(state).detach().argmax(dim=-1).item()
        else:
            action = np.random.randint(0, self.n_actions)
        return action

    def learn(self, steps=None):
        # mini-batch sample
        states, actions, rewards, next_states, dones = self.memory.sample()
        # --- dqn --- #
        # 1. current q value
        q_value = self.q_model(states).gather(1, actions)
        # 2. target model value
        next_max_q_as = self.target_q_model(next_states).detach().max(dim=-1, keepdim=True)[0]
        target_q_value = rewards + self.gamma * next_max_q_as * (1-dones)
        # loss - gradient - update
        loss = F.mse_loss(q_value, target_q_value)
        # grad zero
        self.optimizer.zero_grad()
        # cal gradient
        loss.backward()
        # update the weights
        self.optimizer.step()
        # update the target model parameters
        if steps % self.update_target_rate == 0:
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