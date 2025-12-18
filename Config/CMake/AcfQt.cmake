target_link_libraries(${PROJECT_NAME} iqtdoc iview iqt2d iqtprm iloggui ifilegui iqtgui iwidgets)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} user32)
endif()
