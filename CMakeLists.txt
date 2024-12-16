# CMackeLists.txt for InternetMessage
#
# © 2024 by Hatem Nabli

cmake_minimum_required(VERSION 3.8)
set(this MessageHeaders)

set(Headers
    include/MessageHeaders/MessageHeaders.hpp
)

set(Sources
    src/MessageHeaders.cpp
)

add_library(${this} STATIC ${Sources} ${Headers})
set_target_properties(${this} PROPERTIES
    FOLDER Libraries
)

target_include_directories(${this} PUBLIC include)

target_link_libraries(${this} PUBLIC
    StringUtils
)

add_subdirectory(test)