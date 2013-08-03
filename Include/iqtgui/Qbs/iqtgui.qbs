import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct
import "../../../Config/Qbs/QtGuiBaseConfig.qbs" as QtGuiBaseConfig

StaticProduct{
	name: "iqtgui"

	Export {
		Depends{ name: "icomp" }
		Depends{ name: "ibase" }
		Depends{ name: "iprm" }
		Depends{ name: "iqt" }
		Depends{ name: "Qt.gui" }
	}
}
