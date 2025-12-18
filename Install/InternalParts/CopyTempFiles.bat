echo off

echo Copying files to Temp directory...
"%~dp0\..\..\Bin\Release_%COMPILER_EXT%\Acf.exe" Install/InternalParts/Create_%LICENSE_TYPE%.acc -config %ACF_CONFIG_FILE% -input . -output Install/Temp
