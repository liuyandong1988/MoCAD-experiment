def get_actor_display_name(actor, truncate=250):
    name = ' '.join(actor.type_id.replace('_', '.').title().split('.')[1:])
    return (name[:truncate - 1] + u'\u2026') if len(name) > truncate else name

FPS = 10.0
# Default parameters
args_lat_hw_dict = {
    'K_P': 0.75,
    'K_D': 0.02,
    'K_I': 0.4,
    'dt': 1.0 / FPS}
args_long_hw_dict = {
    'K_P': 0.37,
    'K_D': 0.024,
    'K_I': 0.032,
    'dt': 1.0 / FPS}
