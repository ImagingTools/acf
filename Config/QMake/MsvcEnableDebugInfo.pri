CONFIG(release, debug|release){
	win32-msvc*{
		# activate debug info also for release builds
		QMAKE_LFLAGS += /DEBUG

		# activate debug info also for release builds
		QMAKE_CXXFLAGS += /Zi /Fd$$DESTDIR/"$$TARGET".pdb
	}
}
