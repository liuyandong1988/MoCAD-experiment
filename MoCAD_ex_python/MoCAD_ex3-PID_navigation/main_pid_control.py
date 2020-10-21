"""
MoCAD experiment 3 : Example of automatic vehicle PID control
UE4 editor
carla 0.9.9.4
-----------------------------------------------------------
1. Show the map with waypoints
2. plan a path
3. spawn a vehicle
4. Control a vehicle by PID
"""


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
import pygame
import time
from sensor import CameraManager
from local_planner import LocalPlanner
from utils import *
import numpy as np


actor_list = list()

try:
    ##########################
    # --- initialization --- #
    ##########################
    # connect with the server
    client = carla.Client('localhost', 2000)
    client.set_timeout(10.0)
    # display mode
    width, height = 1080, 780
    display = pygame.display.set_mode(
        (width, height),
        pygame.HWSURFACE | pygame.DOUBLEBUF)
    # world
    world = client.get_world()

    ##########################
    # --- plan the route --- #
    ##########################
    # all waypoints based on the distance
    map = world.get_map()
    waypoints = map.generate_waypoints(distance=0.5)
    # the waypoints on the samw lane
    left_lane_w, right_lane_w = list(), list()
    for idx, w in enumerate(waypoints):
        if idx % 2 == 0:
            right_lane_w.append(w)
        else:
            left_lane_w.append(w)
    # planning route
    planning_route = right_lane_w[-90:] + right_lane_w[:500]   # distance = 0.5 --> 500, distance = 1 --> 200
    for w in planning_route:
        world.debug.draw_string(w.transform.location, 'O', draw_shadow=False,
                                      color=carla.Color(r=255, g=0, b=0), life_time=60.0,
                                      persistent_lines=True)
    # start and end
    start_waypoint, end_waypoint = planning_route[0], planning_route[-1]


    #########################
    # --- spawn the car --- #
    #########################
    # blueprins
    blueprint_library = world.get_blueprint_library()  # carla.BlueprintLibrary
    # Choose a vehicle blueprint
    vehicle_bp = blueprint_library.filter('model3')[0]
    # the start point
    spawn_point = start_waypoint.transform
    spawn_point.location.z = 1.0          # important
    vehicle = world.spawn_actor(vehicle_bp, spawn_point)
    actor_list.append(vehicle)
    time.sleep(1.0)

    #############################
    # --- Set up the sensor --- #
    #############################
    pygame.init()
    pygame.font.init()
    gamma = 2.2
    # show the information on the screen
    hud = HUD(width, height)
    camera_manager = CameraManager(vehicle, hud, gamma)
    camera_manager.set_sensor(0, notify=False)  # sensor index rgb

    ##########################
    # --- PID controller --- #
    ##########################
    # local planner
    local_planner = LocalPlanner(vehicle, planning_route)
    ##################################################
    # --- Vehicle Trajectory Live Plotting Setup --- #
    ##################################################
    enable_live_plot = True
    lp_traj, lp_1d, trajectory_fig, forward_speed_fig, throttle_fig, steer_fig = draw_trajectory(planning_route)

    # live plotter is disabled, hide windows
    if not enable_live_plot:
        lp_traj._root.withdraw()
        lp_1d._root.withdraw()
    # Store history
    x_history = list()
    y_history = list()
    yaw_history = list()
    speed_history = list()
    time_history = list()
    current_timestamp = 0
    #########################
    # ---  PID control  --- #
    #########################
    clock = pygame.time.Clock()           # track amount of time or to manage
    world.on_tick(hud.on_world_tick)      # calculate the server fps
    while True:
        clock.tick(15)
        current_timestamp += 1
        # client each timestamp: show the information
        hud.tick(world, vehicle, clock)
        # show the screen
        camera_manager.render(display)
        hud.render(display)
        # update the screen
        pygame.display.flip()
        # --- control PID --- #
        control = local_planner.run_step()
        if control:
            vehicle.apply_control(control)
            # --- Update live plotter with new feedback --- #
            current_pos = vehicle.get_location()
            current_speed = np.sqrt(np.square(vehicle.get_velocity().x) + np.square(vehicle.get_velocity().y))
            cmd_throttle, cmd_steer = control.throttle, control.steer
            # --- show the vehicle information --- #
            x_history.append(current_pos.x)
            y_history.append(current_pos.y)
            speed_history.append(current_speed)
            time_history.append(current_speed)
            # trajectory
            trajectory_fig.roll("trajectory", current_pos.x, current_pos.y)
            trajectory_fig.roll("car", current_pos.x, current_pos.y)
            # speed, throttle, steer
            forward_speed_fig.roll("forward_speed",
                                   current_timestamp,
                                   current_speed * 3.6)  # m/s to km/h
            throttle_fig.roll("throttle", current_timestamp, cmd_throttle * 100)
            # brake_fig.roll("brake", current_timestamp, cmd_brake)
            steer_fig.roll("steer", current_timestamp, cmd_steer * 180 / np.pi)
            # update
            lp_traj.refresh()
            lp_1d.refresh()
        else:
            # finish running
            vehicle.apply_control(carla.VehicleControl(throttle=0.0, steer=0.0))
            # Store the various outputs
            store_trajectory_plot(trajectory_fig.fig, 'trajectory.png')
            store_trajectory_plot(forward_speed_fig.fig, 'forward_speed.png')
            store_trajectory_plot(throttle_fig.fig, 'throttle_output.png')
            store_trajectory_plot(steer_fig.fig, 'steer_output.png')
            write_trajectory_file(x_history, y_history, speed_history, time_history)
            break

    # update the camera screen
    pygame.display.flip()

finally:
    print('destroying actors')
    for actor in actor_list:
        actor.destroy()
    print('done.')
    pygame.quit()
