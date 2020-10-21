import gym
import time
import torch
import random

def demo_show(episodes, method):

    # --- 1. env --- #
    env = gym.make('CartPole-v0')
    action_dim = env.action_space.n
    # reproduction
    env.seed(1)
    random.seed(1)
    # test
    # env.reset()
    # env.render()
    # input('123')
    # --- 2. policy --- #
    agent_policy = torch.load('./model/'+method+'_1000')
    # --- 3. training --- #
    for episode_i in range(1, episodes+1):
        episode_reward = 0
        state = env.reset()
        while True:
            # state-policy-action 10% randomly
            if random.random() > 0.05:
                # by model policy
                action = agent_policy(torch.tensor(state, dtype=torch.float, device='cuda')).detach().argmax(dim=-1).item()
            else:
                action = random.randint(0, action_dim-1)
            # interact with env
            next_state, reward, done, _ = env.step(action)
            env.render()
            time.sleep(0.05)
            episode_reward += reward
            # transform the state
            state = next_state
            if done:
                print('Episode: {} Reward: {}'.format(episode_i, episode_reward))
                break
    input('Game over')





if __name__ == '__main__':
    method = 'double_dqn'
    demo_show(10, method)