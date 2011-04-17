# static libs for the GCC compiler
ACFQT_LIBS = -liqtmm -liqtproc -liqtdoc -liqt2d -liqtprm -liqtgui -liqt

# Dependent libs for visual studio compiler
win32-msvc*{
	ACFQT_DEPENDENCIES = iqtmm.lib iqtproc.lib iqt2d.lib iqtdoc.lib iqtprm.lib iqtgui.lib iqt.lib
}
# Dependent libs for the GCC compiler
else{
	ACFQT_DEPENDENCIES = libiqtmm.a libiqtproc.a libiqt2d.a libiqtdoc.a libiqtprm.a libiqtgui.a libiqt.a
}
