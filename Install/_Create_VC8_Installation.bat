echo off

set COMPILER_EXT=VC8
set LICENSE_TYPE=LGPL
set LICENSE_DIR=Install/LGPL
set LICENSE_INSTALL_PATH=../LGPL/License.txt

cd ..

call Install\InternalParts\CreateTempDirs.bat

call Install\InternalParts\Rebuild.bat

call Install\InternalParts\CopyDlls.bat

call Install\InternalParts\CreateScripts.bat

call Install\InternalParts\Create_TechnicalDoc.bat

call Install\InternalParts\CopyTempFiles.bat

call Install\InternalParts\Compile_Setup.bat

echo Installation done

pause

call Install\InternalParts\RemoveTempDirs.bat

