import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/icomp.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
