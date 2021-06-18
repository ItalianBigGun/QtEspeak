# QtEspeak

#### 介绍
基于Qt和espeak实现的一个TTS应用。

#### 软件架构
软件架构说明


#### 安装教程



#### 使用说明

- 建议使用Qt5.9.8的mingW32位编译器。现成库下载路径：
https://gitee.com/boyboynextdoor/maybe-used-lib espeakAndportaudio-mingw53_32.rar<br/>
- 将espeak-data _文件夹位置_ 放入到环境变量名为ESPEAK_DATA_PATH中，下载地址：
https://gitee.com/boyboynextdoor/maybe-used-tools espeak-data_win.zip <br/>
- 如果上一步操作之后，仍然出现espeak-data找不到的情况。解决办法有：
    1. 尝试安装espeak的window安装包。下载地址：
https://gitee.com/boyboynextdoor/maybe-used-tools setup_espeak-1.48.04.exe
    2. 在espeak_Initialize函数的第三个参数填写espeak-data所在目录路径。
#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request

