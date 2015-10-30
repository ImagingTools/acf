echo off

set LICENSE_TYPE=LGPL
set LICENSE_DIR=Install/LGPL
set LICENSE_INSTALL_PATH=../LGPL/License.txt
set ACF_CONFIG_FILE=$(ACFSLNDIR)/Config/AcfSlnConfig.awc

call %~dp0\..\Build\VC10\_CreateVCProjectsFromQMake.bat
call %~dp0\..\Build\VC12\_CreateVCProjectsFromQMake.bat

cd %~dp0\..

set COMPILER_EXT=VC9
call Install\InternalParts\CreateTempDirs.bat

echo Compiling...
vcbuild /r %~dp0\..\Build\%COMPILER_EXT%\AcfAll.sln $All
vcbuild /r %~dp0\..\Docs\Tutorial\Build\%COMPILER_EXT%\TutorialAcf.sln $All
echo Compiling done

call Install\InternalParts\CreateScripts.bat

call Install\InternalParts\Create_TechnicalDoc.bat

call Install\InternalParts\CopyTempFiles.bat

call Install\InternalParts\Compile_Setup.bat

echo Installation done

call Install\InternalParts\RemoveTempDirs.bat
