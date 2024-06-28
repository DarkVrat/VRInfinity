cmake_minimum_required (VERSION 3.24 FATAL_ERROR)

set(PROJECT VRInfinity)
project(${PROJECT})

set(DATABASE_HEADERS 
	src/dataBase/DBController.h
	src/dataBase/converterTimeString.h
)

set(DATABASE_SOURCE
	src/dataBase/DBController.cpp
)

set(TABLES_HEADERS
	src/dataBase/tables/game.h
	src/dataBase/tables/imageNews.h
	src/dataBase/tables/news.h
	src/dataBase/tables/textNews.h
	src/dataBase/tables/user.h
	src/dataBase/tables/visit.h
)

set(CRUD_HEADERS
	src/dataBase/CRUD/gameCRUD.h
	src/dataBase/CRUD/imageNewsCRUD.h
	src/dataBase/CRUD/newsCRUD.h
	src/dataBase/CRUD/textNewsCRUD.h
	src/dataBase/CRUD/userCRUD.h
	src/dataBase/CRUD/visitCRUD.h
)

add_executable(${PROJECT} 
	src/main.cpp
	src/security.h
	src/parser.h
	src/genericHTML.h
	src/stringConverter.h
	src/Utf8Conv.hpp
	${DATABASE_HEADERS}
	${DATABASE_SOURCE}
	${TABLES_HEADERS}
	${CRUD_HEADERS}
)

source_group("Header Files\\DataBase" FILES ${DATABASE_HEADERS})
source_group("Header Files\\DataBase\\TABLES" FILES ${TABLES_HEADERS})
source_group("Header Files\\DataBase\\CRUD" FILES ${CRUD_HEADERS})
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
