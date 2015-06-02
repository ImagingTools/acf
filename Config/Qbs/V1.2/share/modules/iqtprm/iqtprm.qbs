import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'ilog' }
	Depends{ name: 'iprm' }
	Depends{ name: 'iqt' }
	Depends{ name: 'imod' }
	Depends{ name: 'iqtgui' }
	Depends{ name: 'iwidgets' }
	Depends{ name: 'istd' }
	Depends{ name: 'ibase' }
	Depends{ name: 'imath' }
	Depends{ name: 'icomp' }
	Depends{ name: 'i2d' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.widgets' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'Qt.gui' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/iqtprm.lib'

	cpp.includePaths: [
		path + '/../../../../../Include',
		path + '/../../../../../AuxInclude'
	]
}
