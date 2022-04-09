# Standard settings for a Qt based UI project
include (QtBaseConfig.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += svgwidgets

