import qbs 1.0

Module{
	Depends{ name: 'ifile' }
	Depends{ name: 'iser' }
	Depends{ name: 'ilog' }
	Depends{ name: 'iprm' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'ibase' }
	Depends{ name: 'icomp' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/ipackage.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
