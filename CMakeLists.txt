cmake_minimum_required (VERSION 3.24 FATAL_ERROR)

set(PROJECT VRInfinity)
project(${PROJECT})

set(DATABASE_HEADERS 
    src/dataBase/users.h
	src/dataBase/usersCRUD.h
	src/dataBase/visits.h
	src/dataBase/visitsCRUD.h
	src/dataBase/DBController.h
)

set(DATABASE_SOURCE
	src/dataBase/DBController.cpp
)

add_executable(${PROJECT} 
	src/main.cpp
	${DATABASE_HEADERS}
	${DATABASE_SOURCE}
)

source_group("Header Files\\DataBase" FILES ${DATABASE_HEADERS})
source_group("Source Files\\DataBase" FILES ${DATABASE_SOURCE})

add_definitions(-D_WIN32_WINNT=0x0601)
add_definitions(/wd4566)
target_compile_features(${PROJECT} PUBLIC cxx_std_17)

add_subdirectory(external/nanodbc)
target_link_libraries(${PROJECT} nanodbc)

set(ASIO_INCLUDE_DIR "C:/libraries/asio/asio/include")
target_include_directories(${PROJECT} PRIVATE ${ASIO_INCLUDE_DIR})

set(CROW_INCLUDE_DIR "external/crow/include")
target_include_directories(${PROJECT} PRIVATE ${CROW_INCLUDE_DIR})

set_target_properties(${PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT}>/res)
