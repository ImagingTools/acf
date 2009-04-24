echo off

if not exist Temp goto DelTempSkip
echo Removeing old temporary files...
rmdir /Q /S Temp
:DelTempSkip

if not exist TempExcl goto DelTempExclSkip
echo Removeing old temporary scripts...
rmdir /Q /S TempExcl
:DelTempExclSkip

echo Copying files to Temp directory...
"../bin/DebugVC8/Acf.exe" Create_VC_LGPL.arx -config ../Config/AcfCore.xpc

echo Createing installation file...
iscc.exe /Q TempExcl\acf.iss

echo Removeing temporary files...
rmdir /Q /S Temp
rmdir /Q /S TempExcl

echo Installation done
