import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'istd' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/iprm.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
