import qbs 1.0

Module{
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/AcfLoc.lib'

	cpp.includePaths: [
		path + '/../../../../../Impl'
	]
}
