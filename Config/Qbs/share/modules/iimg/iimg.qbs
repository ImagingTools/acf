import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'ilog' }
	Depends{ name: 'iqt' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'idoc' }
	Depends{ name: 'icmm' }
	Depends{ name: 'ibase' }
	Depends{ name: 'imath' }
	Depends{ name: 'i2d' }
	Depends{ name: 'icomp' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.gui' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/iimg.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
