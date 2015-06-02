import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/idoc.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
