# ---- training params --- #
# training episode
EPISODES = 3000
# record show:10 episode average
AVERAGE_RATE = 10

# --- model params --- #
REPLAY_MEMORY_SIZE = 10000
UPDATE_TARGET_EVERY = 10000
DISCOUNT = 0.99
INIT_EPSILON = 1
## 0.99999 --> 500_000 = 0.006    1_000_000
## 0.99993 --> 50_000 = 0.03 100_000
## 3000 episode, 0.9985 ** 2500 = 0.0234
EPSILON_DECAY = 0.9985
MIN_EPSILON = 0.03
BATCH_SIZE = 128

# --- show parmas --- #
# show image size
IMG_WIDTH = 1280
IMG_HEIGHT = 720
# input height and width
INPUT_WIDTH = 256
INPUT_HEIGHT = 128
# running during time
SECONDS_PER_EPISODE = 60
# show image or not
SHOW_PREVIEW = False

