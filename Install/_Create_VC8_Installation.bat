echo off

if not exist Temp goto DelTempSkip
echo Removeing old temporary files...
rmdir /Q /S Temp
:DelTempSkip

if not exist TempExcl goto DelTempExclSkip
echo Removeing old temporary scripts...
rmdir /Q /S TempExcl
:DelTempExclSkip

call Rebuild_VC8.bat

call Prepare_ACF.bat

call Create_TechnicalDoc.bat

call Create_VC_LGPL.bat

call Compile_Setup.bat

echo Installation done

pause

echo Removeing temporary files...
rmdir /Q /S Temp
rmdir /Q /S TempExcl

