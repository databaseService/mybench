project(mybench)

message("mybench src loading")

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mybench.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mybench_util.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mybench_config.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mybench_db_type.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mybench_result.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mybench_worker.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mybench_test_manager.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/test/mybench_test_login.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/concurrent/count_down_latch.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/driver/db_mysql.cc
)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include"
        )

add_executable(${PROJECT_NAME}
        ${SOURCES}
        )

TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread mysqlclient)
