import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'istd' }
	Depends{ name: 'imath' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/icmm.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
