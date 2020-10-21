"""
MoCAD experiment 1-1 : Create a race road and spawn a vehicle
UE4 editor 4.24
carla 0.9.9.4
python 3.5
-------------------------------------------------------------
1. create a race road world
2. draw the waypoints
3. spawn a vehicle
4. navigating through waypoints
5. use the map
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
import pygame
import random
import time
import numpy as np
import weakref
import math

actor_list = list()

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
    # --- weather --- #
    # 1. change weather params: carla.WeatherParameters
    # weather = carla.WeatherParameters(
    #     cloudiness=80.0,
    #     precipitation=30.0,
    #     sun_altitude_angle=70.0)
    # world.set_weather(weather)
    # print(world.get_weather())
    # 2. set_weather: ClearNoon, HardRainNoon, WetCloudySunset ...
    world.set_weather(carla.WeatherParameters.ClearNoon)


    #####################
    # --- Landmarks --- #
    #####################
    # --- carla.Landmark --- #
    # opendrive_id = carla.Landmark.road_id
    # print(opendrive_id)
    # --- waypoints --- #
    waypoints = map.generate_waypoints(distance=3.0)
    for w in waypoints:
        world.debug.draw_string(w.transform.location, 'O', draw_shadow=False,
                                color=carla.Color(r=255, g=0, b=0), life_time=1.0,
                                persistent_lines=True)
    first_waypoint = waypoints[0]
    print('id: ', first_waypoint.id)  # identificator
    print('Transform: ', first_waypoint.transform)   # location and rotation
    print('Road id: ', first_waypoint.road_id)
    print('Next waypoint: ', first_waypoint.next(distance=3.0)[0])

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
    # spawn_point = map.get_spawn_points()[0]
    # # Transform(Location(x=-24.665806, y=-31.008774, z=2.171178), Rotation(pitch=0.000000, yaw=131.344223, roll=0.000000))
    # print('spawn_point:', spawn_point)
    # vehicle = world.spawn_actor(vehicle_bp, spawn_point)
    # 2. waypoint
    waypoint = waypoints[10]
    spawn_point = waypoint.transform
    spawn_point.location.z = 1.0
    vehicle = world.spawn_actor(vehicle_bp, spawn_point)
    actor_list.append(vehicle)

    ########################################
    # --- Navigating through waypoints --- #
    ########################################
    # disables the simulation of physics on this actor
    vehicle.set_simulate_physics(False)
    times = 0
    while times!=200:
        # Find next waypoint 1 meter ahead.
        next_waypoints = waypoint.next(1.0)
        waypoint = next_waypoints[0]
        # Teleport the vehicle.
        vehicle.set_transform(waypoint.transform)
        times += 1
        time.sleep(0.2)


    #######################################
    # --- Generating a map navigation --- #
    #######################################
    waypoint_tuple_list = map.get_topology()
    first_tuple = waypoint_tuple_list[0]
    for w in first_tuple:
        world.debug.draw_string(w.transform.location, 'w', draw_shadow=False,
                                color=carla.Color(r=0, g=255, b=0), life_time=10.0,
                                persistent_lines=True)


finally:
    print('destroying actors')
    for actor in actor_list:
        actor.destroy()
    print('done.')

