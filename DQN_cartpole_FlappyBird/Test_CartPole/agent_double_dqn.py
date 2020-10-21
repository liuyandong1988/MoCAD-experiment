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


class SumTree(object):
    """
    This SumTree code is a modified version and the original code is from:
    https://github.com/jaara/AI-blog/blob/master/SumTree.py
    Story data_XX with its priority in the tree.
    """
    data_pointer = 0

    def __init__(self, capacity):
        self.capacity = capacity  # for all priority values
        self.tree = np.zeros(2 * capacity - 1)
        # [--------------Parent nodes-------------][-------leaves to recode priority-------]
        #             size: capacity - 1                       size: capacity
        self.data = np.zeros(capacity, dtype=object)  # for all transitions
        # [--------------data_XX frame-------------]
        #             size: capacity

    def add(self, p, data):
        tree_idx = self.data_pointer + self.capacity - 1
        self.data[self.data_pointer] = data  # update data_frame
        self.update(tree_idx, p)  # update tree_frame

        self.data_pointer += 1
        if self.data_pointer >= self.capacity:  # replace when exceed the capacity
            self.data_pointer = 0

    def update(self, tree_idx, p):
        change = p - self.tree[tree_idx]
        self.tree[tree_idx] = p
        # then propagate the change through tree
        while tree_idx != 0:    # this method is faster than the recursive loop in the reference code
            tree_idx = (tree_idx - 1) // 2
            self.tree[tree_idx] += change

    def get_leaf(self, v):
        """
        Tree structure and array storage:
        Tree index:
             0         -> storing priority sum
            / \
          1     2
         / \   / \
        3   4 5   6    -> storing priority for transitions
        Array type for storing:
        [0,1,2,3,4,5,6]
        """
        parent_idx = 0
        while True:     # the while loop is faster than the method in the reference code
            cl_idx = 2 * parent_idx + 1         # this leaf's left and right kids
            cr_idx = cl_idx + 1
            if cl_idx >= len(self.tree):        # reach bottom, end search
                leaf_idx = parent_idx
                break
            else:       # downward search, always search for a higher priority node
                if v <= self.tree[cl_idx]:
                    parent_idx = cl_idx
                else:
                    v -= self.tree[cl_idx]
                    parent_idx = cr_idx

        data_idx = leaf_idx - self.capacity + 1
        return leaf_idx, self.tree[leaf_idx], self.data[data_idx]

    @property
    def total_p(self):
        return self.tree[0]  # the root


class Memory(object):  # stored as ( s, a, r, s_ ) in SumTree
    """
    This Memory class is modified based on the original code from:
    https://github.com/jaara/AI-blog/blob/master/Seaquest-DDQN-PER.py
    """
    epsilon = 0.01  # small amount to avoid zero priority
    alpha = 0.6  # [0~1] convert the importance of TD error to priority
    beta = 0.4  # importance-sampling, from initial value increasing to 1
    beta_increment_per_sampling = 0.001
    abs_err_upper = 1.  # clipped abs error

    def __init__(self, capacity):
        self.tree = SumTree(capacity)

    def store(self, transition):
        max_p = np.max(self.tree.tree[-self.tree.capacity:])
        if max_p == 0:
            max_p = self.abs_err_upper
        self.tree.add(max_p, transition)   # set the max p for new p

    def sample(self, n):
        b_idx, b_memory, ISWeights = np.empty((n,), dtype=np.int32), np.empty((n, self.tree.data[0].size)), np.empty((n, 1))
        pri_seg = self.tree.total_p / n       # priority segment
        self.beta = np.min([1., self.beta + self.beta_increment_per_sampling])  # max = 1

        min_prob = np.min(self.tree.tree[-self.tree.capacity:]) / self.tree.total_p     # for later calculate ISweight
        for i in range(n):
            a, b = pri_seg * i, pri_seg * (i + 1)
            v = np.random.uniform(a, b)
            idx, p, data = self.tree.get_leaf(v)
            prob = p / self.tree.total_p
            ISWeights[i, 0] = np.power(prob/min_prob, -self.beta)
            b_idx[i], b_memory[i, :] = idx, data
        return b_idx, b_memory, ISWeights

    def batch_update(self, tree_idx, abs_errors):
        abs_errors += self.epsilon  # convert to abs and avoid 0
        clipped_errors = np.minimum(abs_errors, self.abs_err_upper)
        ps = np.power(clipped_errors, self.alpha)
        for ti, p in zip(tree_idx, ps):
            self.tree.update(ti, p)


class Double_DQN():
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