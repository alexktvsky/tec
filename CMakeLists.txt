cmake_minimum_required(VERSION 3.13.0)

project(tec
    LANGUAGES CXX
    DESCRIPTION "TEC (Template Extension Compiler)"
)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_sources(${PROJECT_NAME}
PRIVATE
    src/main.cpp
    src/syntax_exception.cpp
    src/token.cpp
    src/preprocessor.cpp
    src/stack.cpp
    src/pda.cpp
    src/template_symbols.cpp
)

target_compile_options(${PROJECT_NAME}
PRIVATE
    -Wall
    -Wextra
    -Wuninitialized
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)

if (NOT CMAKE_BUILD_TYPE)
    set (build_type release)
else()
    string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)
endif()

if (build_type STREQUAL debug)
    target_compile_definitions(${PROJECT_NAME} PRIVATE TEC_DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -g3 -O0 -ggdb3)
elseif (build_type STREQUAL release)
    target_compile_options(${PROJECT_NAME} PRIVATE -g0 -O3)
endif()

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_REQUIRED_LIBRARIES}
)

string(TOLOWER ${CMAKE_SYSTEM_NAME} system_name)
if (system_name STREQUAL windows)
else()
    target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(PROGRAMS util/tec-gcc DESTINATION bin)
install(PROGRAMS util/tec-clang DESTINATION bin)
install(PROGRAMS util/tec-i686-w64-mingw32-gcc DESTINATION bin)
install(PROGRAMS util/tec-x86_64-w64-mingw32-gcc DESTINATION bin)
