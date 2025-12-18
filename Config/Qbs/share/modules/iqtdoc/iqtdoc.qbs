import qbs 1.0

Module{
	Depends{ name: 'ifile' }
	Depends{ name: 'iser' }
	Depends{ name: 'ilog' }
	Depends{ name: 'ifilegui' }
	Depends{ name: 'iprm' }
	Depends{ name: 'imod' }
	Depends{ name: 'iqt' }
	Depends{ name: 'iqtgui' }
	Depends{ name: 'iwidgets' }
	Depends{ name: 'istd' }
	Depends{ name: 'idoc' }
	Depends{ name: 'ibase' }
	Depends{ name: 'imath' }
	Depends{ name: 'icomp' }
	Depends{ name: 'i2d' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'Qt.widgets' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/iqtdoc.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
