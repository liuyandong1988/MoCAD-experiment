"""Show info on the screen."""


import pygame
import os
import datetime
import math
import carla
import live_plotter as lv

FIGSIZE_X_INCHES   = 8      # x figure size of feedback in inches
FIGSIZE_Y_INCHES   = 8      # y figure size of feedback in inches
PLOT_LEFT          = 0.1    # in fractions of figure width and height
PLOT_BOT           = 0.1
PLOT_WIDTH         = 0.8
PLOT_HEIGHT        = 0.8
TOTAL_EPISODE_FRAMES = 900


# controller output directory
CONTROLLER_OUTPUT_FOLDER = os.path.dirname(os.path.realpath(__file__)) + \
                           '/result_output/'


def write_trajectory_file(x_list, y_list, v_list, t_list):
    create_controller_output_dir(CONTROLLER_OUTPUT_FOLDER)
    file_name = os.path.join(CONTROLLER_OUTPUT_FOLDER, 'trajectory.txt')

    with open(file_name, 'w') as trajectory_file:
        for i in range(len(x_list)):
            trajectory_file.write('%3.3f, %3.3f, %2.3f, %6.3f\n' %\
                                  (x_list[i], y_list[i], v_list[i], t_list[i]))


def create_controller_output_dir(output_folder):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

def store_trajectory_plot(graph, fname):
    """ Store the resulting plot.
    """
    create_controller_output_dir(CONTROLLER_OUTPUT_FOLDER)

    file_name = os.path.join(CONTROLLER_OUTPUT_FOLDER, fname)
    graph.savefig(file_name)


