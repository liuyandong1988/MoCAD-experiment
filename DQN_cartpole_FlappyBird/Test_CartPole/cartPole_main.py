"""
The DQN improvement: Prioritized Experience Replay (based on https://arxiv.org/abs/1511.05952)
View more on my tutorial page: https://morvanzhou.github.io/tutorials/
Using:
pytorch
"""

import os
import gym
from agent_double_dqn import Double_DQN
from agent_dqn import DQN
from agent_fix_target_dqn import DQNFixTarget
from agent_dueling_dqn import Dueling_DQN
import numpy as np
import torch
from torch.utils.tensorboard import SummaryWriter

def cart_pole_training(max_episode, method, eps_end = 0.05, eps_decay = 0.9995, epsilon = 1.0, memory_size = 10000):
    # --- 1. environment ---#
    env = gym.make("CartPole-v0")
    # reproduction
    np.random.seed(0)
    torch.manual_seed(0)
    env.seed(0)
    # # test env
    # env.reset()
    # env.render()
    # input('test env')
    # --- 2 agent dqn --- #
    action_dim = env.action_space.n
    obs_dim = env.observation_space.shape[0]
    if method == 'dqn':
        agent_policy = DQN(n_actions=action_dim, n_features=obs_dim, memory_size= memory_size)
    elif method == 'dqn_fix':
        agent_policy = DQNFixTarget(n_actions=action_dim, n_features=obs_dim, memory_size=memory_size)
    elif method == 'double_dqn':
        agent_policy = Double_DQN(n_actions=action_dim, n_features=obs_dim, memory_size= memory_size)
    elif method == 'dueling_dqn':
        agent_policy = Dueling_DQN(n_actions=action_dim, n_features=obs_dim, memory_size=memory_size)
    # --- 3 training --- #
    total_steps = 0
    record_steps = list()
    record_reward = list()
    # tensorboard
    td = SummaryWriter()
    for i_episode in range(1, max_episode+1):
        episode_steps = 0
        episode_reward = 0
        # reset the observation
        state = env.reset()
        # epsilon update
        if total_steps > memory_size:
            epsilon = max(eps_end, eps_decay * epsilon)  # decrease epsilon
        while True:
            # epsilon-greedy
            action = agent_policy.choose_action(state, epsilon)
            # interact with env
            next_state, reward, done, info = env.step(action)
            total_steps += 1
            episode_steps += 1
            episode_reward += reward
            # save the experience
            agent_policy.memory.add(state, action, reward, next_state, done)
            loss = 0
            # update the model
            if total_steps > memory_size:
                loss = agent_policy.learn(total_steps)
            # transform the states
            state = next_state
            # episode done
            if done:
                print('\repisode:{}\t epsilon:{:.3f}\t step:{}\t reward:{}\t loss: {:.3f}'.
                      format(i_episode, epsilon, episode_steps, episode_reward, loss), end='')
                record_reward.append(episode_reward)
                record_steps.append(episode_steps)
                # show in tensorboard
                td.add_scalar('Reward', episode_reward, i_episode)
                if i_episode % 100 == 0:
                    print('\repisode:', i_episode, ' epsilon:', round(epsilon, 4),
                          ' mean_step:', np.mean(np.array(record_steps[i_episode-100:i_episode])),
                          ' mean_reward:', np.mean(np.array(record_reward[i_episode-100:i_episode])),
                          ' loss:', loss)
                # save the model and data_XX
                if i_episode % 1000 == 0 or i_episode == max_episode:
                    model_path = './model'
                    if not os.path.exists(model_path):
                        os.mkdir(model_path)
                    torch.save(agent_policy.q_model, os.path.join(model_path, method+'_{}'.format(i_episode)))
                    # data_XX
                    data_path = './data'
                    if not os.path.exists(data_path):
                        os.mkdir(data_path)
                    np.save(os.path.join(data_path, method+'_steps_{}'.format(i_episode)), np.array(record_steps))
                    np.save(os.path.join(data_path, method+'_reward_{}'.format(i_episode)), np.array(record_reward))
                break




if __name__ == '__main__':
    max_episode = 10000
    method = 'double_dqn'  # dqn, dqn_fix, double_dqn, dueling_dqn
    cart_pole_training(max_episode, method)
