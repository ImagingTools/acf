echo Update ACfPublic repository
svn update D:\Temp\AcfPublic --username acf --password acf --non-interactive --trust-server-cert --no-auth-cache
echo Update of ACfPublic repository finished

cd %~dp0

echo Remove old ACfPublic files
cd D:\Temp\AcfPublic\Acf
del /s /q /a-h *
cd D:\Temp\AcfPublic\\AcfSln
del /s /q /a-h *
cd D:\Temp\AcfPublic\\Iacf
del /s /q /a-h *

echo Create installation files

echo Create Acf
cd D:\Temp\Qt6\Acf\Install
call _Create_VC17_OpenSource.bat
xcopy D:\Temp\Qt6\Acf\Install\Temp D:\Temp\AcfPublic\Acf /E /Y

echo Create Acf-Solutions
cd D:\Temp\Qt6\AcfSln\Install
call _Create_VC17_OpenSource.bat
xcopy D:\Temp\Qt6\AcfSln\Install\Temp D:\Temp\AcfPublic\AcfSln /E /Y

echo Create IACF
cd D:\Temp\Qt6\IAcf\Install
call _Create_VC17_OpenSource.bat
xcopy D:\Temp\Qt6\IAcf\Install\Temp D:\Temp\AcfPublic\Iacf /E /Y

cd D:\Temp\AcfPublic

echo Delete removed files
for /f "usebackq tokens=2*" %%i in (`svn status ^| findstr /r "^\!"`) do (
    svn delete "%%i %%j"
)

echo Add new files
for /f "usebackq tokens=2*" %%i in (`svn status ^| findstr /r "^\?"`) do (
    svn add "%%i %%j"
)

echo Commit changes to the repository
svn commit D:\Temp\AcfPublic --username acf --password acf --non-interactive --trust-server-cert --no-auth-cache -F D:\Temp\Qt6\IAcf\Install\TempExcl\Commit.log   