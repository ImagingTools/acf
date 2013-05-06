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
AppComments=Compiled using $CompilerName$, see www.imagingtools.de to find out more about ACF
AppCopyright=Copyright (C) 2007-2011 Witold Gantzke and Kirill Lepskiy
VersionInfoVersion=$AcfVersion:0$
ChangesEnvironment=yes

[Files]
Source: ..\Temp\*; DestDir: {app}; Flags: recursesubdirs; Components: acfComp
Source: {#EXTLIBDIR}\Microsoft\Redist\Debug$CompilerName$\*; DestDir: {app}\Bin\Debug$CompilerName$; Flags: recursesubdirs; Components: acfComp
Source: {#EXTLIBDIR}\Microsoft\Redist\Release$CompilerName$\*; DestDir: {app}\Bin\Release$CompilerName$; Flags: recursesubdirs; Components: acfComp

Source: {#QTDIR}\bin\Qt*4.dll; DestDir: {app}\ExtLib\Bin; Components: qtComp
Source: {#QTDIR}\bin\phonon4.dll; DestDir: {app}\ExtLib\Bin; Components: qtComp
Source: {#QTDIR}\bin\phonond4.dll; DestDir: {app}\ExtLib\Bin; Components: qtComp
Source: {#QTDIR}\plugins\imageformats\*.dll; DestDir: {app}\Bin\Release$CompilerName$\imageformats; Components: acfComp
Source: {#QTDIR}\plugins\iconengines\*.dll; DestDir: {app}\Bin\Release$CompilerName$\iconengines; Components: acfComp
Source: {#QTDIR}\plugins\imageformats\*.dll; DestDir: {app}\Bin\Debug$CompilerName$\imageformats; Components: acfComp
Source: {#QTDIR}\plugins\iconengines\*.dll; DestDir: {app}\Bin\Debug$CompilerName$\iconengines; Components: acfComp

[Languages]
Name: en; MessagesFile: compiler:Default.isl
Name: de; MessagesFile: compiler:Languages\German.isl
Name: pl; MessagesFile: compiler:Languages\Polish.isl

[CustomMessages]
en.MinimalType=Minimal installation
de.MinimalType=Minimale Installation
pl.MinimalType=Instalacja minimalna

en.FullType=Full installation
de.FullType=Komplete Installation
pl.FullType=Instalacja kompletna

en.CustomType=Custom installation
de.CustomType=Ausgewählte Komponenten
pl.CustomType=Wybrane komponenty

en.QtBinaries=Qt binaries
de.QtBinaries=Qt binäre Datei
pl.QtBinaries=Pliki binarne Qt

en.AddExtPath=Add external binaries path to the system PATH
de.AddExtPath=Pfad für externe Bibliotheken zum Systempfad hinzufügen
pl.AddExtPath=Dodaj ¶cie¿kê zewnetrznych bibliotek do ¶cie¿ki systemowej

en.AddAcfPath=Add ACF tools path to the system PATH
de.AddAcfPath=Pfad für ACF-Tools zum Systempfad hinzufügen
pl.AddAcfPath=Dodaj ¶cie¿kê narzêdzi ACF do ¶cie¿ki systemowej

[Types]
Name: minimalType; Description: {cm:MinimalType}
Name: fullType; Description: {cm:FullType}
Name: customType; Description: {cm:CustomType}; Flags: iscustom

[Components]
Name: acfComp; Description: ACF; Types: minimalType fullType customType
Name: qtComp; Description: {cm:QtBinaries}; Types: fullType customType

[Icons]
Name: "{group}\Compositor"; Filename: {app}\Bin\Release$CompilerName$\Compositor.exe; WorkingDir: {app}\Bin\Release$CompilerName$; Components: acfComp
Name: "{group}\AcfStarter"; Filename: {app}\Bin\Release$CompilerName$\AcfStarter.exe; WorkingDir: {app}\Bin\Release$CompilerName$; Components: acfComp
Name: "{group}\API Documentation"; Filename: {app}\Docs\TechnicalDoc\index.html; Components: acfComp

[Tasks]
Name: AddExtPath; Description: {cm:AddExtPath}; Flags: checkedonce; Components: qtComp
Name: AddAcfPath; Description: {cm:AddAcfPath}; Flags: checkedonce; Components: acfComp

[Registry]
Root: HKCU; Subkey: "Environment"; ValueType: string; ValueName: "Path"; ValueData: "{olddata};{app}\ExtLib\Bin"; Flags: createvalueifdoesntexist; Tasks: AddExtPath
Root: HKCU; Subkey: "Environment"; ValueType: string; ValueName: "Path"; ValueData: "{olddata};{app}\Bin\Release$CompilerName$"; Flags: createvalueifdoesntexist; Tasks: AddAcfPath
Root: HKCU; Subkey: "Environment"; ValueName: ACFDIR; ValueData: {app}; ValueType: string; Flags: uninsdeletevalue; Components: acfComp

Root: HKLM; Subkey: "SOFTWARE\\Classes\\.arx"; ValueType: string; ValueData: arx_auto_file; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\arx_auto_file"; ValueData: "ACF Registry"; ValueType: string; Flags: uninsdeletevalue; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\arx_auto_file"; ValueName: EditFlags; ValueData: 0; ValueType: dword; Flags: uninsdeletevalue; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\arx_auto_file"; ValueName: BrowserFlags; ValueData: 8; ValueType: dword; Flags: uninsdeletevalue; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\arx_auto_file\\shell\\open\\command"; ValueData: "{app}\\Bin\\Release$CompilerName$\\Acf.exe %22%251%22"; ValueType: string; Flags: uninsdeletevalue; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\arx_auto_file\\shell\\edit\\command"; ValueData: "{app}\\Bin\\Release$CompilerName$\\Compositor.exe %22%251%22"; ValueType: string; Flags: uninsdeletevalue; Components: acfComp

Root: HKLM; Subkey: "SOFTWARE\\Classes\\.xpc"; ValueType: string; ValueData: xpc_auto_file; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\xpc_auto_file"; ValueData: "ACF Registry"; ValueType: string; Flags: uninsdeletevalue; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\xpc_auto_file"; ValueName: EditFlags; ValueData: 0; ValueType: dword; Flags: uninsdeletevalue; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\xpc_auto_file"; ValueName: BrowserFlags; ValueData: 8; ValueType: dword; Flags: uninsdeletevalue; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\xpc_auto_file\\shell\\open\\command"; ValueData: "{app}\\Bin\\Release$CompilerName$\\AcfStarter.exe %22%251%22"; ValueType: string; Flags: uninsdeletevalue; Components: acfComp
Root: HKLM; Subkey: "SOFTWARE\\Classes\\xpc_auto_file\\shell\\edit\\command"; ValueData: "{app}\\Bin\\Release$CompilerName$\\Compositor.exe %22%251%22"; ValueType: string; Flags: uninsdeletevalue; Components: acfComp
