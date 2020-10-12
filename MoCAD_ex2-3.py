"""
MoCAD experiment 2-2 :  control vehicle by keyboard carla 0.9.9.4 version
-----------------------------------------------------------
1. spawn a vehicle
2. synchronous mode
3. control vehicle by keyboard
"""

import glob
import os
import sys


try:
    sys.path.append(glob.glob('/home/liu/Car/carla/PythonAPI/carla/dist/carla-*%d.%d-%s.egg' % (
        sys.version_info.major,
        sys.version_info.minor,
        'win-amd64' if os.name == 'nt' else 'linux-x86_64'))[0])   # your egg abs path
except IndexError:
    pass


import carla    # import from *.egg
import numpy as np
import pygame
import cv2
import queue
import time
import math

"""
Welcome to CARLA manual control.

Use ARROWS or WASD keys for control.

    W            : throttle
    S            : brake
    A/D          : steer left/right 
"""
import pygame

from pygame.locals import K_s
from pygame.locals import K_w
from pygame.locals import K_a
from pygame.locals import K_d

IMG_WIDTH = 1080
IMG_HEIGHT = 740
surface = None
SHOW_CAM = False



# ==============================================================================
# -- KeyboardControl -----------------------------------------------------------
# ==============================================================================


class KeyboardControl(object):
    """Class that handles keyboard input."""
    def __init__(self, player):

        self._control = carla.VehicleControl()
        self._steer_cache = 0.0
        self.player = player


    def parse_events(self, clock):
        self._parse_vehicle_keys(pygame.key.get_pressed(), clock.get_time())
        self.player.apply_control(self._control)

    def _parse_vehicle_keys(self, keys, milliseconds):
        if  keys[K_w]:
            self._control.throttle = min(self._control.throttle + 0.01, 1)
        else:
            self._control.throttle = 0.0
            # fix the velocity
            # self._control.throttle = 0.40

        if keys[K_s]:
            self._control.brake = min(self._control.brake + 0.2, 1)
        else:
            self._control.brake = 0

        steer_increment = 5e-4 * milliseconds
        if keys[K_a]:
            if self._steer_cache > 0:
                self._steer_cache = 0
            else:
                self._steer_cache -= steer_increment
        elif keys[K_d]:
            if self._steer_cache < 0:
                self._steer_cache = 0
            else:
                self._steer_cache += steer_increment
        else:
            self._steer_cache = 0.0
        self._steer_cache = min(0.7, max(-0.7, self._steer_cache))
        self._control.steer = round(self._steer_cache, 7)




class CarlaSyncMode(object):
    """
    Context manager to synchronize output from different sensors. Synchronous
    mode is enabled as long as we are inside this context

        with CarlaSyncMode(world, sensors) as sync_mode:
            while True:
                data = sync_mode.tick(timeout=1.0)

    """

    def __init__(self, world, *sensors, **kwargs):
        self.world = world
        self.sensors = sensors
        self.frame = None
        self.delta_seconds = 1.0 / kwargs.get('fps', 20)
        self._queues = []
        self._settings = None

    def __enter__(self):
        # some data about the simulation such as synchrony between client and server or rendering mode
        self._settings = self.world.get_settings()
        # ---- This is important carla.WorldSettings
        self.frame = self.world.apply_settings(carla.WorldSettings(
            no_rendering_mode=False,
            synchronous_mode=True,
            fixed_delta_seconds=self.delta_seconds))

        def make_queue(register_event):
            q = queue.Queue()
            register_event(q.put)
            self._queues.append(q)

        make_queue(self.world.on_tick)
        for sensor in self.sensors:
            make_queue(sensor.listen)
        return self

    def tick(self, timeout):
        # This method only has effect on synchronous mode, when both client and server move together.
        # The client tells the server when to step to the next frame and returns the id of the newly started frame.
        self.frame = self.world.tick()
        # get the data synchronous data
        data = [self._retrieve_data(q, timeout) for q in self._queues]
        assert all(x.frame == self.frame for x in data)
        return data

    def __exit__(self, *args, **kwargs):
        self.world.apply_settings(self._settings)

    def _retrieve_data(self, sensor_queue, timeout):
        while True:
            data = sensor_queue.get(timeout=timeout)
            # client timestamp == world timestamp
            if data.frame == self.frame:
                return data

def should_quit():
    """
    stop event
    :return:
    """
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            return True
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_ESCAPE:
                return True
    return False

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


