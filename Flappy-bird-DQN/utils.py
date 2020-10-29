import cv2
import torch
import random
import numpy as np


def pre_processing(image, width, height):
    image = cv2.cvtColor(cv2.resize(image, (width, height)), cv2.COLOR_BGR2GRAY)
    _, image = cv2.threshold(image, 1, 255, cv2.THRESH_BINARY)
    return image[None, :, :].astype(np.float32)


def update_epsilon(episode, max_episode, eps_begin=1.0, episode_thread=0.1, eps_end=0.05):
    """
    update the epsilon
    """
    epsilon = eps_begin - 1.3 * episode * (eps_begin - eps_end) / max_episode
    if epsilon < episode_thread:
        epsilon = eps_end
    return epsilon


