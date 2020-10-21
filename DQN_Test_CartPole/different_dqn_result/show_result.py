import os
import numpy as np
import matplotlib.pyplot as plt

smooth_num = 300
# plot the result
dqn_data_random0 = np.load('dqn_reward_r0.npy')
dqn_data_random1 = np.load('dqn_reward_r1.npy')
fix_dqn = np.load('dqn_fix_reward_10000.npy')
double_dqn_data = np.load('double_dqn_reward_10000.npy')

data_lab = [dqn_data_random0, dqn_data_random1, fix_dqn, double_dqn_data] # dqn, fixed target, double dqn

each_data = list()
all_mean_reward = list()
# x: episodes y: reward
for data in data_lab:
    for i in range(len(data)-smooth_num):
        each_data.append(np.mean(data[i:i+smooth_num]))
    all_mean_reward.append(each_data[:])
    each_data.clear()

dqn_mean_r0 = all_mean_reward[0]
dqn_mean_r1 = all_mean_reward[1]
fix_mean_reward = all_mean_reward[2]
double_dqn_mean_reward = all_mean_reward[3]
x_episode = range(len(dqn_mean_r0))
plt.plot(x_episode, dqn_mean_r0, color='red', label='dqn_random_0')
plt.plot(x_episode, dqn_mean_r1, color='black', label='dqn_random_1')
plt.plot(x_episode, fix_mean_reward, color='green', label='fixed_target_dqn')
plt.plot(x_episode, double_dqn_mean_reward, color='blue', label='double dqn')
plt.xlabel('Episode')
plt.ylabel('Average Reward')
plt.title('Reward by different dqn (CartPole)')
plt.legend()
plt.show()

