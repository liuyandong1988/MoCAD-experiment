import os
import numpy as np
import matplotlib.pyplot as plt

smooth_num = 300
# plot the result
double_dqn_data = np.load('double_dqn_reward_10000.npy')
dueling_dqn_data = np.load('dueling_dqn_reward_10000.npy')
data_lab = [double_dqn_data, dueling_dqn_data] # dqn, fixed target, double dqn

each_data = list()
all_mean_reward = list()
# x: episodes y: reward
for data in data_lab:
    for i in range(len(data)-smooth_num):
        each_data.append(np.mean(data[i:i+smooth_num]))
    all_mean_reward.append(each_data[:])
    each_data.clear()

double_dqn_mean_reward = all_mean_reward[0]
dueling_dqn_mean_reward = all_mean_reward[1]
x_episode = range(len(double_dqn_mean_reward))
plt.plot(x_episode, double_dqn_mean_reward, color='blue', label='double dqn')
plt.plot(x_episode, dueling_dqn_mean_reward, color='red', label='dueling dqn')
plt.xlabel('Episode')
plt.ylabel('Average Reward')
plt.title('Reward by dqn and dueling (CartPole)')
plt.legend()
plt.show()

