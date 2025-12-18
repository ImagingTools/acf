import qbs 1.0

Module{
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/istd.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
