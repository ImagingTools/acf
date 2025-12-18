import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'iqt' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'imath' }
	Depends{ name: 'i2d' }
	Depends{ name: 'icomp' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.widgets' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/iwidgets.lib'

	cpp.includePaths: [
		path + '/../../../../../Include',
		path + '/../../../../../AuxInclude'
	]
}