class HUD(object):
    """
    Class for HUD text
    """

    def __init__(self, width, height):
        """
        Constructor method
        """
        self.dim = (width, height)
        font = pygame.font.Font(pygame.font.get_default_font(), 20)
        font_name = 'courier' if os.name == 'nt' else 'mono'
        fonts = [x for x in pygame.font.get_fonts() if font_name in x]
        default_font = 'ubuntumono'
        mono = default_font if default_font in fonts else fonts[0]
        mono = pygame.font.match_font(mono)
        self._font_mono = pygame.font.Font(mono, 12 if os.name == 'nt' else 14)
        self._notifications = FadingText(font, (width, 40), (0, height - 40))
        self.help = HelpText(pygame.font.Font(mono, 24), width, height)
        self.server_fps = 0
        self.frame = 0
        self.simulation_time = 0
        self._show_info = True
        self._info_text = []
        self._server_clock = pygame.time.Clock()

    def on_world_tick(self, timestamp):
        """
        Gets informations from the world at every tick
        """
        self._server_clock.tick()
        self.server_fps = self._server_clock.get_fps()
        self.frame = timestamp.frame_count
        self.simulation_time = timestamp.elapsed_seconds

    def tick(self, world, player, clock):
        """
        HUD method for each tick
        """
        self._notifications.tick(world, clock)
        if not self._show_info:
            return
        transform = player.get_transform()
        vel = player.get_velocity()
        control = player.get_control()
        # vehicle orientation
        heading = 'N' if abs(transform.rotation.yaw) < 89.5 else ''
        heading += 'S' if abs(transform.rotation.yaw) > 90.5 else ''
        heading += 'E' if 179.5 > transform.rotation.yaw > 0.5 else ''
        heading += 'W' if -0.5 > transform.rotation.yaw > -179.5 else ''
        # colhist = world.collision_sensor.get_collision_history()
        # collision = [colhist[x + self.frame - 200] for x in range(0, 200)]
        # max_col = max(1.0, max(collision))
        # collision = [x / max_col for x in collision]
        vehicles = world.get_actors().filter('vehicle.*')

        self._info_text = [
            'Server:  % 16.0f FPS' % self.server_fps,
            'Client:  % 16.0f FPS' % clock.get_fps(),
            '',
            'Vehicle: % 20s' % get_actor_display_name(player, truncate=20),
            # 'Map:     % 20s' % world.map.name,
            'Simulation time: % 12s' % datetime.timedelta(seconds=int(self.simulation_time)),
            '',
            'Speed:   % 15.0f km/h' % (3.6 * math.sqrt(vel.x**2 + vel.y**2 + vel.z**2)),
            u'Heading:% 16.0f\N{DEGREE SIGN} % 2s' % (transform.rotation.yaw, heading),
            'Location:% 20s' % ('(% 5.1f, % 5.1f)' % (transform.location.x, transform.location.y)),
            # 'GNSS:% 24s' % ('(% 2.6f, % 3.6f)' % (world.gnss_sensor.lat, world.gnss_sensor.lon)),
            'Height:  % 18.0f m' % transform.location.z,
            '']
        if isinstance(control, carla.VehicleControl):
            self._info_text += [
                ('Throttle:', control.throttle, 0.0, 1.0),
                ('Steer:', control.steer, -1.0, 1.0),
                ('Brake:', control.brake, 0.0, 1.0),
                ('Reverse:', control.reverse),
                ('Hand brake:', control.hand_brake),
                ('Manual:', control.manual_gear_shift),
                'Gear:        %s' % {-1: 'R', 0: 'N'}.get(control.gear, control.gear)]
        elif isinstance(control, carla.WalkerControl):
            self._info_text += [
                ('Speed:', control.speed, 0.0, 5.556),
                ('Jump:', control.jump)]
        # self._info_text += [
        #     '',
        #     'Collision:',
        #     collision,
        #     '',
        #     'Number of vehicles: % 8d' % len(vehicles)]

        if len(vehicles) > 1:
            self._info_text += ['Nearby vehicles:']

        def dist(l):
            return math.sqrt((l.x - transform.location.x)**2 + (l.y - transform.location.y)
                             ** 2 + (l.z - transform.location.z)**2)
        vehicles = [(dist(x.get_location()), x) for x in vehicles if x.id != player.id]

        for dist, vehicle in sorted(vehicles):
            if dist > 200.0:
                break
            vehicle_type = get_actor_display_name(vehicle, truncate=22)
            self._info_text.append('% 4dm %s' % (dist, vehicle_type))

    def toggle_info(self):
        """Toggle info on or off"""
        self._show_info = not self._show_info

    def notification(self, text, seconds=2.0):
        """Notification text"""
        self._notifications.set_text(text, seconds=seconds)

    def error(self, text):
        """Error text"""
        self._notifications.set_text('Error: %s' % text, (255, 0, 0))

    def render(self, display):
        """Render for HUD class"""
        if self._show_info:
            info_surface = pygame.Surface((220, self.dim[1]))
            info_surface.set_alpha(100)
            display.blit(info_surface, (0, 0))
            v_offset = 4
            bar_h_offset = 100
            bar_width = 106
            for item in self._info_text:
                if v_offset + 18 > self.dim[1]:
                    break
                if isinstance(item, list):
                    if len(item) > 1:
                        points = [(x + 8, v_offset + 8 + (1 - y) * 30) for x, y in enumerate(item)]
                        pygame.draw.lines(display, (255, 136, 0), False, points, 2)
                    item = None
                    v_offset += 18
                elif isinstance(item, tuple):
                    if isinstance(item[1], bool):
                        rect = pygame.Rect((bar_h_offset, v_offset + 8), (6, 6))
                        pygame.draw.rect(display, (255, 255, 255), rect, 0 if item[1] else 1)
                    else:
                        rect_border = pygame.Rect((bar_h_offset, v_offset + 8), (bar_width, 6))
                        pygame.draw.rect(display, (255, 255, 255), rect_border, 1)
                        fig = (item[1] - item[2]) / (item[3] - item[2])
                        if item[2] < 0.0:
                            rect = pygame.Rect(
                                (bar_h_offset + fig * (bar_width - 6), v_offset + 8), (6, 6))
                        else:
                            rect = pygame.Rect((bar_h_offset, v_offset + 8), (fig * bar_width, 6))
                        pygame.draw.rect(display, (255, 255, 255), rect)
                    item = item[0]
                if item:  # At this point has to be a str.
                    surface = self._font_mono.render(item, True, (255, 255, 255))
                    display.blit(surface, (8, v_offset))
                v_offset += 18
        self._notifications.render(display)
        self.help.render(display)


class FadingText(object):
    """ Class for fading text """

    def __init__(self, font, dim, pos):
        """Constructor method"""
        self.font = font
        self.dim = dim
        self.pos = pos
        self.seconds_left = 0
        self.surface = pygame.Surface(self.dim)

    def set_text(self, text, color=(255, 255, 255), seconds=2.0):
        """Set fading text"""
        text_texture = self.font.render(text, True, color)
        self.surface = pygame.Surface(self.dim)
        self.seconds_left = seconds
        self.surface.fill((0, 0, 0, 0))
        self.surface.blit(text_texture, (10, 11))

    def tick(self, _, clock):
        """Fading text method for every tick"""
        delta_seconds = 1e-3 * clock.get_time()
        self.seconds_left = max(0.0, self.seconds_left - delta_seconds)
        self.surface.set_alpha(500.0 * self.seconds_left)

    def render(self, display):
        """Render fading text method"""
        display.blit(self.surface, self.pos)

