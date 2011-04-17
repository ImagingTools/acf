# static libs for the GCC compiler
ACFSTD_LIBS = -libase -licomp -li2d -liproc -lidoc -lisec -liprm -liimg -licmm -limath -lisys -limod -liser -listd

# Dependent libs for visual studio compiler
win32-msvc*{
	ACFSTD_DEPENDENCIES = i2d.lib ibase.lib icmm.lib icomp.lib idoc.lib iimg.lib imath.lib imod.lib iprm.lib isec.lib iser.lib istd.lib isys.lib iproc.lib
}
# Dependent libs for the GCC compiler
else{
	ACFSTD_DEPENDENCIES = libibase.a libicomp.a libiproc.a libidoc.a libisec.a libiprm.a libiimg.a libicmm.a libi2d.a libimath.a libisys.a libimod.a libiser.a libistd.a
}
