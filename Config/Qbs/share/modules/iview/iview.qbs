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
	Depends{ name: 'iimg' }
	Depends{ name: 'idoc' }
	Depends{ name: 'icmm' }
	Depends{ name: 'ibase' }
	Depends{ name: 'imath' }
	Depends{ name: 'icomp' }
	Depends{ name: 'i2d' }
	Depends{ name: 'Qt.widgets' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/iview.lib'

	cpp.includePaths: [
		path + '/../../../../../Include',
		path + '/../../../../../AuxInclude'
	]
}
