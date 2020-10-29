"""
MoCAD experiment 5 : Control a vehicle by reinforcement learning
UE4 editor
carla 0.9.9.4
python 3.5
torch version: 1.5.1
pygame 1.9.6
-----------------------------------------------------------
1. spawn a vehicle
2. Model Neural Network
3. RL training
"""

import random
import numpy as np
import torch
import os
from agent_dqn import DQNAgent
from carla_env import CarEnv, get_speed, get_font
from threading import Thread
import time
import pygame
from param import *
from torch.utils.tensorboard import SummaryWriter






if __name__ == '__main__':

    try:

        # FPS = 10   # frames in second
        ep_rewards = list() # record the episode reward
        # reproduce
        SEED = 123
        random.seed(SEED)
        np.random.seed(SEED)
        torch.manual_seed(SEED)
        # -----------------------------#
        # ----    Create agent    ---- #
        # -----------------------------#
        agent = DQNAgent()
        # -----------------------------#
        # ----      Carla Env     ---- #
        # -----------------------------#
        env = CarEnv()
        # ----------------------------------------------#
        # ---- Initialization the training model   ---- #
        # ----------------------------------------------#
        # Start training thread and wait for training to be initialized
        trainer_thread = Thread(target=agent.train_in_loop, daemon=True)
        trainer_thread.start()
        while not agent.training_initialized:
            time.sleep(0.01)
        # -----------------------------#
        # ----    Training        ---- #
        # -----------------------------#
        # Initialize predictions - forest prediction takes longer as of initialization that has to be done
        # It's better to do a first prediction then before we start iterating over episode steps
        q_value = agent.get_qs(np.ones((1, INPUT_HEIGHT, INPUT_WIDTH)))
        epsilon = INIT_EPSILON
        total_step = 0
        best_score = -200
        tb = SummaryWriter()
        # ----      Show          ---- #
        pygame.init()
        # pygame set the mode
        display = pygame.display.set_mode([IMG_WIDTH, IMG_HEIGHT])
        font = get_font()
        # clock show the frame
        clock = pygame.time.Clock()
        # Iterate over episodes
        for episode_i in range(1, EPISODES + 1):
            # Reset environment and get initial state
            current_state = env.reset()
            # Reset flag and start iterating until episode ends
            done = False
            # Play for given number of seconds only
            while True:
                time_step += 1
                # This part stays mostly the same, the change is to query a model for Q values
                if np.random.random() > epsilon:
                    # Get action from Q table
                    action = agent.get_qs(current_state).argmax(dim=-1).detach().to('cpu').numpy()[0]
                else:
                    # Get random action [0, 1, 2] -> steer: left, middle, right
                    action = np.random.randint(0, 3)
                    # This takes no time, so we add a delay matching 60 FPS (prediction above takes longer)
                    # time.sleep(1 / FPS)
                # interact with the carla Env
                new_state, reward, done, _ = env.step(action)
                if time_step > 20:
                    # waiting for spawning the vehicle
                    # Every step we update replay memory
                    agent.update_replay_memory((current_state, action, reward, new_state, done))
                # transform the state
                current_state = new_state
                episode_reward += reward
                if done:
                    break
            # End of episode - destroy agents
            for actor in env.actor_list:
                actor.destroy()
            # show results in the tensorboard
            tb.add_scalar('Episode Reward', episode_reward, episode_i)
            tb.add_scalar('Epsilon', epsilon, episode_i)
            # all the reward in training
            ep_rewards.append(episode_reward)
            if episode_i % 100 == 0:
                print('Training {}/{}, epsilon {}, Reward {}, Time {}'
                      .format(episode_i, EPISODES, epsilon, episode_reward, time.time()-start_time))

            # Save model
            save_dir = 'model'
            if episode_i % 1000 == 0:
                if not os.path.exists(save_dir):
                    os.mkdir(save_dir)
                print('Save the model ...')
                torch.save(agent.q_model, os.path.join(save_dir, 'agent_{}'.format(episode_i)))

            # Decay epsilon
            if epsilon > MIN_EPSILON:
                epsilon *= EPSILON_DECAY
                epsilon = max(MIN_EPSILON, epsilon)
    finally:
        for actor in env.actor_list:
            actor.destroy()

