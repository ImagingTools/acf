import qbs 1.0

Module{
	Depends{ name: 'ifile' }
	Depends{ name: 'iser' }
	Depends{ name: 'ilog' }
	Depends{ name: 'iprm' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'ibase' }
	Depends{ name: 'icomp' }
	Depends{ name: 'cpp' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.core' }

	readonly property path componentFilePath: path + '/../../../../../Bin/' + acf.compilerDir + '/FilePck.arp'

	cpp.includePaths: [
		path + '/../../../../../Impl',
		path + '/../../../../../Include'
	]
}
