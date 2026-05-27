cmake_minimum_required(VERSION 3.16)
project(Lab_4 CXX)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

set(SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/)

set(GUI 
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/
    )
set(HEADER_DIR
    ${SRC}Optional
    ${SRC}Exception
    ${SRC}Generator
    ${CMAKE_CURRENT_SOURCE_DIR}/externals/lab_2/src
    ${CMAKE_CURRENT_SOURCE_DIR}/externals/lab_2/src/Sequence
    ${CMAKE_CURRENT_SOURCE_DIR}/externals/lab_2/src/Sequence/LazySequence/
    ${CMAKE_CURRENT_SOURCE_DIR}/externals/lab_2/src/DynamicArray
    ${CMAKE_CURRENT_SOURCE_DIR}/externals/lab_2/src/LinkedList
    ${CMAKE_CURRENT_SOURCE_DIR}/externals/lab_2/src/Exception
    ${CMAKE_CURRENT_SOURCE_DIR}/externals/lab_2/src/IEnumerator
    ${CMAKE_CURRENT_SOURCE_DIR}/externals/lab_2/src/IEnumerator/LazyEnumerators/
)

set(HEADER_FILES
    ${SRC}Optional/Optional.hpp
    ${SRC}Exception/Exception.hpp
    ${SRC}Generator/Generator.hpp
)

set(TEMPLATE_FILES

)

set(SOURCE_FILES
    
)

set(TEST_SOURCE_FILES
    tests/OptionalTest.cpp
)

add_subdirectory(externals/lab_2 lab_2_build)

set(BUILD_LAB2_TESTS ON CACHE BOOL "" FORCE)
set(BUILD_LAB2_GUI ON CACHE BOOL "" FORCE)



add_library(${PROJECT_NAME} STATIC
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${TEMPLATE_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Lab_2
    OpenGUI
    gtest
)



target_include_directories(${PROJECT_NAME} PUBLIC
 ${HEADER_DIR}

 )

add_executable(main
    ${GUI}main.cpp

)

target_link_libraries(main PRIVATE
    ${PROJECT_NAME}
    Lab_2
    OpenGUI
)

target_include_directories(main PUBLIC
    ${GUI}
    ${GUI}View/
    ${GUI}Controller/
    ${GUI}Model/
    ${PROJECT_NAME}
)

enable_testing()


add_executable(test_4
    ${TEST_SOURCE_FILES}
)

target_link_libraries(test_4 PRIVATE 
    ${PROJECT_NAME} 
    gtest_main
)


target_include_directories(test_4 PRIVATE ${HEADER_DIR})

include(GoogleTest)
gtest_discover_tests(test_4
)
