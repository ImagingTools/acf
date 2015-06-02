import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'istd' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/imath.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
