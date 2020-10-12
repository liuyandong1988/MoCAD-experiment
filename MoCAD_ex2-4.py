"""
MoCAD experiment 2-1 : Sensors
-----------------------------------------------------------
sensor reference: https://carla.readthedocs.io/en/latest/ref_sensors/
1. rgb-camera sensor
2. collision sensor
3. depth-camera sensor
4. gnss sensor
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
import time
import weakref



IMG_WIDTH = 1080
IMG_HEIGHT = 740
surface = None
depth_surface = None
SHOW_CAM = False
collision_mark = False
cnts = 0

def process_collision(event):
    """
    process collision
    """
    global collision_mark, cnts
    collision_mark = True
    cnts += 1
    print('Collision frame:  Collision times:', event.frame, cnts)


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


def process_depth_img(image):
    global depth_surface
    image.convert(carla.ColorConverter.LogarithmicDepth)           #  carla.ColorConverter.Depth LogarithmicDepth, depth
    array = np.frombuffer(image.raw_data, dtype=np.dtype("uint8"))
    array = np.reshape(array, (image.height, image.width, 4))
    array = array[:, :, :3]
    array = array[:, :, ::-1]
    depth_surface = pygame.surfarray.make_surface(array.swapaxes(0, 1))
    # save the image
    if SHOW_CAM:
        image_name = round(image.frame, 8)
        image.save_to_disk('./image/%d' % image_name)

def process_gnss(data):
    height = data.altitude
    latitude = data.latitude
    longitude = data.longitude
    print('------------------- GNSS -----------------------')
    print('Global navigation satellite system: height_{}, latitude_{}, longitude_{}'
          .format(round(height,3), round(latitude, 3), round(longitude, 3)))

def process_imu(data):
    accelerometer = data.accelerometer
    location = data.transform.location
    x, y, z = location.x, location.y, location.z
    print('------------------- IMU -----------------------')
    print("Vehicle accelerometer: {}, position:({}, {}, {})"
          .format(accelerometer, round(x, 2), round(y, 2), round(z, 2)))


class ObstacleSensor(object):
    def __init__(self, parent_actor):
        self.sensor = None
        self._history = []
        self._parent = parent_actor
        self._event_count = 0
        self.obstacle_distance = None
        self.sensor_transform = carla.Transform(carla.Location(x=1.6, z=1.7), carla.Rotation(yaw=0)) # Put this sensor on the windshield of the car.
        world = self._parent.get_world()
        bp = world.get_blueprint_library().find('sensor.other.obstacle')
        bp.set_attribute('distance', '5.0')      # sensor distance
        bp.set_attribute('hit_radius', '1.0')
        bp.set_attribute('only_dynamics', 'False')
        bp.set_attribute('debug_linetrace', 'False')
        bp.set_attribute('sensor_tick', '0.1')
        self.sensor = world.spawn_actor(bp, self.sensor_transform, attach_to=self._parent)
        weak_self = weakref.ref(self)
        self.sensor.listen(lambda event: ObstacleSensor._process_event(weak_self, event))


    @staticmethod
    def _process_event(weak_self, event):
        self = weak_self()
        if not self:
            return
        if event.other_actor.type_id.startswith('vehicle.'):
            vehicle = event.other_actor.type_id
        self.obstacle_distance = event.distance
        print('------------------- Obstacle -----------------------')
        print ("Obstacle sensor Distance %f" % (self.obstacle_distance))



def carla_main():

    actor_list = list()
    global collision_mark
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
        rgb_sensor = world.spawn_actor(rgb_camera_bp, spawn_point, attach_to=vehicle)
        rgb_sensor.listen(lambda data: process_img(data))
        actor_list.append(rgb_sensor)

        # ############################
        # # --- Collision sensor --- #
        # ############################
        # collision_bp = blueprint_library.find('sensor.other.collision')
        # collision_sensor = world.spawn_actor(collision_bp, carla.Transform(), attach_to=vehicle)
        # actor_list.append(collision_sensor)
        # collision_sensor.listen(lambda event: process_collision(event))

        ########################
        # --- Depth sensor --- #
        ########################
        depth_image_bp = blueprint_library.find("sensor.camera.depth")
        depth_image_bp.set_attribute("image_size_x", "%f"%(IMG_WIDTH))
        depth_image_bp.set_attribute("image_size_y", "%f"%(IMG_HEIGHT))
        depth_image_bp.set_attribute("fov", "110")
        depth_sensor = world.spawn_actor(depth_image_bp, spawn_point, attach_to=vehicle)
        actor_list.append(depth_sensor)
        depth_sensor.listen(lambda image: process_depth_img(image))

        # #######################
        # # --- GNSS sensor --- #
        # #######################
        # gnss_bp = blueprint_library.find('sensor.other.gnss')
        # gnss_sensor = world.spawn_actor(gnss_bp, carla.Transform(carla.Location(x=1.0, z=2.8)), attach_to=vehicle)
        # actor_list.append(gnss_sensor)
        # gnss_sensor.listen(lambda data: process_gnss(data))
        #
        # ######################
        # # --- IMU sensor --- #
        # ######################
        # imu_bp = blueprint_library.find('sensor.other.imu')
        # imu_sensor = world.spawn_actor(imu_bp, carla.Transform(carla.Location(x=0.0, z=1.0, y=0.0)), attach_to=vehicle)
        # actor_list.append(imu_sensor)
        # imu_sensor.listen(lambda  data: process_imu(data))


        ###########################
        # --- obstacle sensor --- #
        ###########################
        # Issues with obstacle detector #1286
        obstacle_sensor = ObstacleSensor(vehicle)

        ###################
        # --- Running --- #
        ###################
        pygame.init()
        # Open a window on the screen
        display  = pygame.display.set_mode([IMG_WIDTH, IMG_HEIGHT])
        # clock limits the frame
        clock = pygame.time.Clock()


        while True:
            clock.tick_busy_loop(10)
            if not collision_mark:
                vehicle.apply_control(carla.VehicleControl(throttle=0.5, brake=0.0, steer=0.0))
            else:
                vehicle.apply_control(carla.VehicleControl(throttle=0.0, brake=1.0, steer=0.0))
            pygame.display.flip()  # update
            # change the show image: surface, depth_surface
            display.blit(surface, (0, 0))


    finally:
        print('destroying actors')
        for actor in actor_list:
            actor.destroy()
        print('done.')

if __name__ == '__main__':
    carla_main()

