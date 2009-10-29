copy /Y %QTDIR%\lib\*.dll DebugVC8
copy /Y %QTDIR%\lib\*.dll.Manifest DebugVC8
xcopy /Y %QTDIR%\plugins\imageformats\*.* DebugVC8\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* DebugVC8\iconengines\*.*
