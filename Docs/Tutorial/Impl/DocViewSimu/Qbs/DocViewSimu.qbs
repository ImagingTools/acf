import qbs.base 1.0
import AcfApplication as AcfApplication

AcfApplication{
    name: "DocViewSimuExe"
    targetName: "DocViewSimu"

    files: ["../*.cpp"]

    Depends{ name: "BasePck" }
    Depends{ name: "QtPck" }
    Depends{ name: "QtGuiPck" }
    Depends{ name: "TutorialPck" }
}
