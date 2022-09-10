cmake_minimum_required(VERSION 3.13)
project(rental_sys C)

set(CMAKE_C_STANDARD 11)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
find_package(MySQL REQUIRED)

add_executable(rental_sys
        main.c
        controller/cliente.c
        controller/login.c
        controller/impiegato.c
        controller/manager.c
        model/db.c
        utils/db.c
        utils/dotenv.c
        utils/io.c
        utils/validation.c
        view/cliente.c
        view/login.c
        view/impiegato.c
        view/manager.c
)

include_directories(${MYSQL_INCLUDE_DIR})
target_link_libraries(rental_sys ${MYSQL_LIBRARIES})

if(MSVC)
    add_compile_options(PRIVATE /W4 /WX)
else()
    add_compile_options(PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
