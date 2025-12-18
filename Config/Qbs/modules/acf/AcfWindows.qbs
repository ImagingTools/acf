import qbs.base 1.0

AcfGeneral{
	condition: qbs.targetOS.contains('windows')

	cpp.staticLibraries: ['Mpr']
}
