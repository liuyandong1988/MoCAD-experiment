# README Carla Behavior clone

## 0. Requirement
（1）UE4 == 4.24
（2）carla == 0.9.9
（3）opencv == 4.2.0
（4）pygame == 1.9.6
（5）pytorch == 1.5.0



## 1. 概述
使用UE4游戏引擎加载Carla工程,使用自己设计的地图运行数据采集程
序。收集数据(rgb-图像————方向盘对应的旋转角度)。设计策略网
略,使用收集到的数据训练网络(监督学习)。将策略网略应用到Carla车
辆控制。

## 2. 收集数据 
collect_data文件夹
(1)自动驾驶模式,收集数据。车辆按waypoint路径行驶,观察状态有限;
(2)人工控制模式,为了降低训练的复杂度,给定恒定的速度,只控制方向,收集(图
像——方向盘角度)注意:调整camera的位置和照射角度,好的图像信息有助于策略网络
的训练。
(3)处理收集到的数据,Carla server端的帧数和client端不一样,传感器采集到的数据要
大于收到的控制信息,要过滤下图片。Client 端帧数的设置跟计算机的性能有关(我的机器
最大帧数是每秒15张图， server和client端都设置成15fps)
(4)收集到的图片要resize,to gray,normalization后用于网络训练;

## 3. 网络训练
train_model文件夹
使用CNN+BATCH normalization+Fully connected网络结构，注意learning rate的设置。
max_iteration = 100000
batch_size = 64
learning_rate = 0.001
（换成RNN呢）


## 4. control by model
使用训练好的model驾驶carla vehicle


## 注意问题
### 1.工程
（1） Carla server端和client端帧数的设置与关系，涉及到pygame中clock函数；
（2）waypoint的生成和使用，绘制道路的waypoints；
（3）steer训练过程按比例放大，提高灵敏度；

### 2.理论
（1）数据为什么要scaling（normalization，standard）和网络层中BATCH normalization；
（2）累计误差，其他的imitation learning的方法。应用到multi-agent上；

### 3. 扩展工作
（1）pygame修饰显示窗口；
（2）使用的CNN网络换成rnn网络效果会提升吗？
（3）但任务回归steer，将模型扩展到多任务回归 steer，brake， throttle；
（4）应用imitation中policy learning的方法，
https://medium.com/@SmartLabAI/a-brief-overview-of-imitation-learning-8a8a75c44a9c

