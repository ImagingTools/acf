echo off

set COMPILER_EXT=VC9
set LICENSE_TYPE=LGPL
set LICENSE_DIR=Install/LGPL
set LICENSE_INSTALL_PATH=../LGPL/License.txt
set ACF_CONFIG_FILE=$(ACFSLNDIR)/Config/Core.xpc

cd %~dp0\..

call Install\InternalParts\CreateTempDirs.bat

echo Compiling...
vcbuild /r %~dp0\..\Build\%COMPILER_EXT%\AcfAll.sln $All
vcbuild /r %~dp0\..\Docs\Tutorial\Build\%COMPILER_EXT%\TutorialAcf.sln $All
echo Compiling done

call Install\InternalParts\CopyDlls.bat

call Install\InternalParts\CreateScripts.bat

call Install\InternalParts\Create_TechnicalDoc.bat

call Install\InternalParts\CopyTempFiles.bat

call Install\InternalParts\Compile_Setup.bat

echo Installation done

call Install\InternalParts\RemoveTempDirs.bat
