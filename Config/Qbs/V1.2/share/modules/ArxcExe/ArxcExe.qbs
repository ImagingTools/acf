import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'ilog' }
	Depends{ name: 'iprm' }
	Depends{ name: 'iqt' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'ipackage' }
	Depends{ name: 'ibase' }
	Depends{ name: 'imath' }
	Depends{ name: 'i2d' }
	Depends{ name: 'icomp' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'acf' }
	Depends{ name: 'cpp' }

	readonly property path acfBinDirectory: path + '/../../../../.'
}
