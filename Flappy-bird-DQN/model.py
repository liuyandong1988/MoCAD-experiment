import torch.nn as nn
import torch.nn.functional as F


class CNNNet(nn.Module):

    def __init__(self):

        super(CNNNet, self).__init__()
        # 3 CNN
        self.conv1 = nn.Conv2d(in_channels=4, out_channels=32, kernel_size=5, stride=1)
        self.conv2 = nn.Conv2d(in_channels=32, out_channels=64, kernel_size=5, stride=1)
        self.conv3 = nn.Conv2d(in_channels=64, out_channels=64, kernel_size=3, stride=1)
        self.fc1 = nn.Linear(in_features=1*64*8*8, out_features=512)
        self.fc2 = nn.Linear(in_features=512, out_features=2)
        # initialization weights
        self._create_weights()

    def _create_weights(self):
        for m in self.modules():
            if isinstance(m, nn.Conv2d) or isinstance(m, nn.Linear):
                nn.init.uniform_(m.weight, -0.01, 0.01)
                nn.init.constant_(m.bias, 0)

    def forward(self, input):
        # first CNN
        x = F.relu(self.conv1(input))
        x = F.max_pool2d(x, kernel_size=2, stride=2)
        # second CNN
        x = F.relu(self.conv2(x))
        x = F.max_pool2d(x, kernel_size=2, stride=2)
        # third CNN
        x = F.relu(self.conv3(x))
        x = F.max_pool2d(x, kernel_size=2, stride=2)
        # hidden linear layer
        x = x.reshape(-1, 1*64*8*8)
        x = F.relu(self.fc1(x))
        # seconde hidden linear layer
        output = self.fc2(x)
        return output

