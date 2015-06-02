import qbs 1.0

Module{
	Depends{ name: 'iser' }
	Depends{ name: 'imod' }
	Depends{ name: 'istd' }
	Depends{ name: 'imath' }
	Depends{ name: 'i2d' }
	Depends{ name: 'icomp' }
	Depends{ name: 'acf' }
	Depends{ name: 'Qt.xml' }
	Depends{ name: 'Qt.core' }
	Depends{ name: 'cpp' }

	cpp.staticLibraries: path + '/../../../../../Lib/' + acf.compilerDir + '/iqt.lib'

	cpp.includePaths: [
		path + '/../../../../../Include'
	]
}
