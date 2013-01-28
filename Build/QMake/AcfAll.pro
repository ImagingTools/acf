TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += Static
SUBDIRS += Component
SUBDIRS += Tool

Static.file = Static.pro
Component.file = Component.pro
Component.depends = Static
Tool.file = Tool.pro
Tool.depends = Component
