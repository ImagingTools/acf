copy /Y %QTDIR%\bin.debug\*.dll ..\..\Bin\DebugVC9
copy /Y %QTDIR%\bin.debug\*.dll.Manifest ..\..\Bin\DebugVC9
xcopy /Y %QTDIR%\plugins\imageformats\*.* ..\..\Bin\DebugVC9\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* ..\..\Bin\DebugVC9\iconengines\*.*