class HelpText(object):
    """ Helper class for text render"""

    def __init__(self, font, width, height):
        """Constructor method"""
        lines = __doc__.split('\n')
        self.font = font
        self.dim = (680, len(lines) * 22 + 12)
        self.pos = (0.5 * width - 0.5 * self.dim[0], 0.5 * height - 0.5 * self.dim[1])
        self.seconds_left = 0
        self.surface = pygame.Surface(self.dim)
        self.surface.fill((0, 0, 0, 0))
        for i, line in enumerate(lines):
            text_texture = self.font.render(line, True, (255, 255, 255))
            self.surface.blit(text_texture, (22, i * 22))
            self._render = False
        self.surface.set_alpha(220)

    def toggle(self):
        """Toggle on or off the render help"""
        self._render = not self._render

    def render(self, display):
        """Render help text method"""
        if self._render:
            display.blit(self.surface, self.pos)

def get_actor_display_name(actor, truncate=250):
    """Method to get actor display name"""
    name = ' '.join(actor.type_id.replace('_', '.').title().split('.')[1:])
    return (name[:truncate - 1] + u'\u2026') if len(name) > truncate else name


def draw_trajectory(planning_route):
    """
    Uses the live plotter to generate live feedback during the simulation
    The two feedback includes the trajectory feedback and
    the controller feedback (which includes the speed tracking).
    """
    lp_traj = lv.LivePlotter(tk_title="Trajectory Trace")
    lp_1d = lv.LivePlotter(tk_title="Controls Feedback")
    # Add 2D position / trajectory plot
    trajectory_fig = lp_traj.plot_new_dynamic_2d_figure(
        title='Vehicle Trajectory',
        figsize=(FIGSIZE_X_INCHES, FIGSIZE_Y_INCHES),
        edgecolor="black",
        rect=[PLOT_LEFT, PLOT_BOT, PLOT_WIDTH, PLOT_HEIGHT])
    trajectory_fig.set_invert_x_axis()  # Because UE4 uses left-handed
    # coordinate system the X
    # axis in the graph is flipped
    trajectory_fig.set_axis_equal()  # X-Y spacing should be equal in size

    # Add waypoint markers
    waypoints_x = list()
    waypoints_y = list()
    for w in planning_route:
        pos = w.transform.location
        waypoints_x.append(pos.x)
        waypoints_y.append(pos.y)
    trajectory_fig.add_graph("waypoints", window_size=len(waypoints_x),
                             x0=waypoints_x, y0=waypoints_y,
                             linestyle="-", marker="", color='g')
    # Add trajectory markers
    start_x, start_y = waypoints_x[0], waypoints_y[0]
    trajectory_fig.add_graph("trajectory", window_size=TOTAL_EPISODE_FRAMES,
                             x0=[start_x] * TOTAL_EPISODE_FRAMES,
                             y0=[start_y] * TOTAL_EPISODE_FRAMES,
                             color=[1, 0.5, 0])

    # Add starting position marker
    trajectory_fig.add_graph("start_pos", window_size=1,
                             x0=[start_x], y0=[start_y],
                             marker=11, color=[1, 0.5, 0],
                             markertext="Start", marker_text_offset=1)
    # Add end position marker
    trajectory_fig.add_graph("end_pos", window_size=1,
                             x0=[waypoints_x[-1]],
                             y0=[waypoints_y[-1]],
                             marker="D", color='r',
                             markertext="End", marker_text_offset=1)
    # Add car marker
    trajectory_fig.add_graph("car", window_size=1,
                             marker="s", color='b', markertext="Car",
                             marker_text_offset=1)

    # --- Add 1D speed profile updater pass: forward speed, throttle, steer  --- #
    forward_speed_fig = lp_1d.plot_new_dynamic_figure(title="Forward Speed (km/h)")
    forward_speed_fig.add_graph("forward_speed",
                                label="forward_speed",
                                window_size=TOTAL_EPISODE_FRAMES)
    # # each waypoints should have the reference signal ...
    # forward_speed_fig.add_graph("reference_signal",
    #                             label="reference_Signal",
    #                             window_size=TOTAL_EPISODE_FRAMES)

    # Add throttle signals graph
    throttle_fig = lp_1d.plot_new_dynamic_figure(title="Throttle (%)")
    throttle_fig.add_graph("throttle",
                           label="throttle",
                           window_size=TOTAL_EPISODE_FRAMES)
    # Add steering signals graph
    steer_fig = lp_1d.plot_new_dynamic_figure(title="Steer (Degree)")
    steer_fig.add_graph("steer",
                        label="steer",
                        window_size=TOTAL_EPISODE_FRAMES)
    return lp_traj, lp_1d, trajectory_fig, forward_speed_fig, throttle_fig, steer_fig