# StudentManagementSystem

学生信息管理系统，涉及了TCP、线程、信号量等方面

# 一、服务端

## （一）简介

学生信息管理系统的服务端

## （二）部署

### 1.准备工作

使用vscode连接MySQL数据库，进行合理的配置

### 2.部署截图

![部署截图](https://github.com/imrewang/StudentManagementSystem/blob/main/screenshot/%E6%9C%8D%E5%8A%A1%E7%AB%AF%E9%83%A8%E7%BD%B2%E6%88%AA%E5%9B%BE.png?raw=true)

### 3.建表语句

建表语句请查看数据库相关.cpp

## （三）运行截图

![运行截图](https://github.com/imrewang/StudentManagementSystem/blob/main/screenshot/%E6%9C%8D%E5%8A%A1%E7%AB%AF%E8%BF%90%E8%A1%8C%E6%88%AA%E5%9B%BE.png?raw=true)

## （四）总结

### 1.临界区问题

[临界区问题](https://www.cnblogs.com/imreW/p/17157685.html)

### 2.信号量问题

[信号量问题](https://www.cnblogs.com/imreW/p/17156531.html)

### 3.#线程池问题

手写完成线程池

线程的创建和销毁的开销是巨大的，而通过线程池的重用大大减少了这些不必要的开销，当然既然少了这么多消费内存的开销，其线程执行速度也是突飞猛进的提升。

控制线程池的并发数可以有效的避免大量的线程池争夺CPU资源而造成堵塞。

线程池可以提供定时、定期、单线程、并发数控制等功能。

### 4.多线程冲突问题

这是典型的#生产者消费者模型

通过创建多个线程，在开始时信号量为0

## （五）改进方向

### 1.使用了任务队列

使用任务队列，先进先出，十分方便。但是是不是可以有高级用户和普通用户之分（好罪恶），可能可以使用不同的队列实现。

# 二、用户端

## （一）简介

学生信息系统的用户端

## （二）部署

### 1.环境

操作系统：windows

环境：配置MFC

### 2.部署截图


## （三）运行截图


## （四）总结

### 1.TCP网络的连接问题

服务端：

网络环境初始化：WSAStartup

创建服务器套接字：socket

绑定本机IP和端口：bind

监听客户端：listen

等待客户端连接：accept

发送消息：send

接收消息：recv

关闭socket：closesocket

清除网络环境：WSACleanup

客户端：

初始化网络环境：WSAStartup

创建套接字：socket

连接服务器：connect

发送数据：send

接收数据：recv

清理网络环境：WSACleanup

### 2.缓存问题

客户端本地使用结构存储已经查询的数据，减少了向服务端的访问

## （五）改进方向

### 1.增加功能

功能较为单一，可以增加其他功能






