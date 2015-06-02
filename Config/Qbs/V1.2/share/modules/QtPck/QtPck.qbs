import qbs 1.0

Module{
	Depends{ name: 'imath' }
	Depends{ name: 'iser' }
	Depends{ name: 'idoc' }
	Depends{ name: 'ibase' }
	Depends{ name: 'istd' }
	Depends{ name: 'iqtgui' }
	Depends{ name: 'iprm' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'ilog' }
	Depends{ name: 'iqtdoc' }
	Depends{ name: 'ifile' }
	Depends{ name: 'icmm' }
	Depends{ name: 'i2d' }
	Depends{ name: 'icomp' }
	Depends{ name: 'iwidgets' }
	Depends{ name: 'imod' }
	Depends{ name: 'iqt' }
	Depends{ name: 'iimg' }
	Depends{ name: 'ifilegui' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }

	readonly property path componentFilePath: path + '/../../../../../Bin/' + acf.compilerDir + '/QtPck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
