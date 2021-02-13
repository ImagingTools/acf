include(${CMAKE_CURRENT_LIST_DIR}/GeneralConfig.cmake)

include_directories(${PROJECT_DIR}/../../)

set(DESTDIR ${PROJECT_DIR}/../../../Lib/${COMPILER_DIR})

file(GLOB PROJECT_NAME_SRC
	"../*.h"
	"../*.cpp"
)

add_library(${PROJECT_NAME} STATIC ${PROJECT_NAME_SRC})

