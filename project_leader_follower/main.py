"""
CARLA: following the vehicle
"""
import glob
import os
import sys

########################
#  --- carla API  ---  #
########################
try:
    sys.path.append(glob.glob('/home/liu/Car/carla/PythonAPI/carla/dist/carla-*%d.%d-%s.egg' % (
        sys.version_info.major,
        sys.version_info.minor,
        'win-amd64' if os.name == 'nt' else 'linux-x86_64'))[0])
except IndexError:
    pass

import carla
import time
import pygame
import logging
import argparse
from world_env import World
from control import KeyboardControl, VehiclePIDController, get_speed
from show_on_screen import HUD
from util import *



def main_loop(args):
    """s
    Running the simulator
    :return:
    """
    pygame.init()
    world = None
    try:
        #####################
        #  --- client  ---  #
        #####################
        see_it = args
        client = carla.Client(args.host, args.port)
        client.set_timeout(1.0)
        #####################
        # -- show info  --  #
        #####################
        hud = HUD(args.width, args.height)
        #####################
        #   --- world  ---  #
        #####################
        world = World(client.get_world(), hud, args)
        # print the route waypoints
        # world.draw_waypoints()
        # --- camera on the leader --- #
        world.leader_sensors()
        world.follow_sensors()
        #####################
        # --- controller ---#
        #####################
        leader_controller = KeyboardControl(world)
        follow_controller = VehiclePIDController(world.follower,
                                                 args_lateral=args_lat_hw_dict,
                                                 args_longitudinal=args_long_hw_dict)

        ####################################
        # --- leader and follower game --- #
        ####################################

        display = pygame.display.set_mode(
            (args.width, args.height),
            pygame.HWSURFACE | pygame.DOUBLEBUF)
        clock = pygame.time.Clock()    # game clock
        while True:
            clock.tick_busy_loop(10.0)     # 10 frame in second
            if leader_controller.parse_events(client, world, clock):
                return
            # time: ms
            world.tick(clock)
            # show the rgb-image
            pygame.display.flip()
            world.render(display)
            # --- follower by pid control --- #
            # the follower route
            start = world.follower.get_location()
            end = world.leader.get_location()
            follower_next_waypoint = world.set_destination(start, end)[0]
            # leader velocity
            target_speed = get_speed(world.leader)
            control = follow_controller.run_step(target_speed, follower_next_waypoint)
            world.follower.apply_control(control)


    finally:
        if world is not None:
            world.destroy()


if __name__ == '__main__':


    argparser = argparse.ArgumentParser(
        description='CARLA: following the vehicle')

    argparser.add_argument(
        '--host',
        metavar='H',
        default='127.0.0.1',
        help='IP of the host server (default: 127.0.0.1)')
    argparser.add_argument(
        '-p', '--port',
        metavar='P',
        default=2000,
        type=int,
        help='TCP port to listen to (default: 2000)')
    argparser.add_argument(
        '--res',
        metavar='WIDTHxHEIGHT',
        default='1280x720',
        # default='256x128',
        help='Window resolution (default: 1280x720)')
    argparser.add_argument(
        '--filter',
        metavar='PATTERN',
        default='jeep',
        help='Actor filter (default: "vehicle.*")')
    argparser.add_argument(
        '--vehicle_num',
        metavar='NUMBER',
        default='2',
        help='VEHICLE numbers')


    args = argparser.parse_args()
    # Screen size
    args.width, args.height = [int(x) for x in args.res.split('x')]
    logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)
    logging.info('listening to server %s:%s', args.host, args.port)
    print(__doc__)

    try:
        main_loop(args)
    except KeyboardInterrupt:
        print('\nCancelled by user. Bye!')
