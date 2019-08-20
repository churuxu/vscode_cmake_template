# vscode_cmake_template

vscode cmake 项目工程模板


支持平台
--------
- PC (Window Linux Mac)
- Android
- STM32 


项目工程简介
-----------
- 当前目录和子目录下所有c cc cpp文件作为源码
- 当前目录和子目录下所有.h文件所在目录作为包含目录
- 编译生成可执行程序，名称为当前目录名
- 当前目录和子目录下的所有文件名带test的源码作为单元测试源码
- 如果有单元测试源码，会额外编译生成一个单元测试程序
- 代码中通过UNITTEST宏区分是单元测试还是主程序
- 自动引用子目录下的所有CMakeLists.txt模块
- 此项目里包含子模块示例，支持编译远程代码库
- 支持多平台，在config目录建立不同平台配置，通过node configure.js命令选择平台



所需软件
-----------
- ninja
  - 添加到PATH环境变量
- cmake
  - bin目录添加到PATH环境变量
- gcc (Windows需要mingw)(ARM单片机需要arm-none-eabi-gcc)
  - bin目录添加到PATH环境变量
- node 
  - 添加到PATH环境变量
- unzip\tar (可选,远程代码模块需要)
  - 添加到PATH环境变量
- vscode  
  - 安装插件：C/C++、Catch2 and Google Test Explorer
- jlink (可选,单片机烧写需要)
  - 添加到PATH环境变量
- ndk (Android需要)
  - 新建NDK_HOME环境变量指向ndk安装目录
  
VSCode Task
------------
- Configure 选择目标编译平台
- Build 编译
- Clean 清理
- Flash 烧写程序到单片机




