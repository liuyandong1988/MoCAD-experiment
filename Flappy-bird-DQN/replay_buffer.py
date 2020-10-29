import torch
import random
import numpy as np
from params import DEVICE

class ReplayBuffer(object):
    def __init__(self, size):
        """Create Prioritized Replay buffer.

        Parameters
        ----------
        size: int
            Max number of transitions to store in the buffer. When the buffer
            overflows the old memories are dropped.
        """
        self._storage = []
        self._maxsize = int(size)
        self._next_idx = 0

    def __len__(self):
        return len(self._storage)

    def clear(self):
        self._storage = []
        self._next_idx = 0

    def add(self, obs_t, action, reward, obs_tp1, done):
        data = (obs_t, action, reward, obs_tp1, done)

        if self._next_idx >= len(self._storage):
            self._storage.append(data)
        else:
            self._storage[self._next_idx] = data
        self._next_idx = (self._next_idx + 1) % self._maxsize

    def _encode_sample(self, idxes):
        # obses_t, actions, rewards, obses_tp1, dones = [], [], [], [], []
        batch = [self._storage[i] for i in idxes]
        state_batch =  [transition[0] for transition in batch]
        action_batch = [transition[1] for transition in batch]
        reward_batch = [transition[2] for transition in batch]
        next_state_batch = [transition[3] for transition in batch]
        dones = [transition[4] for transition in batch]
        # # for i in idxes:
        # #     data = self._storage[i]
        # #     obs_t, action, reward, obs_tp1, done = data
        # #     obses_t.append(obs_t[:])
        # #     actions.append(action)
        # #     rewards.append(reward)
        # #     obses_tp1.append(obs_tp1[:])
        # #     dones.append(done)
        # # obs_cur = np.concatenate(np.array(obses_t), dim=0)
        #
        # state_batch, action_batch, reward_batch, next_state_batch, terminal_batch = zip(*batch)
        state_batch = torch.cat(tuple(state for state in state_batch)).to(device=DEVICE)
        action_batch = torch.from_numpy(np.array(action_batch, dtype=np.long)[:, None]).to(device=DEVICE)
        reward_batch = torch.from_numpy(np.array(reward_batch, dtype=np.float32)[:, None]).to(device=DEVICE)
        next_state_batch = torch.cat(tuple(state for state in next_state_batch)).to(device=DEVICE)
        dones = torch.from_numpy(np.array([0 if done else 1 for done in dones], dtype=np.float32)[:, None]).to(device=DEVICE)

        return state_batch, action_batch, reward_batch, next_state_batch, dones

    def make_index(self, batch_size):
        return [random.randint(0, len(self._storage) - 1) for _ in range(batch_size)]

    def make_latest_index(self, batch_size):
        idx = [(self._next_idx - 1 - i) % self._maxsize for i in range(batch_size)]
        np.random.shuffle(idx)
        return idx

    def sample_index(self, idxes):
        return self._encode_sample(idxes)

    def sample(self, batch_size):
        """Sample a batch of experiences.

        Parameters
        ----------
        batch_size: int
            How many transitions to sample.

        Returns
        -------
        obs_batch: np.array
            batch of observations
        act_batch: np.array
            batch of actions executed given obs_batch
        rew_batch: np.array
            rewards received as results of executing act_batch
        next_obs_batch: np.array
            next set of observations seen after executing act_batch
        done_mask: np.array
            done_mask[i] = 1 if executing act_batch[i] resulted in
            the end of an episode and 0 otherwise.
        """
        if batch_size > 0:
            idxes = self.make_index(batch_size)
        else:
            idxes = range(0, len(self._storage))
        return self._encode_sample(idxes)

    def collect(self):
        return self.sample(-1)
