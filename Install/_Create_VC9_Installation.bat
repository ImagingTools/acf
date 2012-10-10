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
set LICENSE_TYPE=LGPL
set LICENSE_DIR=Install/LGPL
set LICENSE_INSTALL_PATH=..\LGPL\License.txt

call Rebuild.bat

call CopyDlls.bat

call CreateScripts.bat

call Create_TechnicalDoc.bat

echo Copying files to Temp directory...
"../Bin/Debug%COMPILER_EXT%/Acf.exe" Create_%COMPILER_EXT%_LGPL.arx -config $(ACFSLNDIR)/Config/Core.xpc -input .. -output Temp

call Compile_Setup.bat

echo Installation done

pause

echo Removing temporary files...
rmdir /Q /S Temp
rmdir /Q /S TempExcl

