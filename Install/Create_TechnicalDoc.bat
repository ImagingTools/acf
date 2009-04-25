echo Creation of technical documentation...

if not exist Temp goto DelTempSkip
echo Removeing old temporary files...
rmdir /Q /S Temp
:DelTempSkip

echo Create doxyfile...
"../bin/DebugVC8/Acf.exe" Create_Doxyfile.arx -config ../Config/AcfCore.xpc

cd Temp

doxygen

cd ..

echo Removeing temporary files...
rmdir /Q /S Temp

echo Technical documentation created
