echo off

set LICENSE_TYPE=Public
set LICENSE_DIR=Install/LGPL
set LICENSE_INSTALL_PATH=../LGPL/License.txt
set ACF_CONFIG_FILE=$(ACFSLNDIR)/Config/AcfSlnConfig.awc

call %~dp0\..\Build\VC12\_CreateVCProjectsFromQMake.bat

cd %~dp0\..

set COMPILER_EXT=VC12
call Install\InternalParts\CreateTempDirs.bat

echo Compiling...
msbuild %~dp0\..\Build\%COMPILER_EXT%\AcfAll.sln /p:Configuration=Debug
msbuild %~dp0\..\Docs\Tutorial\Build\%COMPILER_EXT%\TutorialAcf.sln /p:Configuration=Debug
msbuild %~dp0\..\Build\%COMPILER_EXT%\AcfAll.sln /p:Configuration=Release
msbuild %~dp0\..\Docs\Tutorial\Build\%COMPILER_EXT%\TutorialAcf.sln /p:Configuration=Release
echo Compiling done

call Install\InternalParts\CreateScripts.bat

call Install\InternalParts\Create_TechnicalDoc.bat

call Install\InternalParts\CopyTempFiles.bat

echo Installation done
