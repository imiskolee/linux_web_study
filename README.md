#web编程相关知识梳理
-----

###Intro
* 仅供学习研究使用，请勿用于生产。
* 这里的代码主要是web服务编程相关知识的梳理
* 编译环境为 Centos + gcc4.8 + cmake 2.8+


###目录结构

	|-timmer 定时器实现
		该目录会有以下实现
		1.基于单时间轮简单的计时器实现  sig_alarm 作为计时源
		2.基于多时间轮的复杂计时器实现  基于epoll的实现
		3.基于最小时间堆的计时器实现	基于epoll的hi先





