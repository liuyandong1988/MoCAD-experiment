"""
MoCAD experiment 1-1 : Create a race road and spawn a vehicle
carla 0.9.9.4 https://github.com/carla-simulator/carla/releases
---------------------------------------------------------------
1. reload the world
2. create 30 vehicles
"""

import glob
import os
import sys


try:
    sys.path.append(glob.glob('/home/liu/carla/CARLA_0.9.9.4/PythonAPI/carla/dist/carla-*%d.%d-%s.egg' % (
        sys.version_info.major,
        sys.version_info.minor,
        'win-amd64' if os.name == 'nt' else 'linux-x86_64'))[0])   # your egg abs path
except IndexError:
    pass


import carla    # import from *.egg
import random
import time

actor_list = list()
VEHICLE_NUM = 30

try:
    ##################
    # --- client --- #
    ##################
    client = carla.Client('localhost', 2000)
    client.set_timeout(10.0) # connect with server

    #################
    # --- world --- #
    #################
    world = client.load_world('Town05')  # change the map
    # Couldn't change the town in CARLA 0.9.9.4 #2337
    map = world.get_map()
    # --- weather --- #
    # 1. change weather params: carla.WeatherParameters
    # weather = carla.WeatherParameters(
    #     cloudiness=80.0,s
    #     precipitation=30.0,
    #     sun_altitude_angle=70.0)
    # world.set_weather(weather)w
    # print(world.get_weather())
    # 2. set_weather: ClearNoon, HardRainNoon, WetCloudySunset ...
    world.set_weather(carla.WeatherParameters.ClearSunset)

    ###################
    # --- vehicle --- #
    ###################
    number_of_vehicles = 30
    # 1. blueprint
    blueprints = world.get_blueprint_library().filter('vehicle.*')
    # 2. spawn points
    spawn_points = world.get_map().get_spawn_points()
    number_of_spawn_points = len(spawn_points)
    if number_of_vehicles < number_of_spawn_points:
        random.shuffle(spawn_points)
    else:
        print('requested %d vehicles, but could only find %d spawn points'%(number_of_vehicles, number_of_spawn_points))
        number_of_vehicles = number_of_spawn_points
    # 3. spawn the vehicles
    batch = []
    for n, transform in enumerate(spawn_points):
        if n >= number_of_vehicles:
            break
        blueprint = random.choice(blueprints)
        if blueprint.has_attribute('color'):
            color = random.choice(blueprint.get_attribute('color').recommended_values)
            blueprint.set_attribute('color', color)
        if blueprint.has_attribute('driver_id'):
            driver_id = random.choice(blueprint.get_attribute('driver_id').recommended_values)
            blueprint.set_attribute('driver_id', driver_id)
        blueprint.set_attribute('role_name', 'autopilot')
        batch.append(world.spawn_actor(blueprint, transform))
    actor_list.extend(batch)

    time.sleep(10.0)



finally:
    print('destroying actors')
    for actor in actor_list:
        actor.destroy()
    print('done.')

