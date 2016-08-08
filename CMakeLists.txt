cmake_minimum_required (VERSION 2.6)
project (Chess)

add_subdirectory(/home/i/oxygine/oxygine-framework/ oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")

add_executable(
Chess

src/main.cpp

src/logic/Board.h
src/logic/Chesscell.h
src/logic/Global.h

src/logic/Board.cpp
src/logic/Chesscell.cpp

src/interface/BoardArena.h
src/interface/Chess.h
src/interface/shared.h

src/interface/BoardArena.cpp
src/interface/Chess.cpp
src/interface/shared.cpp
)
target_link_libraries(Chess ${OXYGINE_CORE_LIBS})



if (WIN32) #disable console mode for VC++
	set_target_properties(Chess PROPERTIES WIN32_EXECUTABLE TRUE)
endif(WIN32)



if (EMSCRIPTEN)
	SET(CMAKE_EXECUTABLE_SUFFIX ".html")	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NO_EXIT_RUNTIME=1 -s WARN_ON_UNDEFINED_SYMBOLS=1 --memory-init-file 0 -s TOTAL_MEMORY=50331648")
	em_link_pre_js(Chess  ${OXYGINE_JS_LIBRARIES}  ${CMAKE_CURRENT_SOURCE_DIR}/data.js)
endif(EMSCRIPTEN)
