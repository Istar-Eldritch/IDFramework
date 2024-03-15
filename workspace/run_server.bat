set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"
set "DAYZS_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZServer\"

if not exist "%SCRIPT_DIR%\mission" (
    mkdir "%SCRIPT_DIR%\mission"
    xcopy /s "%DAYZS_PATH%\mpmissions\dayzOffline.chernarusplus" "%SCRIPT_DIR%\mission\dayzOffline.chernarusplus"
)

start /D "%DAYZ_PATH%" DayZDiag_x64.exe "-mission=%SCRIPT_DIR%\mission\dayzOffline.chernarusplus" "-mod=P:\Mods\@CF;P:\Mods\@Community-Online-Tools;P:\Mods\@IDFramework" -server -filePatching "-profiles=%SCRIPT_DIR%\profiles" "-config=%SCRIPT_DIR%\serverDZ.cfg"
