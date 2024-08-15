cmake_minimum_required (VERSION 3.24 FATAL_ERROR)

set(PROJECT VRInfinity)
project(${PROJECT})

set(DATABASE_HEADERS 
	src/dataBase/DBController.h
	src/dataBase/poolControllers.h
)

set(DATABASE_SOURCE
	src/dataBase/DBController.cpp
	src/dataBase/poolControllers.cpp
)

set(TABLES_HEADERS
	src/dataBase/tables/game_vr.h
	src/dataBase/tables/image_news.h
	src/dataBase/tables/news.h
	src/dataBase/tables/reset_password.h
	src/dataBase/tables/service.h
	src/dataBase/tables/text_news.h
	src/dataBase/tables/user.h
	src/dataBase/tables/visit.h
	src/dataBase/tables/visits_smallhall.h
	src/dataBase/tables/visits_vr.h
)

set(CRUD_HEADERS
	src/dataBase/CRUD/game_vr_CRUD.h
	src/dataBase/CRUD/image_news_CRUD.h
	src/dataBase/CRUD/news_CRUD.h
	src/dataBase/CRUD/reset_password_CRUD.h
	src/dataBase/CRUD/service_CRUD.h
	src/dataBase/CRUD/text_news_CRUD.h
	src/dataBase/CRUD/user_CRUD.h
	src/dataBase/CRUD/visit_CRUD.h
	src/dataBase/CRUD/visits_smallhall_CRUD.h
	src/dataBase/CRUD/visits_vr_CRUD.h
)

add_executable(${PROJECT} 
	${DATABASE_HEADERS}
	${DATABASE_SOURCE}
	${TABLES_HEADERS}
	${CRUD_HEADERS}
	
	src/bookingLogic.cpp
	src/bookingLogic.h
	
	src/genericHTML.cpp
	src/genericHTML.h
	
	src/getMethods.cpp
	src/getMethods.h
	
	src/main.cpp
	
	src/localParser.cpp
	src/localParser.h
	
	src/postMethods.cpp
	src/postMethods.h
	
	src/security.cpp
	src/security.h
	
	src/utility.h
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
