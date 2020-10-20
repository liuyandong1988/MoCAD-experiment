import csv
import cv2
import random
import numpy as np
import matplotlib.pyplot as plt
# from sklearn.utils import shuffle

#load lines in csv file to read the images
def load_csv(file):
    lines = []
    with open(file) as csvfile:
        reader = csv.reader(csvfile)
        for line in reader:
            if 0.20<=float(line[2])<=0.80:
                lines.append(line)
    return lines



def load_images(lines_path, image_path):
    """
    Load the images
    """

    path = image_path
    lines = lines_path
    images = []
    angles = []

    for line in lines:
        source_path = line[0]
        n = 8
        if float(line[1]) == 0.0 and random.randint(0, n) != 0:
            continue  # get 1/n steer = 0.0 data
        filename = source_path.split('/')[-1]
        current_path = path + filename
        image = cv2.imread(current_path)
        # rgb-image to gray
        gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        gray_image = gray_image.reshape(image.shape[0], image.shape[1], 1)
        images.append(gray_image)
        angle = float(line[1])
        angles.append(angle)
    # network input
    X_train = np.array(images)
    y_train = np.array(angles)
    return X_train, y_train



# Check and show the train, validation data steering feature
def show_steering(y_train, y_valid):
    '''take train and validation data label-steering and visualize a histogram.
    input: y_train : train set label,
           y_valid : validation set label,
    output: Histogram of labels'''

    max_degree = 25
    degree_per_steering = 10
    n_classes = max_degree * degree_per_steering
    fig, axes = plt.subplots(2, 1, figsize=(8, 8))
    plt.subplots_adjust(left=0, right=0.95, top=0.9, bottom=0.25)
    ax0, ax1 = axes.flatten()

    ax0.hist(y_train, bins=n_classes, histtype='bar', color='blue', rwidth=0.6, label='train')
    ax0.set_title('Number of Original')
    ax0.set_xlabel('Steering Angle')
    ax0.set_ylabel('Total Image')

    # for valid data
    ax1.hist(y_valid, bins=n_classes, histtype='bar', color='red', rwidth=0.6, label='valid')
    ax1.set_title('****')
    ax1.set_xlabel('Steering Angle')
    ax1.set_ylabel('Total Image')

    fig.tight_layout()
    plt.show()

