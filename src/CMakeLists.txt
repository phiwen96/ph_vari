

file(GLOB_RECURSE HEADER_LIST "${PROJECT_SOURCE_DIR}/include/*.hpp" *.hpp)
file(GLOB SOURCE_LIST ${PROJECT_SOURCE_DIR}/src/*.cpp)

set (self ${PROJECT_NAME})

add_library (${self} STATIC 
	${SOURCE_LIST}  ${HEADER_LIST} 
)





ph_precompile (TARGET ${self} PUBLIC PCH ${std_pch} ${boost_pch})

add_library (ph::vari ALIAS ${self})
target_link_libraries (${self} PRIVATE ph::type ph::time ph::macros ph::type_list ${Boost_LIBRARIES})
target_include_directories (${self} PUBLIC ${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})
# target_include_directories (${self} PUBLIC ph::coroutines)
target_compile_features (${self} PUBLIC cxx_std_20)
target_include_directories (${self} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(${self} PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${self}>
    $<INSTALL_INTERFACE:include/${self}>
)

target_include_directories(${self} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

else()
    set_target_properties(${PROJECT_NAME}          PROPERTIES FOLDER "dependencies/${PROJECT_NAME}")
endif()