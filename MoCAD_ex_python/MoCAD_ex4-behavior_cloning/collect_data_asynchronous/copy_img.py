# read the driving log
import csv
import os
import shutil
import random

def load_csv(file):
    lines = []
    with open(file) as csvfile:
        reader = csv.reader(csvfile)
        for line in reader:
            if 0.20<=float(line[2])<=0.80:
                lines.append(line)  # keep the velocity
    return lines



def walk_dir(datas, a_folder, b_folder):
    # read the name
    for data in datas:
        image_name = data[0]
        image_file = os.path.join(a_folder, image_name)
        copy_to_file = os.path.join(b_folder, image_name)
        print(image_file)
        shutil.copy2(image_file, copy_to_file)




# load the data
data_csv = load_csv('driving_log.csv')
image_folder = './IMG'
copy_folder = './filter_img'
# print(len(data_csv))
# read the name
walk_dir(data_csv, image_folder, copy_folder)
print('Finish !!')


