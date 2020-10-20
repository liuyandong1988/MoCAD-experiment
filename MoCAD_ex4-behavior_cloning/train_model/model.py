import torch.nn.functional as F
import torch.nn as nn
from torchvision import transforms as T





class Network(nn.Module):

    def __init__(self):
        super(Network, self).__init__()
        self.conv1 = nn.Conv2d(in_channels=1, out_channels=24, kernel_size=5, stride=(2, 2))
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
        self.fc3 = nn.Linear(in_features=10, out_features=1)

    def forward(self, x):
        # reshape size
        x = F.interpolate(x, size=[60, 120], mode="bilinear", align_corners=False)
        # normal
        x = T.Lambda((lambda x: (x / 255.0)))(x)
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
        # fc 3
        x = self.fc3(x)
        return x