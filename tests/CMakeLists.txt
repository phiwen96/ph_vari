# Testing library
set (${self} test_${PROJECT_NAME})
ph_fetch (catch2)
file(GLOB_RECURSE HEADER_LIST "include/*.hpp")
file(GLOB SOURCE_LIST *.cpp)
add_executable (test_${PROJECT_NAME} ${SOURCE_LIST} ${HEADER_LIST})
target_link_libraries (test_${PROJECT_NAME} PRIVATE ph::type ph::time ph::macros ph::type_list ${PROJECT_NAME} Catch2::Catch2)
target_include_directories (test_${PROJECT_NAME} PRIVATE include)
ph_precompile (TARGET test_${PROJECT_NAME} PRIVATE PCH ${std_pch})
add_test (NAME testtest_${PROJECT_NAME} COMMAND ${self}) 


if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_target_properties(test_${PROJECT_NAME}          PROPERTIES FOLDER "${PROJECT_NAME}")
else()
    set_target_properties(test_${PROJECT_NAME}           PROPERTIES FOLDER "dependencies/${PROJECT_NAME}")
endif()



