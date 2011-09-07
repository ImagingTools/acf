echo Creation of technical documentation...

del /q /s ..\Docs\TechnicalDoc\*

echo Create doxyfile...
"..\bin\Debug%COMPILER_EXT%\Acf.exe" Create_Doxyfile.arx -config ../Config/Core.xpc

cd TempExcl

doxygen

cd ..

echo Technical documentation created
