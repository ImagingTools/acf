copy /Y %QTDIR%\bin\*.dll ..\..\Bin\ReleaseVC9
copy /Y %QTDIR%\bin\*.dll.Manifest ..\..\Bin\ReleaseVC9
xcopy /Y %QTDIR%\plugins\imageformats\*.* ..\..\Bin\ReleaseVC9\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* ..\..\Bin\ReleaseVC9\iconengines\*.*
