@echo off
echo Running A3-RasterizationProject...

REM 切换到项目根目录（脚本的上级目录）
cd /d "%~dp0\.."

REM 如果存在 build\Release 目录，遍历并运行所有 pretest*.exe
if exist build\Release (
    set FOUND=0
    for %%f in (build\Release\pretest*.exe) do (
        if exist "%%f" (
            set FOUND=1
            echo Running %%~nxf...
            "%%~f"
            echo.
        )
    )
    if "%FOUND%"=="0" (
        echo No pretest executables found in build\Release. Please run build.bat first.
    )
) else (
    echo build\Release directory not found. Please run build.bat first.
)

exit