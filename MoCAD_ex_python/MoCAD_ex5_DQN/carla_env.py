import glob
import os
import sys
import random
import time
import math
import numpy as np
import cv2
import pygame

# -----------------------------#
# ----    Carla 0.9.9 API ---- #
# -----------------------------#

try:
    sys.path.append(glob.glob('/home/liu/Car/carla/PythonAPI/carla/dist/carla-*%d.%d-%s.egg' % (
        sys.version_info.major,
        sys.version_info.minor,
        'win-amd64' if os.name == 'nt' else 'linux-x86_64'))[0])
except IndexError:
    pass

import carla
from param import *
from collections import deque



class CarEnv:

    SHOW_CAM = SHOW_PREVIEW
    STEER_AMT = 0.3
    im_width = IMG_WIDTH
    im_height = IMG_HEIGHT
    actor_list = []
    front_camera = None
    collision_hist = []

    def __init__(self):
        self.client = carla.Client('localhost', 2000)
        self.client.set_timeout(3.0)
        self.world = self.client.get_world()
        self.server_clock = pygame.time.Clock()
        # the start way_point
        self.start_point = self.set_start_waypoint()
        # states
        self.states = None
        self.surface = None

    def set_start_waypoint(self):
        """
        Find a start point
        :return:
        """
        waypoints = self.world.get_map().generate_waypoints(distance=1.0)
        # draw waypoint to define the point position
        w = waypoints[480].transform
        # confirm a start point on the straight line
        self.world.debug.draw_string(w.location, 'O', draw_shadow=False,
                                      color=carla.Color(r=255, g=0, b=0), life_time=10.0,
                                      persistent_lines=True)
        w.location.z = 1.0
        return w

    def reset(self):
        """
        Reset the vehicle start position and Camera-Collision sensor
        :return:
        """
        self.collision_hist = []
        self.actor_list = []
        # The world contains the list blueprints that we can use for adding new
        # actors into the simulation.
        blueprint_library = self.world.get_blueprint_library()
        vehicle_bp = blueprint_library.filter('model3')[0]
        self.vehicle = self.world.spawn_actor(vehicle_bp, self.start_point)
        self.actor_list.append(self.vehicle)
        time.sleep(1.0) # sleep to get things started and to not detect a collision when the car spawns/falls from sky.
        # add Depth sensor
        self.depth_bp = blueprint_library.find('sensor.camera.rgb')
        # change the dimensions of the image
        self.depth_bp.set_attribute('image_size_x', '%f'%self.im_width)
        self.depth_bp.set_attribute('image_size_y', '%f'%self.im_height)
        self.depth_bp.set_attribute('fov', '110')
        # change the camera position and rotation
        transform = carla.Transform(carla.Location(x=2.5, y=0.0, z=2.0), carla.Rotation(pitch=-20.0, yaw=0.0, roll=0.0))
        self.sensor = self.world.spawn_actor(self.depth_bp, transform, attach_to=self.vehicle)
        self.actor_list.append(self.sensor)
        self.sensor.listen(lambda data: self.process_img(data))
        self.vehicle.apply_control(carla.VehicleControl(throttle=0.0, brake=0.0))
        # add collision sensor
        collision_sensor = self.world.get_blueprint_library().find('sensor.other.collision')
        self.collision_sensor = self.world.spawn_actor(collision_sensor, transform, attach_to=self.vehicle)
        self.actor_list.append(self.collision_sensor)
        self.collision_sensor.listen(lambda event: self.collision_data(event))
        while self.front_camera is None:
            time.sleep(0.01)
        self.episode_start = time.time()
        self.vehicle.apply_control(carla.VehicleControl(brake=0.0, throttle=0.0))
        return self.states

    def collision_data(self, event):
        self.collision_hist.append(event)

    def process_img(self, image):
        """
        gray-image is new state
        :param image:
        :return:
        """
        # change the image
        array = np.frombuffer(image.raw_data, dtype=np.dtype("uint8"))
        array = np.reshape(array, (image.height, image.width, 4))
        array = array[:, :, :3]
        array = array[:, :, ::-1]
        # pygame: show the image
        self.surface = pygame.surfarray.make_surface(array.swapaxes(0, 1))
        # rgb->gray
        gray_image = array
        gray_image = cv2.cvtColor(gray_image, cv2.COLOR_BGR2GRAY)
        # resize 1280*720 --> 256*128
        gray_image = gray_image.reshape(gray_image.shape[0], gray_image.shape[1], 1)
        self.front_camera = cv2.resize(gray_image, dsize=(INPUT_WIDTH, INPUT_HEIGHT), interpolation=cv2.INTER_CUBIC)
        # channel * height * width : 1* 128 * 256
        self.states = self.front_camera.reshape((1, INPUT_HEIGHT, INPUT_WIDTH))
        # save the image
        # cv2.imwrite('IMG/%d.png' % round(image.frame, 8), self.front_camera)
        # record the server frame
        self.server_clock.tick()

    def step(self, action):
        '''
        For now let's just pass steer left, center, right?
        0, 1, 2
        '''
        # fixed the velocity
        v_throttle = 0.5
        if action == 0:
            self.vehicle.apply_control(carla.VehicleControl(throttle=v_throttle, steer=0))
        if action == 1:
            self.vehicle.apply_control(carla.VehicleControl(throttle=v_throttle, steer=-1*self.STEER_AMT))
        if action == 2:
            self.vehicle.apply_control(carla.VehicleControl(throttle=v_throttle, steer=1*self.STEER_AMT))

        # reward design and done
        kmh = get_speed(self.vehicle)
        if len(self.collision_hist) != 0:
            done = True
            reward = -10
        # elif kmh < 10:
        #     done = False
        #     reward = -1
        else:
            done = False
            reward = 1
        # The vehicle running SECONDS_PER_EPISODE seconds and stop
        if self.episode_start + SECONDS_PER_EPISODE < time.time():
            done = True

        return self.states, reward, done, None

    def running_demo(self):

        pygame.init()
        # pygame set the mode
        display = pygame.display.set_mode([IMG_WIDTH, IMG_HEIGHT])
        font = get_font()
        # clock show the frame
        clock = pygame.time.Clock()

        while True:
            clock.tick_busy_loop(15)  # control the frame
            self.vehicle.apply_control(carla.VehicleControl(brake=0.0, throttle=1.0))
            # show the fps
            display.blit(font.render('% 5d FPS (client)' % clock.get_fps(), True, (0, 0, 0)), (8, 10))
            display.blit(font.render('% 5d FPS (server)' % self.server_clock.get_fps(), True, (0, 0, 0)), (8, 28))
            display.blit(font.render('% 5d mk/h (velocity)' % get_speed(env.vehicle), True, (0, 0, 0)), (8, 46))
            pygame.display.flip()  # update
            display.blit(self.surface, (0, 0))

    def destroy_actors(self):
        for actor in self.actor_list:
            actor.destroy()

def get_speed(vehicle):
    """
    Compute speed of a vehicle in Km/h.

        :param vehicle: the vehicle for which speed is calculated
        :return: speed as a float in Km/h
    """
    vel = vehicle.get_velocity()
    return 3.6 * math.sqrt(vel.x ** 2 + vel.y ** 2 + vel.z ** 2)


def get_font():
    """
    show font
    :return:
    """
    fonts = [x for x in pygame.font.get_fonts()]
    default_font = 'ubuntumono'
    font = default_font if default_font in fonts else fonts[0]
    font = pygame.font.match_font(font)
    return pygame.font.Font(font, 24)


if __name__ == '__main__':
    env = CarEnv()
    env.reset()
    # demo running
    env.running_demo()

