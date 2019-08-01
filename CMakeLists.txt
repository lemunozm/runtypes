cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(runtypes
    VERSION "0.1.0"
    LANGUAGES CXX
    )

include(GNUInstallDirs)

option(RUNTYPES_BUILD_TESTS "Build tests." OFF)
option(RUNTYPES_BUILD_EXAMPLES "Build examples." OFF)

#################################### LIBRARY ########################################
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

#################################### EXAMPLES #######################################
if(RUNTYPES_BUILD_EXAMPLES)

    set(EXAMPLE_BASIC ${PROJECT_NAME}_example_basic)

    add_executable(${EXAMPLE_BASIC} example/basic.cpp)

    target_link_libraries(${EXAMPLE_BASIC}
        PRIVATE
            ${PROJECT_NAME}
        )

    target_compile_options(${PROJECT_NAME}
        INTERFACE
            $<$<CXX_COMPILER_ID:GNU>:-Wall>
            $<$<CXX_COMPILER_ID:GNU>:-Wextra>
        )

    set_target_properties(${EXAMPLE_BASIC}
        PROPERTIES
            CXX_STANDARD 11
        )

endif()

###################################### TEST #########################################
if(RUNTYPES_BUILD_TESTS)

    set(TEST_UNITARY ${PROJECT_NAME}_test_unitary)

    add_executable(${TEST_UNITARY} test/unitary.cpp)

    target_link_libraries(${TEST_UNITARY}
        PRIVATE
            ${PROJECT_NAME}
        )

    target_include_directories(${TEST_UNITARY}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/test/thirdparty/catch2/include
        )

    target_compile_options(${TEST_UNITARY}
        INTERFACE
            $<$<CXX_COMPILER_ID:GNU>:-Wall>
            $<$<CXX_COMPILER_ID:GNU>:-Wextra>
        )

    set_target_properties(${TEST_UNITARY}
        PROPERTIES
            CXX_STANDARD 11
        )

endif()

#################################### INSTALL ########################################
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

