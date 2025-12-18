import qbs 1.0

Module{
	Depends{ name: 'imath' }
	Depends{ name: 'iser' }
	Depends{ name: 'idoc' }
	Depends{ name: 'iview' }
	Depends{ name: 'ibase' }
	Depends{ name: 'istd' }
	Depends{ name: 'iqtgui' }
	Depends{ name: 'iprm' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'ilog' }
	Depends{ name: 'icmm' }
	Depends{ name: 'i2d' }
	Depends{ name: 'icomp' }
	Depends{ name: 'iwidgets' }
	Depends{ name: 'imod' }
	Depends{ name: 'iqt' }
	Depends{ name: 'iimg' }
	Depends{ name: 'iqt2d' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.core' }

	readonly property path componentFilePath: path + '/../../../../../Bin/' + acf.compilerDir + '/QtViewPck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
