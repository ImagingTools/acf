To create installation file you need to set system path for following tools:

vcbuild	-	installed with VisualStudio 2005 in {VCInstallationPath}\VC\vcpackages
doxygen -	to download
moc	-	typically to find in $(QTDIR)
uic	-	typically to find in $(QTDIR)
rcc	-	typically to find in $(QTDIR)

Additionally installation of Qt is needed and QTDIR path must be set.
All environment variables from ExtLib must be registered using RegisterEnvVariables.js or RegisterEnvVariablesLocal.js.

Running of installation script:

_Create_VC8_Installation.bat

