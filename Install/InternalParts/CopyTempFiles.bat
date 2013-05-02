echo off

echo Copying files to Temp directory...
"%ACFDIR%/Bin/Debug%COMPILER_EXT%/Acf.exe" Install/InternalParts/Create_%COMPILER_EXT%_%LICENSE_TYPE%.arx -config $(ACFSLNDIR)/Config/Core.xpc -input . -output Install/Temp
