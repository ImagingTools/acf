echo Creation of technical documentation...

del /q /s Docs\TechnicalDoc\*

cd Install\TempExcl

doxygen

cd ..\..

echo Technical documentation created
