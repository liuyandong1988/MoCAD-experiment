import glob
import os
import sys
try:
    sys.path.append(glob.glob('/home/liu/Car/carla/PythonAPI/carla/dist/carla-*%d.%d-%s.egg' % (
        sys.version_info.major,
        sys.version_info.minor,
        'win-amd64' if os.name == 'nt' else 'linux-x86_64'))[0])
except IndexError:
    pass
import carla

import random
import time
import numpy as np
import cv2
import pygame


IM_WIDTH = 780
IM_HEIGHT = 640
surface = None
backward_surface = None

# def process_img(image):
#     global times
#     times += 1
#     print(times)
#     i3 = cv2.imread('1.png')
#     # i3 = i3.reshape((862, 1796, 3))
#     # cv2.resize(InputArray, src, OutputArray dst, Size, fx, fy, interpolation)
#     # i3 = i3[:, :, :3]
#     cv2.imshow("", i3)
#     cv2.waitKey(50)
#     return i3/255.0

def process_forward_img(image):

    global surface
    # image.convert(image.raw_data)
    array = np.frombuffer(image.raw_data, dtype=np.dtype("uint8"))
    array = np.reshape(array, (image.height, image.width, 4))
    array = array[:, :, :3]
    array = array[:, :, ::-1]  # from back
    surface = pygame.surfarray.make_surface(array.swapaxes(0, 1))
    # self.rgb_image = array
    # i = np.array(image.raw_data)
    # i2 = i.reshape((IM_HEIGHT, IM_WIDTH, 4))
    # i3 = i2[:, :, :3]

def process_backward_img(image):

    global backward_surface
    # image.convert(image.raw_data)
    array = np.frombuffer(image.raw_data, dtype=np.dtype("uint8"))
    array = np.reshape(array, (image.height, image.width, 4))
    array = array[:, :, :3]
    array = array[:, :, ::-1]  # from back
    backward_surface = pygame.surfarray.make_surface(array.swapaxes(0, 1))
    # self.rgb_image = array
    # i = np.array(image.raw_data)
    # i2 = i.reshape((IM_HEIGHT, IM_WIDTH, 4))
    # i3 = i2[:, :, :3]


actor_list = []
try:
    # --- client --- #
    client = carla.Client('localhost', 2000)
    client.set_timeout(2.0)
    # --- server simulator --- #
    world = client.get_world()
    blueprint_library = world.get_blueprint_library()
    # vehicle
    vehicle_bp = blueprint_library.filter('jeep')[0]
    vehicle_bp.set_attribute('color', '255, 0, 0')
    # draw the way points for defining the start
    map = world.get_map()
    waypoints = map.generate_waypoints(distance= 5)
    # # draw the way points
    # for idx, w in enumerate(waypoints):
    #     if idx == 99:
    #         world.debug.draw_string(w.transform.location, 'O', draw_shadow=False,
    #                                 color=carla.Color(r=255, g=0, b=0), life_time=10.0,
    #                                 persistent_lines=True)
    #     else:
    #         pass
    start_point = waypoints[99].transform
    start_point.location.z = 1.0
    vehicle = world.spawn_actor(vehicle_bp, start_point)
    actor_list.append(vehicle)
    # --- sensor --- #
    # put the rgb-camera on the vehicle
    camera_bp = blueprint_library.find('sensor.camera.rgb')
    # change the dimensions of the image
    camera_bp.set_attribute('image_size_x', f'{IM_WIDTH}')
    camera_bp.set_attribute('image_size_y', f'{IM_HEIGHT}')
    camera_bp.set_attribute('fov', '110')
    # - forward camera - #
    forward_camera_spawn_point = carla.Transform(carla.Location(x=2.5, z=0.7))
    # spawn the sensor and attach to vehicle.
    forward_camera = world.spawn_actor(camera_bp, forward_camera_spawn_point, attach_to=vehicle)
    # add sensor to list of actors
    forward_camera.listen(lambda data: process_forward_img(data))
    actor_list.append(forward_camera)
    # - backward camera - #
    backward_camera_spawn_point = carla.Transform(carla.Location(x=-2.0, y=0, z=2), carla.Rotation(pitch=0.0, yaw=180.0, roll=0.0))
    backward_camera = world.spawn_actor(camera_bp, backward_camera_spawn_point, attach_to=vehicle)
    backward_camera.listen(lambda data: process_backward_img(data))
    # pygame set the mode
    display=pygame.display.set_mode([IM_WIDTH, IM_HEIGHT])
    display_back=pygame.display.set_mode([IM_WIDTH, IM_HEIGHT])
    pygame.init()
    # clock show the frame
    clock = pygame.time.Clock()  # 创建一个对象来帮助跟踪时间

    while True:
        clock.tick_busy_loop(10) # control the frame
        world.tick()             # world server waiting for client
        pygame.display.flip()    # update
        display.blit(surface, (0, 0))
        # display_back.blit(backward_surface, (0, 0))
        vehicle.apply_control(carla.VehicleControl(throttle=0.5, steer=0.0))  # control the vehicle



    # # spawn_point = world.get_map().get_spawn_points()[0]
    # # vehicle = world.spawn_actor(bp, spawn_point)
    # # vehicle.apply_control(carla.VehicleControl(throttle=1.0, steer=0.0))
    # # vehicle.set_autopilot(True)  # if you just wanted some NPCs to drive.
    #
    # actor_list.append(vehicle)
    #
    # # https://carla.readthedocs.io/en/latest/cameras_and_sensors
    # # get the blueprint for this sensor
    # blueprint = blueprint_library.find('sensor.camera.rgb')
    # # change the dimensions of the image
    # blueprint.set_attribute('image_size_x', f'{IM_WIDTH}')
    # blueprint.set_attribute('image_size_y', f'{IM_HEIGHT}')
    # blueprint.set_attribute('fov', '110')
    #
    # # Adjust sensor relative to vehicle
    # spawn_point = carla.Transform(carla.Location(x=2.5, z=0.7))
    # # spawn the sensor and attach to vehicle.
    # sensor = world.spawn_actor(blueprint, spawn_point, attach_to=vehicle)
    # # add sensor to list of actors
    # actor_list.append(sensor)
    # # do something with this sensor
    # sensor.listen(lambda data: process_img(data))
    #
    # # pygame set the mode
    # display=pygame.display.set_mode([IM_WIDTH, IM_HEIGHT])
    # pygame.init()
    # # clock show the frame
    # clock = pygame.time.Clock()  # 创建一个对象来帮助跟踪时间
    #
    # while True:
    #     clock.tick_busy_loop(10) # control the frame
    #     world.tick()             # world server waiting for client
    #     pygame.display.flip()    # update
    #     display.blit(surface, (0, 0))
    #     vehicle.apply_control(carla.VehicleControl(throttle=1.0, steer=0.0))  # control the vehicle

finally:
    print('destroying actors')
    for actor in actor_list:
        actor.destroy()
    print('done.')
