import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'istd' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/imod.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
