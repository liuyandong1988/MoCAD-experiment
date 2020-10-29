from collections import deque
from param import *
import random
import numpy as np
import time
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torchvision import transforms as T

print('torch version:', torch.__version__)

class My_CNN(nn.Module):

    def __init__(self, image_channel=2, output_dim=3):
        super().__init__()
        self.conv0 = nn.Conv2d(in_channels=image_channel, out_channels=64, kernel_size=5)
        self.conv1 = nn.Conv2d(in_channels=64, out_channels=16, kernel_size=3)
        self.fc0 = nn.Linear(in_features= 16 * 30 * 62, out_features=1024)
        self.fc1 = nn.Linear(in_features=1024, out_features=128)
        self.fc_out = nn.Linear(in_features=127, out_features=32)
        self.Adan = nn.Linear(31, output_dim)
        self.V = nn.Linear(31, 1)

    def forward(self, x):
        # first CNN
        x = F.relu(self.conv0(x))
        x = F.max_pool1d(x, kernel_size=2, stride=2)
        # second CNN
        x = F.relu(self.conv1(x))
        x = F.max_pool1d(x, kernel_size=2, stride=2)
        # hidden linear layer
        x = x.reshape(-2,  16 * 30 * 62)
        x = F.relu(self.fc0(x))
        # seconde hidden linear layer
        x = F.relu(self.fc1(x))
        # fully connect output
        x = F.relu(self.fc_out(x))
        adv = self.Adan(x)
        v = self.V(x)
        adv_average = torch.mean(adv, dim=-2, keepdim=True)
        return v + (adv-adv_average)



class Network(nn.Module):

    def __init__(self, image_channel=1, output_dim=3):
        super(Network, self).__init__()
        self.conv1 = nn.Conv2d(in_channels=image_channel, out_channels=24, kernel_size=5, stride=(2, 2))
        self.conv1_bn = nn.BatchNorm2d(24)
        self.conv2 = nn.Conv2d(in_channels=24, out_channels=36, kernel_size=5, stride=(2, 2))
        self.conv2_bn = nn.BatchNorm2d(36)
        self.conv3 = nn.Conv2d(in_channels=36, out_channels=48, kernel_size=5, stride=(2, 2))
        self.conv3_bn = nn.BatchNorm2d(48)
        self.conv4 = nn.Conv2d(in_channels=48, out_channels=64, kernel_size=3, stride=(1, 1))
        self.conv4_bn = nn.BatchNorm2d(64)
        self.conv5 = nn.Conv2d(in_channels=64, out_channels=64, kernel_size=3, stride=(1, 1))
        self.fc1 = nn.Linear(in_features=1280, out_features=256)
        self.fc2 = nn.Linear(in_features=256, out_features=10)
        self.Adan = nn.Linear(10, output_dim)
        self.V = nn.Linear(10, 1)

    def forward(self, x):
        # reshape size
        x = F.interpolate(x, size=[60, 120], mode="bilinear", align_corners=False)
        # conv 1
        x = self.conv1_bn(F.elu(self.conv1(x)))
        # conv 2
        x = self.conv2_bn(F.elu(self.conv2(x)))
        # conv 3
        x = self.conv3_bn(F.elu(self.conv3(x)))
        # conv 4
        x = self.conv4_bn(F.elu(self.conv4(x)))
        # Flatten batch * dim
        x = x.view(-1, 1280)
        # fc 1
        x = F.dropout(F.relu(self.fc1(x)), 0.2)
        # fc 2
        x = F.dropout(F.relu(self.fc2(x)), 0.5)
        # output
        adv = self.Adan(x)
        v = self.V(x)
        adv_average = torch.mean(adv, dim=-2, keepdim=True)
        return v + (adv-adv_average)



