echo Creation of technical documentation...

del /q /s ..\Docs\TechnicalDoc\*

cd TempExcl

doxygen

cd ..

echo Technical documentation created
