import torch
import random
import time
from utils import *
from params import *
import torch.nn.functional as F
from agentDQN import AgentDQN
from flappy_bird import FlappyBird
from torch.utils.tensorboard import SummaryWriter
import numpy as np




def training_main():

    # reproduced
    torch.cuda.manual_seed(123)
    random.seed(0)
    np.random.seed(0)
    # --- 1. Environment --- #
    game_state = FlappyBird()
    # --- 2. training --- #
    agent_dqn = AgentDQN()
    # record the data
    td = SummaryWriter()
    iter = 0
    total_step = 0
    while iter < MAX_EPISODE:
        iter += 1
        # choose action by epsilon-greedy, epsilon update
        image, reward, terminal = game_state.next_frame(0)
        image = pre_processing(image[:game_state.screen_width, :int(game_state.base_y)], IMAGE_SIZE, IMAGE_SIZE)
        # 288*512*3 --> 1*84*84
        image = torch.from_numpy(image).to(device=DEVICE)
        # 1*4*84*84   batch*frame*width*height
        state = torch.cat(tuple(image for _ in range(4)))[None, :, :, :]
        epsilon = update_epsilon(iter, MAX_EPISODE)
        episode_reward = 0
        episode_step = 0
        terminal = False
        start_time = time.time()
        while not terminal:
            action = agent_dqn.choose_action(state, epsilon)
            # interact with the environment
            next_image, reward, terminal = game_state.next_frame(action)
            episode_reward += reward
            episode_step += 1
            total_step += 1
            next_image = pre_processing(next_image[:game_state.screen_width, :int(game_state.base_y)], IMAGE_SIZE, IMAGE_SIZE)
            next_image = torch.from_numpy(next_image).to(device=DEVICE)
            # instead of one frame
            next_state = torch.cat((state[0, 1:, :, :], next_image))[None, :, :, :]
            agent_dqn.replay_buffer.add(state, action, reward, next_state, terminal)
            if len(agent_dqn.replay_buffer) < 2*BATCH_SIZE:
                # need enough experience
                iter = 1
                episode_reward = 0
                episode_step = 0
                total_step = 0
                continue
            # --- update the model weight --- #
            loss = agent_dqn.update_model(total_step)
            # update the states
            state = next_state
            if terminal:
                print("Iteration: {}/{}, Epsilon {}, Reward: {}, Step: {}, time: {}"
                      .format(iter, MAX_EPISODE, round(epsilon,3), round(episode_reward,2), episode_step, round(time.time()-start_time, 2)))
                td.add_scalar('Train/Reward', episode_reward, iter)
                td.add_scalar('Train/Step', episode_step, iter)
                if iter % 5000 == 0:
                    torch.save(agent_dqn.q_model, "{}/flappy_bird_{}".format('./model', iter))
    torch.save(agent_dqn.q_model, "{}/flappy_bird".format('./model'))


if __name__ == '__main__':
    training_main()