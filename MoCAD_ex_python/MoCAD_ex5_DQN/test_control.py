"""
image --> Model --> action
"""
import cv2
import torch
import math
from param import INPUT_WIDTH, INPUT_HEIGHT

class DQNPolicy():

    """
    obtain the camera-rgb image and sends control info
    """

    def __init__(self, vehicle, model):

        self.vehicle = vehicle
        self.dqn_model = model


    def run_step(self, sensor_data):

        return self._compute_action(sensor_data)


    def _compute_action(self, rgb_image):

        """
        camera_rgb --> model --> action
        """
        # resize 1280*720 --> 256*128
        # rgb->gray
        gray_image = cv2.cvtColor(rgb_image, cv2.COLOR_BGR2GRAY)
        gray_image = gray_image.reshape(gray_image.shape[0], gray_image.shape[1], 1)
        gray_image = cv2.resize(gray_image, dsize=(INPUT_WIDTH, INPUT_HEIGHT), interpolation=cv2.INTER_CUBIC)
        # show the gray input image
        # cv2.imshow('image', gray_image)
        # cv2.waitKey(0)
        # see the image and execute the action by policy model   batch*channel*height*width
        input_image = torch.from_numpy(gray_image).\
            to(device=torch.device('cuda'), dtype=torch.float32).reshape(1, 1, INPUT_HEIGHT, INPUT_WIDTH)
        action = self.dqn_model(input_image).argmax(dim=-1).detach().to('cpu').numpy()[0]
        kmh = get_speed(self.vehicle)
        # the fixed velocity
        v_throttle = 0.5
        if action == 0:
            v_steer = 0
        elif action == 1:
            v_steer = -1
        elif action == 2:
            v_steer = 1
        else:
            pass
        # steer, throttle and brake
        return (v_throttle, v_steer, 0)

def get_speed(vehicle):
    """
    Compute speed of a vehicle in Km/h.

        :param vehicle: the vehicle for which speed is calculated
        :return: speed as a float in Km/h
    """
    vel = vehicle.get_velocity()
    return 3.6 * math.sqrt(vel.x ** 2 + vel.y ** 2 + vel.z ** 2)




