
project(SWL)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SWL_SOURCE_DIR}/cmake-find-modules")

set(APPLICATION_CXX_FILES
	src/widgets/application.cpp
	src/widgets/baseapplication.cpp
	src/SDL_FontCache.cpp
	src/widgets/canvas.cpp
	src/statemachine.cpp
	src/utility.cpp
	)
set(TEXT_BUTTON_CXX_FILES
	src/interfaces/button.cpp
	src/interfaces/text.cpp
	src/widgets/widgetitem.cpp
	src/widgets/rectitem.cpp
	src/widgets/textitem.cpp
	src/widgets/textbutton.cpp
	)

add_executable(demo_buttons
	demos/buttons_demo.cpp
	${APPLICATION_CXX_FILES}
	${TEXT_BUTTON_CXX_FILES}
	)
add_executable(demo_listview_simple
	demos/listview_simple_demo.cpp
	${APPLICATION_CXX_FILES}
	${TEXT_BUTTON_CXX_FILES}
	)
add_executable(demo_listview_haf
	demos/listview_hire_and_fire_demo.cpp
	${APPLICATION_CXX_FILES}
	${TEXT_BUTTON_CXX_FILES}
	)
add_executable(demo_scenes
	demos/scenes_demo.cpp
	${APPLICATION_CXX_FILES}
	${TEXT_BUTTON_CXX_FILES}
	)
add_executable(demo_canvas
	demos/canvas_demo.cpp
	${APPLICATION_CXX_FILES}
	${TEXT_BUTTON_CXX_FILES}
	)
add_executable(demo_menu
	demos/menuview_demo.cpp
	${APPLICATION_CXX_FILES}
	${TEXT_BUTTON_CXX_FILES}
	src/models/menumodel.cpp
	)

# Set compilers
# Comment the following two lines if CMake build fails:
SET(CMAKE_C_COMPILER /usr/bin/cc)
SET(CMAKE_CXX_COMPILER /usr/bin/c++)
# and, UNCOMMENT the following single line if CMake build fails:
#SET(CMAKE_CXX_COMPILER /usr/local/bin/c++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-std=c++11)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(
	${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
	${SDL2_MIXER_INCLUDE_DIR}
	include/
	src/
	)

set(LIBRARIES
	${SDL2_LIBRARY} 
	${SDL2_IMAGE_LIBRARIES} 
	${SDL2_TTF_LIBRARIES}
	${SDL2_MIXER_LIBRARIES}
	)

target_link_libraries(demo_buttons ${LIBRARIES})
target_link_libraries(demo_listview_simple ${LIBRARIES})
target_link_libraries(demo_listview_haf ${LIBRARIES})
target_link_libraries(demo_scenes ${LIBRARIES})
target_link_libraries(demo_canvas ${LIBRARIES})
target_link_libraries(demo_menu ${LIBRARIES})


