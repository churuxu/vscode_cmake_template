# vscode_cmake_template

vscode cmake 项目工程模板

项目工程简介
-----------
- 当前目录和子目录下所有c cpp文件作为源码
- 当前目录和子目录下所有.h文件所在目录作为包含目录
- 编译生成可执行程序，名称为当前目录名


所需软件
-----------
- ninja
  - 添加到PATH环境变量
- cmake
  - bin目录添加到PATH环境变量
- gcc (windows下使用mingw)
  - bin目录添加到PATH环境变量
  - 新建环境变量 GDP_PATH=gdb程序完整路径
- node (可选)
  - 添加到PATH环境变量
- vscode  

VSCode Task
------------
- build 编译
- clean 清理
- c_cpp_properties-generate 生成c_cpp_properties.json



