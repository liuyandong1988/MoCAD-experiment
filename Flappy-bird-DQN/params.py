import torch
# Method
METHOD = 'DoubleDQN'  # 'DoubleDQN', 'DuelingDQN'
DEVICE = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')

# model
IMAGE_SIZE = 84
LR = 1e-3

# training
MAX_EPISODE = 100000
MEMORY_SIZE = 10000
BATCH_SIZE = 256
GAMMA = 0.99
TARGET_MODEL_UPDATE_RATE = 1000

