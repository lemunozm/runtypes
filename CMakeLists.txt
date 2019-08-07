cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(runtypes
    VERSION "0.1.0"
    LANGUAGES CXX
    )

include(GNUInstallDirs)

option(RUNTYPES_BUILD_TESTS "Build tests." OFF)
option(RUNTYPES_BUILD_EXAMPLES "Build examples." OFF)

#####################################################################################
#                                    Library
#####################################################################################
add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/runtypes/Exception.hpp>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/runtypes/Type.hpp>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/runtypes/CType.hpp>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/runtypes/Struct.hpp>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/runtypes/Data.hpp>
    )

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    )

target_compile_features(${PROJECT_NAME}
    INTERFACE
        cxx_range_for #"equivalent" to cxx_std_11
    )

#####################################################################################
#                                    Examples
#####################################################################################
macro(compile_example EXAMPLE_NAME EXAMPLE_SOURCE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})

    target_link_libraries(${EXAMPLE_NAME}
        PRIVATE
            ${PROJECT_NAME}
        )

    target_compile_options(${EXAMPLE_NAME}
        INTERFACE
            $<$<CXX_COMPILER_ID:GNU>:-Wall>
            $<$<CXX_COMPILER_ID:GNU>:-Wextra>
        )

    set_target_properties(${EXAMPLE_NAME}
        PROPERTIES
            CXX_STANDARD 11
        )
endmacro()

if(RUNTYPES_BUILD_EXAMPLES)
    compile_example(${PROJECT_NAME}_example_basic examples/basic.cpp)
    compile_example(${PROJECT_NAME}_example_custom_struct examples/custom_struct.cpp)
endif()

#####################################################################################
#                                      Tests
#####################################################################################
macro(compile_test TEST_NAME TEST_SOURCE)
    compile_example(${TEST_NAME} ${TEST_SOURCE})

    target_include_directories(${TEST_NAME}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/test/thirdparty/catch2/include
        )

endmacro()

if(RUNTYPES_BUILD_TESTS)
    compile_test(${PROJECT_NAME}_test_unitary test/unitary.cpp)
endif()

#####################################################################################
#                                   Installation
#####################################################################################
install(
    TARGETS
        ${PROJECT_NAME}
    EXPORT
        ${PROJECT_NAME}_Targets
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION
        ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION
        ${CMAKE_INSTALL_LIBDIR}
    )

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}/include/runtypes/
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/runtypes
    FILES_MATCHING
        PATTERN "*.hpp"
    )

install(
    EXPORT
        ${PROJECT_NAME}_Targets
    DESTINATION
        ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake
    )

