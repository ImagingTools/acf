import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'ilog' }
	Depends{ name: 'iprm' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'icomp' }
	Depends{ name: 'cpp' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'acf' }

	readonly property path componentFilePath: path + '/../../../../../Bin/' + acf.compilerDir + '/PackagePck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
