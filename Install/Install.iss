#define QTDIR GetEnv("QTDIR")
#define EXTLIBDIR GetEnv("EXTLIBDIR")

[Setup]
AppVerName=$AcfProductName$ $AcfVersion:0$
AppVersion=$AcfVersion:0$
AppPublisher=Witold Gantzke & Kirill Lepskiy
AppPublisherURL=http://www.ilena.org/
AppName=ACF
OutputBaseFilename=Setup_$AcfProductName$_$LICENSE_TYPE$_v$AcfVersion:0$_$CompilerName$

DefaultDirName={pf}\ImagingTools\$AcfProductName$_v$AcfVersion:0$
WizardImageFile=..\Resources\WizardImageFile.bmp
WizardSmallImageFile=..\Resources\WizardSmallImageFile.bmp
SetupIconFile=..\..\Docs\Images\Icons\Logo.ico
AlwaysShowDirOnReadyPage=True
DefaultGroupName=ImagingTools\ACF
DisableProgramGroupPage=True
UninstallDisplayIcon=..\..\Docs\Images\Icons\Logo.ico
UninstallDisplayName=$AcfProductName$ $AcfVersion:0$
OutputDir=..\Setup
LicenseFile=$LICENSE_INSTALL_PATH$
VersionInfoDescription=$AcfProductName$ v. $AcfVersion:0$ ($AcfRawVersion:0$) by $AcfCompanyName$
AppComments=Compiled using $CompilerName$, see www.ilena.org to find out more about ACF
AppCopyright=Copyright (C) 2007-2015 Witold Gantzke and Kirill Lepskiy
VersionInfoVersion=$AcfVersion:0$
ChangesEnvironment=yes

[Files]
Source: ..\Temp\Bin\*; DestDir: {app}\Bin; Flags: recursesubdirs; Components: binaryComp
Source: ..\Temp\Lib\*; DestDir: {app}\Lib; Flags: recursesubdirs; Components: binaryComp

Source: ..\Temp\Config\*; DestDir: {app}\Config; Flags: recursesubdirs; Components: standardComp
Source: ..\Temp\Partitura\*; DestDir: {app}\Partitura; Flags: recursesubdirs; Components: standardComp
Source: ..\Temp\*; DestDir: {app}; Components: standardComp
Source: ..\Temp\Docs\*.txt; DestDir: {app}\Docs; Components: standardComp
Source: ..\Temp\Include\*; Excludes: "*.cpp,*.qrc,*.ui,\Resources\*"; DestDir: {app}\Include\*; Flags: recursesubdirs; Components: standardComp
Source: ..\Temp\Impl\*; Excludes: "*.cpp"; DestDir: {app}\Impl\*; Flags: recursesubdirs; Components: standardComp

Source: ..\Temp\Docs\*; DestDir: {app}\Docs; Flags: recursesubdirs; Components: docuComp

Source: ..\Temp\Include\*; DestDir: {app}\Include\*; Flags: recursesubdirs; Components: sourceComp
Source: ..\Temp\Impl\*; DestDir: {app}\Impl\*; Flags: recursesubdirs; Components: sourceComp

