#Static lib definitions

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib.include)
set(MY_STATIC_LOGGER_LIB_SRC logger.c)

add_library(${PROJECT_NAME}_static STATIC ${MY_STATIC_LOGGER_LIB_SRC})



install(TARGETS ${PROJECT_NAME}_static ARCHIVE DESTINATION lib)

if(${CMAKE_CROSSCOMPILING})

    set(MY_SHARED_LOGGER_LIB_SRC logger.c)
    add_library(${PROJECT_NAME}_shared SHARED ${MY_SHARED_LOGGER_LIB_SRC})
    install(TARGETS ${PROJECT_NAME}_shared RUNTIME DESTINATION bin)

    #make shared library a dependency if we want shared library for this
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_shared)

    set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
endif(${CMAKE_CROSSCOMPILING})

add_executable(${PROJECT_NAME} ${MY_STATIC_LOGGER_LIB_SRC})
