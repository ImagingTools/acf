import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'ilog' }
	Depends{ name: 'iprm' }
	Depends{ name: 'iqt' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'iimg' }
	Depends{ name: 'idoc' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'icmm' }
	Depends{ name: 'ibase' }
	Depends{ name: 'imath' }
	Depends{ name: 'i2d' }
	Depends{ name: 'icomp' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'acf' }

	readonly property path componentFilePath: path + '/../../../../../Bin/' + acf.compilerDir + '/BitmapPck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
