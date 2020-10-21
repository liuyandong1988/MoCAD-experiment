"""
image --> Model --> action
"""
import cv2
import torch

class BehaviorCloning():

    """
    obtain the camera-rgb image and sends control info
    """

    def __init__(self, vehicle, model, image_cut):

        self.vehicle = vehicle
        self.image_cut = image_cut
        self.bc_model = model


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
        gray_image = cv2.resize(gray_image, dsize=(256, 128), interpolation=cv2.INTER_CUBIC)
        # cv2.imshow('image', gray_image)
        # cv2.waitKey(0)
        # see the image and execute the action by policy model   batch*channel*height*width
        input_image = torch.from_numpy(gray_image).\
            to(device=torch.device('cuda'), dtype=torch.float32).reshape(1, 1, 128, 256)
        steer = self.bc_model(input_image).item()
        # throttle and brake set by experiment
        brake = 0.0
        acc = 0.4

        return (acc, steer, brake)