class DQNAgent:

    def __init__(self):
        self.device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')
        # # --- model --- #
        self.q_model = Network()
        self.q_model.to(self.device)
        self.optimizer = optim.Adam(self.q_model.parameters(), lr=0.001)
        # target_model
        self.target_q_model = Network()
        self.target_q_model.to(self.device)
        # copy the q model weights to target model
        self.target_q_model.load_state_dict(self.q_model.state_dict())
        self.target_update_counter = 0
        # memory
        self.replay_memory = deque(maxlen=REPLAY_MEMORY_SIZE)
        self.batch_size = BATCH_SIZE
        self.training_initialized = False
        self.terminate =  False
        self.first_train_mark = True
        self.times = 0

        total_num = sum(p.numel() for p in self.q_model.parameters())
        trainable_num = sum(p.numel() for p in self.q_model.parameters() if p.requires_grad)
        print('Total:', total_num, 'Trainable:', trainable_num)



    def update_replay_memory(self, transition):
        # transition = (current_state, action, reward, new_state, done)
        self.replay_memory.append(transition)

    def train(self):
        start_to_train = 10*self.batch_size
        if len(self.replay_memory) < 10*self.batch_size:
            return

        # --- start to train --- #
        if self.first_train_mark:
            self.first_train_mark = False
            print('Start training ')
        mini_batch = random.sample(self.replay_memory, self.batch_size)
        self.times += 1
        # 1.mini-batch
        # state, action, rew, next_state, done
        state_b = np.array([batch[0] for batch in mini_batch])
        action_b = np.array([batch[1] for batch in mini_batch])
        reward_b = np.array([batch[2] for batch in mini_batch])
        next_state_b = np.array([batch[3] for batch in mini_batch])
        done_b = np.array([0 if batch[4] else 1 for batch in mini_batch])
        # to tensor
        state_b = torch.from_numpy(state_b).to(device=self.device, dtype=torch.float)
        action_b = torch.from_numpy(action_b).to(device=self.device, dtype=torch.long).unsqueeze(dim=-1)
        reward_b = torch.from_numpy(reward_b).to(device=self.device, dtype=torch.float).unsqueeze(dim=-1)
        next_state_b = torch.from_numpy(next_state_b).to(device=self.device, dtype=torch.float)
        done_b = torch.from_numpy(done_b).to(device=self.device, dtype=torch.float).unsqueeze(dim=-1)
        # --- double dqn --- #
        # 1. current q value
        q_value = self.q_model(state_b)
        max_q_value = q_value.gather(1, action_b)
        # --- dqn target --- #
        next_q_action = self.q_model(next_state_b).argmax(dim=-1).unsqueeze(dim=-1)
        target_q = self.target_q_model(next_state_b).gather(1, next_q_action)
        target_q_value = reward_b + DISCOUNT * target_q * done_b
        # loss - gradient - update
        loss = F.smooth_l1_loss(max_q_value, target_q_value)
        # grad zero
        self.optimizer.zero_grad()
        # cal gradient
        loss.backward()
        # # clip
        # nn.utils.clip_grad_norm_(self.q_model.parameters(), max_norm=0.5)
        # update
        self.optimizer.step()
        self.target_update_counter += 1
        if self.times % 3000 == 0:
            print('Have training ', self.times)
        if self.target_update_counter > UPDATE_TARGET_EVERY:
            print('Update the Target model params ... ')
            self.target_q_model.load_state_dict(self.q_model.state_dict())
            self.target_update_counter = 0

        return loss.item()


    def train_in_loop(self):
        """
        training the model
        :return:
        """

        # random the data for initialization
        if not self.training_initialized:
            # batch*channel*height*width
            X = torch.tensor(np.random.uniform(size=(1, 1, INPUT_HEIGHT, INPUT_HEIGHT)), dtype=torch.float, device=self.device)
            y_label = torch.tensor(np.random.uniform(size=(1, 3)), dtype=torch.float, device=self.device)
            y_pred = self.q_model(X)
            self.training_initialized = True

        # update the model
        while True:
            if self.terminate:
                return
            self.train()
            # time.sleep(0.01)

    def get_qs(self, state):
        """
        image--> model --> predict q_value
        :return:
        """
        # Scale
        state = np.array(state)
        # batch*channel*height*width
        state = state.reshape(-1, *state.shape) / 255.0
        state = torch.from_numpy(state).to(device=self.device, dtype=torch.float)
        pred_q_value = self.q_model(state)
        return pred_q_value


net = Network()
print(net)

