project(ifilegui LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

if(QT_VERSION_MAJOR EQUAL 6)
	find_package(Qt${QT_VERSION_MAJOR} COMPONENTS SvgWidgets REQUIRED)
	target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::SvgWidgets)
	target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Concurrent)
endif()

acf_use_qt_graphics_modules()



