echo Creation of technical documentation...

echo Create doxyfile...
"../bin/DebugVC8/Acf.exe" Create_Doxyfile.arx -config ../Config/AcfCore.xpc

cd TempExcl

doxygen

cd ..

echo Technical documentation created