def draw_image(surface, image, blend=False):
    array = np.frombuffer(image.raw_data, dtype=np.dtype("uint8"))
    array = np.reshape(array, (image.height, image.width, 4))
    array = array[:, :, :3]
    array = array[:, :, ::-1]
    image_surface = pygame.surfarray.make_surface(array.swapaxes(0, 1))
    if blend:
        image_surface.set_alpha(100)
    surface.blit(image_surface, (0, 0))

def get_speed(vehicle):
    """
    Compute speed of a vehicle in Km/h.

        :param vehicle: the vehicle for which speed is calculated
        :return: speed as a float in Km/h
    """
    vel = vehicle.get_velocity()

    return 3.6 * math.sqrt(vel.x ** 2 + vel.y ** 2 + vel.z ** 2)

def carla_main():

    actor_list = list()
    #######################
    # --- pygame show --- #
    #######################
    pygame.init()
    display = pygame.display.set_mode((IMG_WIDTH, IMG_HEIGHT), pygame.HWSURFACE | pygame.DOUBLEBUF)
    font = get_font()
    clock = pygame.time.Clock()
    try:
        ##################
        # --- client --- #
        ##################
        client = carla.Client('localhost', 2000)
        client.set_timeout(10.0) # connect with server

        #################
        # --- world --- #
        #################
        world = client.get_world()
        map = world.get_map()
        ###################
        # --- vehicle --- #
        ###################
        # --- blueprint --- #
        blueprint_library = world.get_blueprint_library()
        vehicle_bp = blueprint_library.filter('model3')[0]
        # vehicle property
        vehicle_bp.set_attribute('role_name', 'runner')
        green = '0.0, 255.0, 0.0'
        vehicle_bp.set_attribute('color', green)  # green
        # --- start point --- #
        # 1. spawn point
        spawn_point = map.get_spawn_points()[5]
        # # Transform(Location(x=-24.665806, y=-31.008774, z=2.171178), Rotation(pitch=0.000000, yaw=131.344223, roll=0.000000))
        # print('spawn_point:', spawn_point)
        vehicle = world.spawn_actor(vehicle_bp, spawn_point)
        actor_list.append(vehicle)
        time.sleep(3.0)
        # set the physics Determines whether an actor will be affected by physics or not
        # vehicle.set_simulate_physics(False)

        #############################
        # --- rgb-camera sensor --- #
        #############################
        # 1. blueprint
        rgb_camera_bp = blueprint_library.find('sensor.camera.rgb')
        # 2. set the attribute of camera
        rgb_camera_bp.set_attribute("image_size_x", "%f"%(IMG_WIDTH))  # image width
        rgb_camera_bp.set_attribute("image_size_y", "%f"%(IMG_HEIGHT))   # image height
        rgb_camera_bp.set_attribute("fov", "110")               # Horizontal field of view in degrees
        # 3. add camera sensor to the vehicle, put the sensor in the car
        spawn_point = carla.Transform(carla.Location(x=0.4, y=0.0, z=1.2), carla.Rotation(pitch=-10.0, yaw=0.0, roll=0.0))
        camera_rgb = world.spawn_actor(rgb_camera_bp, spawn_point, attach_to=vehicle)
        actor_list.append(camera_rgb)

        ######################
        # --- controller --- #
        ######################

        controller = KeyboardControl(vehicle)

        ############################################
        # --- Create a synchronous mode context ---#
        ############################################
        synchronous_fps = 15
        with CarlaSyncMode(world, camera_rgb, fps=synchronous_fps) as sync_mode:

            while True:
                #  quit the while
                if should_quit():
                    return

                # start clock
                clock.tick_busy_loop(synchronous_fps)
                # clock.tick(synchronous_fps)
                # Advance the simulation and wait for the data.
                snapshot, image_rgb = sync_mode.tick(timeout=2.0)

                # Control vehicle by keyboard
                controller.parse_events(clock)

                # frame per second
                fps = round(1.0 / snapshot.timestamp.delta_seconds)
                # Draw the display.
                draw_image(display, image_rgb)
                # vehicle velocity
                vehicle_velocity = get_speed(vehicle)
                # show the fps
                display.blit(font.render('% 5d FPS (client)' % clock.get_fps(), True, (0, 0, 0)), (8, 10))
                display.blit(font.render('% 5d FPS (server)' % fps, True, (0, 0, 0)), (8, 28))
                display.blit(font.render('% 5d mk/h (velocity)' % vehicle_velocity, True, (0, 0, 0)), (8, 46))
                pygame.display.flip()


    finally:
        print('destroying actors')
        for actor in actor_list:
            actor.destroy()
        pygame.quit()
        print('done.')

if __name__ == '__main__':
    carla_main()

