"""
imitation learning Behavior clone conda(pytorch=1.5.0)
"""

from utils import load_csv, load_images, show_steering
from model import Network
import torch.nn.functional as F
import torch.optim as optim
from torch.utils.tensorboard import SummaryWriter
import time
import random
import torch
import numpy as np
import os

# --- reproduce --- #
SEED = 123
np.random.seed(SEED)
random.seed(SEED)
torch.manual_seed(SEED)

# --- load image names in the csv file --- #
file = '../collect_data_asynchronous/asyc_man_driving_log.csv'
image_path = '../collect_data_asynchronous/asyc_man_image/'
train_data = load_csv(file)

# --- load image and switch rgb-image to gray --- #
X_train_s, y_train_s = load_images(train_data, image_path)
# show data steer angle
show_steering(y_train_s, y_train_s)


####################
# ---  Model  ---  #
####################
# tensorboard
tb = SummaryWriter()
# gpu or cpu
device_cuda = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')
print('Device:', device_cuda)
model = Network()
print('# --- Network --- # ')
print(model)
model.to(device=device_cuda)

# hyper-parameter
max_iteration = 50000
batch_size = 128
times_list = list()
data_num = len(X_train_s)

# optimizer
optim = optim.Adam(model.parameters(), lr=0.001)
for i in range(1, max_iteration+1):
    start_time = time.time()
    x_inputs, y_label = list(), list()
    # mini-batch: sample randomly
    for _ in range(batch_size):
        sample_index = random.randint(0, data_num-1)
        x_inputs.append(X_train_s[sample_index])
        y_label.append(y_train_s[sample_index]*100.0) # the scale steer
    # batch * channel * height * width
    x_inputs = torch.from_numpy(np.array(x_inputs)).to(dtype=torch.float32, device=device_cuda).reshape(batch_size, 1, 128, 256)
    y_label = torch.from_numpy(np.array(y_label)).to(dtype=torch.float32, device=device_cuda)
    # model outputs
    predictions = model(x_inputs).squeeze(dim=-1)
    # mse loss function
    loss = F.mse_loss(predictions, y_label)
    # zero grad
    optim.zero_grad()
    # loss cal grad
    loss.backward()
    # update the params
    optim.step()
    tb.add_scalar('loss', loss.item(), i)
    times_list.append(time.time()-start_time)
    # print the training information
    if i % 1000 == 0:
        print('Iteration: {} Loss: {} Time: {}'.
              format(i, round(loss.item(),4), round(sum(times_list), 4)))
        times_list.clear()
    # save the model
    if i % 10000 == 0:
        file_path = './model'
        if not os.path.exists(file_path):
            os.makedirs(file_path)
        torch.save(model, os.path.join(file_path, 'model_{}'.format(i)))

# save the model finish the iteration
torch.save(model, os.path.join(file_path, 'model_{}'.format(i)))



