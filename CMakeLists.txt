# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.12)
project(monte_carlo_pi)

# Set source directory
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Print useful information
message(STATUS "[src] CMake version ${CMAKE_VERSION}")
message(STATUS "[src] System ${CMAKE_SYSTEM_NAME}")
message(STATUS "[src] Processor ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "[src] C++ standard version: ${CMAKE_CXX_STANDARD}")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set installation directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
message(STATUS "[src] Folder is set to : ${CMAKE_INSTALL_PREFIX}")

# Enable testing
enable_testing()

# Add code coverage support for MSVC
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O0 /profile")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /PROFILE")
endif()

# Add Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Find OpenMP
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found")
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set compiler flags
if(MSVC)
    add_compile_options(/W4 /WX)
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        add_compile_options(/EHsc)
    endif()
endif()

# Add Monte Carlo Pi library
add_library(monte_carlo_pi_lib STATIC
    ${SRC_DIR}/lib/monte_carlo_pi.cpp
)
target_include_directories(monte_carlo_pi_lib PUBLIC ${SRC_DIR}/lib)
target_link_libraries(monte_carlo_pi_lib PUBLIC OpenMP::OpenMP_CXX)

# Set static runtime for non-GUI targets
if(MSVC)
    set_property(TARGET monte_carlo_pi_lib PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Add GUI application
add_executable(monte_carlo_pi_app
    ${SRC_DIR}/app/monte_carlo_pi_app.cpp
)
target_link_libraries(monte_carlo_pi_app PRIVATE monte_carlo_pi_lib)
if(MSVC)
    # GUI uygulaması için CLR desteği
    set_target_properties(monte_carlo_pi_app PROPERTIES
        VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.2"
        VS_GLOBAL_CLRSupport "true"
        COMPILE_FLAGS "/clr"
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
    )
    
    # GUI uygulaması için dinamik çalışma zamanı kütüphanesi
    set_property(TARGET monte_carlo_pi_app PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>")
    
    # GUI uygulaması için özel derleyici bayrakları
    target_compile_options(monte_carlo_pi_app PRIVATE /EHa)
    
    # GUI uygulaması için /RTC seçeneklerini devre dışı bırak
    target_compile_options(monte_carlo_pi_app PRIVATE /RTC-)
    
    # GUI uygulaması için /RTC1 seçeneğini devre dışı bırak (alternatif yöntem)
    set_property(TARGET monte_carlo_pi_app PROPERTY
        VS_GLOBAL_RuntimeLibrary "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>")
    set_property(TARGET monte_carlo_pi_app PROPERTY
        VS_GLOBAL_EnableManagedIncrementalBuild "false")
    
    # GUI uygulaması için /RTC1 seçeneğini devre dışı bırak (doğrudan yöntem)
    set_property(TARGET monte_carlo_pi_app PROPERTY
        VS_GLOBAL_BasicRuntimeChecks "Default")
endif()

# Add test executable
add_executable(monte_carlo_pi_test
    ${SRC_DIR}/tests/monte_carlo_pi_test.cpp
)
target_link_libraries(monte_carlo_pi_test PRIVATE
    monte_carlo_pi_lib
    GTest::gtest_main
)

# Set static runtime for test executable
if(MSVC)
    set_property(TARGET monte_carlo_pi_test PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_test(NAME monte_carlo_pi_test COMMAND monte_carlo_pi_test)
set_tests_properties(monte_carlo_pi_test PROPERTIES
    PASS_REGULAR_EXPRESSION "All tests passed"
    FAIL_REGULAR_EXPRESSION "Failed"
)


