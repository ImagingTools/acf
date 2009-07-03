echo off

if not exist Temp goto DelTempSkip
echo Removing old temporary files...
rmdir /Q /S Temp
:DelTempSkip

if not exist TempExcl goto DelTempExclSkip
echo Removing old temporary scripts...
rmdir /Q /S TempExcl
:DelTempExclSkip

call Rebuild_VC8.bat

call Prepare_ACF.bat

call Create_TechnicalDoc.bat

call Create_VC_LGPL.bat

call Compile_Setup.bat

echo Installation done

pause

echo Removing temporary files...
rmdir /Q /S Temp
rmdir /Q /S TempExcl

