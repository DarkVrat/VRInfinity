cmake_minimum_required (VERSION 3.24 FATAL_ERROR)

set(PROJECT VRInfinity)
project(${PROJECT})

add_executable(${PROJECT} 
	src/main.cpp
)

add_definitions(-D_WIN32_WINNT=0x0601)
add_definitions(/wd4566)
target_compile_features(${PROJECT} PUBLIC cxx_std_17)

set(ASIO_INCLUDE_DIR "C:/libraries/asio/asio/include")
target_include_directories(${PROJECT} PRIVATE ${ASIO_INCLUDE_DIR})

set(CROW_INCLUDE_DIR "external/crow/include")
target_include_directories(${PROJECT} PRIVATE ${CROW_INCLUDE_DIR})

set_target_properties(${PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT}>/res)
