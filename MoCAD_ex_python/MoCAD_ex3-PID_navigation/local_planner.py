import math
from collections import deque
from pid_controller import VehiclePIDController

class LocalPlanner():

    FPS = 15 # USE for dt

    def __init__(self, vehicle, global_plan):
        self._vehicle = vehicle
        self._map = vehicle.get_world().get_map()
        self.waypoints_queue = deque(maxlen=20000)  # queue with tuples of (waypoint, RoadOption)
        self._init_global_plan(global_plan)
        self._buffer_size = 5
        self._waypoint_buffer = deque(maxlen=self._buffer_size)
        self._init_controller()


    def _init_controller(self):
        """
        Controller initialization.
        """
        # Default parameters
        self.args_lat_hw_dict = {
            'K_P': 0.75,
            'K_D': 0.02,
            'K_I': 0.4,
            'dt': 1.0 / self.FPS}
        self.args_lat_city_dict = {
            'K_P': 0.58,
            'K_D': 0.02,
            'K_I': 0.5,
            'dt': 1.0 / self.FPS}
        self.args_long_hw_dict = {
            'K_P': 0.37,
            'K_D': 0.024,
            'K_I': 0.032,
            'dt': 1.0 / self.FPS}
        self.args_long_city_dict = {
            'K_P': 0.15,
            'K_D': 0.05,
            'K_I': 0.07,
            'dt': 1.0 / self.FPS}
        self._min_distance = 3
        self._target_speed = self._vehicle.get_speed_limit()


    def _init_global_plan(self, plan):

        for elem in plan:
            self.waypoints_queue.append(elem)


    def run_step(self, target_speed=None):
        """
        Execute one step of local planning which involves
        running the longitudinal and lateral PID controllers to
        follow the waypoints trajectory.
        """

        if target_speed is not None:
            self._target_speed = target_speed
        else:
            # self._target_speed = self._vehicle.get_speed_limit()  # the landmarkType.MaximumSpeed
            # modify the target speed, control the vehicle easily
            self._target_speed = 20

        # Buffering the waypoints
        if not self._waypoint_buffer:
            for i in range(self._buffer_size):
                if self.waypoints_queue:
                    self._waypoint_buffer.append(self.waypoints_queue.popleft())
                else:
                    break

        # Current vehicle waypoint
        self._current_waypoint = self._map.get_waypoint(self._vehicle.get_location())
        # Target waypoint
        try:
            self.target_waypoint = self._waypoint_buffer[0]
            # fast modify pid
            args_lat = self.args_lat_hw_dict
            args_long = self.args_long_hw_dict
            # slow
            # args_lat = self.args_lat_city_dict
            # args_long = self.args_long_city_dict
            pid_lat_mark = True
            self._pid_controller = VehiclePIDController(self._vehicle,
                                                        args_lateral=args_lat,
                                                        args_longitudinal=args_long,
                                                        pid_lat_mark=pid_lat_mark)
            if pid_lat_mark:
                control = self._pid_controller.run_step(self._target_speed, self.target_waypoint)
            else:
                control = self._pid_controller.run_step(self._target_speed, self.target_waypoint, self._waypoint_buffer)

            # Purge the queue of obsolete waypoints
            vehicle_transform = self._vehicle.get_transform()
            max_index = -1
            for i, waypoint in enumerate(self._waypoint_buffer):
                if distance_vehicle(
                        waypoint, vehicle_transform) < self._min_distance:
                    max_index = i
            if max_index >= 0:
                for i in range(max_index + 1):
                    self._waypoint_buffer.popleft()
            return control

        except IndexError:
            print('Finish running !')
            return None

def distance_vehicle(waypoint, vehicle_transform):
    """
    Returns the 2D distance from a waypoint to a vehicle

        :param waypoint: actual waypoint
        :param vehicle_transform: transform of the target vehicle
    """
    loc = vehicle_transform.location
    x = waypoint.transform.location.x - loc.x
    y = waypoint.transform.location.y - loc.y

    return math.sqrt(x * x + y * y)

