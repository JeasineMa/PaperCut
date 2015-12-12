##全向小车底层程序使用说明v0.1 20151118

TODO:
1.增加针对Windows/Linux/OS X平台的编译说明
2.增加下载说明
3.增加外设连接说明

###适用硬件平台：
	STM32F407xx

###目录说明：
/README.md 即本文件，描述了全向小车底层程序的使用
/Libraries/* STM32的标准函数库文件，链接脚本
* /Project/inc/* 用户包含文件*
* /Project/src/* 用户源文件*
/Project/TrueSTUDIO/* makefile，STM32启动文件
/Utilities/* 额外的程序组件

###文件说明：
位于/Project/inc/* , /Project/src/*中的各文件均已经将文件说明附于文件头部。
其中：
macros.c/macros.h 中声明和实现了针对用户的操作接口，请务必仔细阅读。
其余文件为底层函数的实现，不必修改。

###使用说明：
1.按照/Project/inc/All_Include.h中的Pins and Peripherals Assignment连接电机，编码器和蓝牙串口
2.在PC/Mac上搭建编译环境
3.修改Project/inc/macros.h和Project/src/macros.c中的函数实现，添加用户自己的函数
4.修改Project/src/main.c中的主函数入口，实现用户自己的程序逻辑
5.通过下载器下载程序
6.调试程序