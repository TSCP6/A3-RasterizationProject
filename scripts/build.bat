@echo off
echo Building A3-RasterizationProject...

REM 切换到项目根目录（脚本的上级目录）
cd /d "%~dp0\.."

REM 创建构建目录
if not exist build mkdir build
cd build

REM 生成项目
cmake .. -G "Visual Studio 17 2022" -A x64

REM 编译项目
cmake --build . --config Release

echo Build completed!
cd ..
exit