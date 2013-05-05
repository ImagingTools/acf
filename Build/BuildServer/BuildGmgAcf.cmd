echo Update GMG ACF repository
svn update C:\Temp\GmgAcf --username acf --password acf --non-interactive --trust-server-cert --no-auth-cache
echo Update of GmgAcf repository finished

echo Remove old GmgAcf files
cd C:\Temp\GmgAcf\Acf
del /s /q /a-h *
cd C:\Temp\GmgAcf\AcfSln
del /s /q /a-h *
cd C:\Temp\GmgAcf\Iacf
del /s /q /a-h *

echo Create installation files

echo Create Acf
cd C:\Temp\Acf\Install
call _Create_VC9_OpenSource.bat
xcopy Temp C:\Temp\GmgAcf\Acf /E /Y

echo Create Acf-Solutions
cd C:\Temp\AcfSln\Install
call _Create_VC9_OpenSource.bat
xcopy Temp C:\Temp\GmgAcf\AcfSln /E /Y

echo Create IACF
cd C:\Temp\Iacf\Install
call _Create_VC9_OpenSource.bat
xcopy Temp C:\Temp\GmgAcf\Iacf /E /Y

cd C:\Temp\GmgAcf

echo Delete removed files
for /f "usebackq tokens=2*" %%i in (`svn status ^| findstr /r "^\!"`) do (

    svn delete "%%i %%j"
)

svn update C:\Temp\AcfPublic --username acf --password acf --non-interactive --trust-server-cert --no-auth-cache
svn export C:\Temp\AcfPublic\3rdParty C:\Temp\GmgAcf\3rdParty --force --username acf --password acf --non-interactive --trust-server-cert --no-auth-cache


echo Add new files
for /f "usebackq tokens=2*" %%i in (`svn status ^| findstr /r "^\?"`) do (

    svn add "%%i %%j"
)


echo Commit changes to the repository
svn commit C:\Temp\GmgAcf --username acf --password acf --non-interactive --trust-server-cert --no-auth-cache -F C:\Temp\Iacf\Install\TempExcl\Commit.log

