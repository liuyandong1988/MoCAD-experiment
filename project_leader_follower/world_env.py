
import sys
import os
# ==============================================================================
# -- Add PythonAPI for release mode --------------------------------------------
# ==============================================================================
try:
    sys.path.append('/home/liu/Car/carla/PythonAPI/carla')
except IndexError:
    pass



import carla
from sensor import CameraManager
from agents.navigation.global_route_planner import GlobalRoutePlanner
from agents.navigation.global_route_planner_dao import GlobalRoutePlannerDAO

class World(object):

    def __init__(self, world, hud, args):
        self.world = world
        self.hud = hud
        self.vehicle_num = int(args.vehicle_num)
        self.vehicles = [None for _ in range(self.vehicle_num)]
        self._vehicle_filter = args.filter   # the vehicle
        self.map = world.get_map()
        # server simulator
        self.world.on_tick(hud.on_world_tick)
        # route waypoint
        self._sampling_resolution = 1.0
        self._grp = None
        # restart the world simulator
        self.restart()

    def restart(self):
        """
        initial vehicles and sensors
        :return:
        """
        # two vehicle position information  10m between two vehicles
        waypoints = self.map.generate_waypoints(distance=1.0)
        leader_start, follower_start = waypoints[413], waypoints[401]
        # self.world.debug.draw_string(leader_start.transform.location, 'O', draw_shadow=False,
        #                              color=carla.Color(r=255, g=0, b=0), life_time=10.0,
        #                              persistent_lines=True)
        # self.world.debug.draw_string(follower_start.transform.location, 'O', draw_shadow=False,
        #                              color=carla.Color(r=255, g=0, b=0), life_time=10.0,
        #                              persistent_lines=True)

        # --- the first vehicle control by human --- #
        # Get a vehicle blueprint.
        leader_bp = self.world.get_blueprint_library().filter(self._vehicle_filter)[0]
        leader_bp.set_attribute('role_name', 'leader')
        leader_bp.set_attribute('color', '0, 255, 0')  # green color
        # Spawn the player
        leader_start = leader_start.transform
        leader_start.location.z = 1.0
        self.leader = self.world.spawn_actor(leader_bp, leader_start)
        # --- the second vehicle control by pid --- #
        # Get a vehicle blueprint.
        follower_bp = self.world.get_blueprint_library().filter(self._vehicle_filter)[0]
        follower_bp.set_attribute('role_name', 'follower')
        follower_bp.set_attribute('color', '255, 0, 0')  # red color
        # Spawn the player
        follower_start = follower_start.transform
        follower_start.location.z = 1.0
        self.follower = self.world.spawn_actor(follower_bp, follower_start)


    def draw_waypoints(self, distance=3, all_lines=False):
        """
        show the waypoints on the map
        :return:
        """
        waypoints = self.map.generate_waypoints(distance)

        line1, line2 = list(), list()
        for idx, w in enumerate(waypoints):
            if idx % 2 == 0:
                line1.append(w)
            else:
                line2.append(w)

        if all_lines:
            for w in waypoints:
                self.world.debug.draw_string(w.transform.location, 'O', draw_shadow=False,
                                             color=carla.Color(r=255, g=0, b=0), life_time=10.0,
                                             persistent_lines=True)
        else:
            for w in line1:
                self.world.debug.draw_string(w.transform.location, 'O', draw_shadow=False,
                                             color=carla.Color(r=255, g=0, b=0), life_time=10.0,
                                             persistent_lines=True)


    def set_destination(self, start_location, end_location):
        """
        This method creates a list of waypoints from agent's position to destination location
        based on the route returned by the global router.
        """

        self.start_waypoint = self.map.get_waypoint(start_location)
        self.end_waypoint = self.map.get_waypoint(end_location)

        follower_route_trace = self._trace_route(self.start_waypoint, self.end_waypoint)
        return follower_route_trace[-2]


    def _trace_route(self, start_waypoint, end_waypoint):
        """
        This method sets up a global router and returns the
        optimal route from start_waypoint to end_waypoint.
        """
        # Setting up global router
        if self._grp is None:
            dao = GlobalRoutePlannerDAO(
                self.map, sampling_resolution=self._sampling_resolution)
            grp = GlobalRoutePlanner(dao)
            grp.setup()
            self._grp = grp

        # Obtain route plan
        route = self._grp.trace_route(
            start_waypoint.transform.location,
            end_waypoint.transform.location)

        return route


    def leader_sensors(self):
        """
        Add sensors to leader
        :return:
        """
        # Set up the sensors.
        self.leader_camera_manager = CameraManager(self.leader, self.hud)
        self.leader_camera_manager.transform_index = 1                       # choose the camera position
        # Keep same camera config if the camera manager exists. rgb-camera
        cam_index = 0
        self.leader_camera_manager.set_sensor(cam_index, notify=False)
        # actor_type = get_actor_display_name(self.player)
        # self.hud.notification(actor_type)

    def follow_sensors(self):
        # Set up the sensors.
        self.follower_camera_manager = CameraManager(self.follower, self.hud)
        self.follower_camera_manager.transform_index = 1                       # choose the camera position
        # Keep same camera config if the camera manager exists. rgb-camera
        cam_index = 0
        self.follower_camera_manager.set_sensor(cam_index, notify=False)

    def tick(self, clock):
        self.hud.tick(self, clock)

    def render(self, display):
        self.follower_camera_manager.render(display)
        self.hud.render(display)


    def destroy(self):
        """Destroys all actors"""
        actors = [
            self.leader_camera_manager.sensor,
            self.follower_camera_manager.sensor,
            self.leader,
            self.follower]
        for actor in actors:
            if actor is not None:
                actor.destroy()