@echo off
echo Running A3-RasterizationProject...

REM 切换到项目根目录
cd /d "%~dp0\.."

set "BIN_DIR=build\Release"

if not exist "%BIN_DIR%" (
    echo %BIN_DIR% not found. Please run build.bat first.
    exit /b 1
)

REM 1) 先运行所有前置任务 pretest*.exe
if exist "%BIN_DIR%\pretest*.exe" (
    for %%f in ("%BIN_DIR%\pretest*.exe") do (
        if exist "%%~f" (
            echo Running %%~nxf...
            "%%~f"
            echo.
        )
    )
) else (
    echo No pretest executables found.
)

if exist "%BIN_DIR%\main_program.exe" (
    echo Running main_program.exe...
    "%BIN_DIR%\main_program.exe"
    goto :EOF
)

echo No suitable executable found in %BIN_DIR%.
exit /b 1