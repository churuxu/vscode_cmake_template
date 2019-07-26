# vscode_cmake_template

vscode cmake 项目工程模板


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


所需软件
-----------
- ninja
  - 添加到PATH环境变量
- cmake
  - bin目录添加到PATH环境变量
- gcc (windows下使用mingw)
  - bin目录添加到PATH环境变量
- node (可选)
  - 添加到PATH环境变量
- unzip\tar(可选)
  - 添加到PATH环境变量
- vscode  
  - 安装插件：C/C++、Catch2 and Google Test Explorer

  
VSCode Task
------------
- build 编译
- clean 清理
- test 运行单元测试
- c_cpp_properties-generate 生成c_cpp_properties.json




