"""
MoCAD experiment 2-1 : Sensors
UE4 editor 4.24
carla 0.9.9.4
python 3.5
-----------------------------------------------------------
1. rgb-camera sensor
2. Asynchronous mode, server variable time step
3. Carla.vehicle apply_control
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
import time



IMG_WIDTH = 1080
IMG_HEIGHT = 740
surface = None
SHOW_CAM = False



def process_img(image):
    """
    process the image
    """
    global surface
    array = np.frombuffer(image.raw_data, dtype=np.dtype("uint8"))
    array = np.reshape(array, (image.height, image.width, 4))
    array = array[:, :, :3]
    array = array[:, :, ::-1] # switch r,g,b
    array = array.swapaxes(0, 1)  # exchange the width and height
    surface = pygame.surfarray.make_surface(array)   # Copy an array to a new surface
    # save the image
    if SHOW_CAM:
        cv2.imshow("RGB-image", array)
        cv2.imwrite('camera_3.png', array)
        cv2.waitKey(1)

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


class World_FPS():

    def __init__(self):
        self.server_clock = pygame.time.Clock()
        self.server_fps = None

    def on_server_tick(self, timestamp):
        self.server_clock.tick()
        self.server_fps = self.server_clock.get_fps()

def carla_main():

    actor_list = list()
    try:

        ##################
        # --- client --- #
        ##################
        client = carla.Client('localhost', 2000)
        client.set_timeout(3.0) # connect with server

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

        #############################
        # --- rgb-camera sensor --- #
        #############################
        # 1. blueprint
        rgb_camera_bp = blueprint_library.find('sensor.camera.rgb')
        # 2. set the attribute of camera
        rgb_camera_bp.set_attribute("image_size_x", "%f"%(IMG_WIDTH))  # image width
        rgb_camera_bp.set_attribute("image_size_y", "%f"%(IMG_HEIGHT))   # image height
        rgb_camera_bp.set_attribute("fov", "110")               # Horizontal field of view in degrees
        # 3. add camera sensor to the vehicle
        spawn_point = carla.Transform(carla.Location(x=2.5, y=0.0, z=1.0), carla.Rotation(pitch=-20.0, yaw=0.0, roll=0.0))
        sensor = world.spawn_actor(rgb_camera_bp, spawn_point, attach_to=vehicle)
        sensor.listen(lambda data: process_img(data))
        actor_list.append(sensor)


        ###################
        # --- Running --- #
        ###################
        pygame.init()
        # Open a window on the screen
        display  = pygame.display.set_mode([IMG_WIDTH, IMG_HEIGHT])
        font = get_font()
        # clock limits the frame
        clock = pygame.time.Clock()

        # server fps
        world_fps = World_FPS()
        world.on_tick(world_fps.on_server_tick)


        while True:
            clock.tick_busy_loop(10)
            vehicle.apply_control(carla.VehicleControl(throttle=1.0, brake=0.0, steer=0.0))
            # show the fps
            display.blit(font.render('% 5d FPS (client)' % clock.get_fps(), True, (0, 0, 0)), (8, 10))
            display.blit(font.render('% 5d FPS (server)' % world_fps.server_fps, True, (0, 0, 0)), (8, 28))
            pygame.display.flip()  # update
            display.blit(surface, (0, 0))


    finally:
        print('destroying actors')
        for actor in actor_list:
            actor.destroy()
        print('done.')

if __name__ == '__main__':
    carla_main()