Source: {#QTDIR}\bin\icu*.dll; DestDir: {app}\Bin\Debug$CompilerName$; Components: qtBinaryComp
Source: {#QTDIR}\bin\icu*.dll; DestDir: {app}\Bin\Release$CompilerName$; Components: qtBinaryComp
Source: {#QTDIR}\bin\Qt?Core.dll; DestDir: {app}\Bin\Debug$CompilerName$; Components: qtBinaryComp
Source: {#QTDIR}\bin\Qt?Core.dll; DestDir: {app}\Bin\Release$CompilerName$; Components: qtBinaryComp
Source: {#QTDIR}\bin\Qt?Gui.dll; DestDir: {app}\Bin\Debug$CompilerName$; Components: qtBinaryComp
Source: {#QTDIR}\bin\Qt?Gui.dll; DestDir: {app}\Bin\Release$CompilerName$; Components: qtBinaryComp
Source: {#QTDIR}\bin\Qt?Widgets.dll; DestDir: {app}\Bin\Debug$CompilerName$; Components: qtBinaryComp
Source: {#QTDIR}\bin\Qt?Widgets.dll; DestDir: {app}\Bin\Release$CompilerName$; Components: qtBinaryComp
Source: {#QTDIR}\bin\Qt?Xml.dll; DestDir: {app}\Bin\Debug$CompilerName$; Components: qtBinaryComp
Source: {#QTDIR}\bin\Qt?Xml.dll; DestDir: {app}\Bin\Release$CompilerName$; Components: qtBinaryComp
Source: {#QTDIR}\bin\Qt?Svg.dll; DestDir: {app}\Bin\Debug$CompilerName$; Components: qtBinaryComp
Source: {#QTDIR}\bin\Qt?Svg.dll; DestDir: {app}\Bin\Release$CompilerName$; Components: qtBinaryComp
Source: {#QTDIR}\plugins\imageformats\qsvg.dll; DestDir: {app}\Bin\Debug$CompilerName$\imageformats; Components: qtBinaryComp
Source: {#QTDIR}\plugins\imageformats\qsvg.dll; DestDir: {app}\Bin\Release$CompilerName$\imageformats; Components: qtBinaryComp
Source: {#QTDIR}\plugins\iconengines\qsvgicon.dll; DestDir: {app}\Bin\Debug$CompilerName$\iconengines; Components: qtBinaryComp
Source: {#QTDIR}\plugins\iconengines\qsvgicon.dll; DestDir: {app}\Bin\Release$CompilerName$\iconengines; Components: qtBinaryComp

[Languages]
Name: en; MessagesFile: compiler:Default.isl
Name: de; MessagesFile: compiler:Languages\German.isl
Name: pl; MessagesFile: compiler:Languages\Polish.isl

[CustomMessages]
en.StandardType=Typical installation
de.StandardType=Standardinstallation
pl.StandardType=Instalacja standardowa

en.FullType=Full installation
de.FullType=Komplete Installation
pl.FullType=Instalacja kompletna

en.CustomType=Custom installation
de.CustomType=Ausgewählte Komponenten
pl.CustomType=Wybrane komponenty

en.Binaries=Compilated binaries
de.Binaries=Kompilierte binäre Dateien
pl.Binaries=Skompilowane pliki binarne

en.Documentation=Documentation
de.Documentation=Dokumentation
pl.Documentation=Dokumentacja

en.Sources=Source files
de.Sources=Source-Dateien
pl.Sources=Pliki zródlowe

en.QtBinaries=Qt binaries
de.QtBinaries=Qt binäre Dateien
pl.QtBinaries=Pliki binarne Qt

en.AddExtPath=Set ACFDIR to environmental variables
de.AddExtPath=Die Umgebungsvariable ACFDIR setzten
pl.AddExtPath=Ustaw ACFDIR w zmiennych systemowych

[Types]
Name: standardType; Description: {cm:StandardType}
Name: fullType; Description: {cm:FullType}
Name: customType; Description: {cm:CustomType}; Flags: iscustom

[Components]
Name: standardComp; Description: ACF; Types: standardType fullType customType
Name: binaryComp; Description: {cm:Binaries}; Types: standardType fullType customType
Name: docuComp; Description: {cm:Documentation}; Types: standardType fullType customType
Name: sourceComp; Description: {cm:Sources}; Types: fullType
Name: qtBinaryComp; Description: {cm:QtBinaries}; Types: standardType fullType customType

[Tasks]
Name: AddExtPath; Description: {cm:AddExtPath}; Flags: checkedonce; Components: standardComp

[Registry]
Root: HKCU; Subkey: "Environment"; ValueName: ACFDIR; ValueData: {app}; ValueType: string; Flags: uninsdeletevalue; Tasks: AddExtPath
