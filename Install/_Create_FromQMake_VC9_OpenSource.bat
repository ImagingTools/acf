echo off

if not exist Temp goto DelTempSkip
echo Removing old temporary files...
rmdir /Q /S Temp
:DelTempSkip

if not exist TempExcl goto DelTempExclSkip
echo Removing old temporary scripts...
rmdir /Q /S TempExcl
:DelTempExclSkip

mkdir Temp
mkdir TempExcl

set COMPILER_EXT=VC9

call Create_TechnicalDoc.bat

echo Copying files to Temp directory...
"../Bin/Debug%COMPILER_EXT%/Acf.exe" Create_FromQMake_VC9_OpenSource.arx -config $(ACFSLNDIR)/Config/Core.xpc

call MakeVCProjects.bat

