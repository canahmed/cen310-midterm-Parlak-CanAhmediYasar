# tests/CMakeLists.txt

# Download and include GoogleTest
include(FetchContent)

# OFFLINE 
# ----------------------------------------------------------------------------------
# Include GoogleTest
set(GTEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest)

# Add GoogleTest as a subdirectory
add_subdirectory(${GTEST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/googletest-build EXCLUDE_FROM_ALL)


# Include the Google Test framework
include_directories(${GTEST_DIR}/googletest/include)
# ----------------------------------------------------------------------------------
 
# ONLINE 
# ----------------------------------------------------------------------------------
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP true
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add test executable
add_executable(monte_carlo_pi_test
    monte_carlo_pi_test.cpp
)

# Link against Google Test and our library
target_link_libraries(monte_carlo_pi_test PRIVATE
    monte_carlo_pi_lib
    GTest::gtest_main
)

# Add test to CTest
add_test(NAME monte_carlo_pi_test COMMAND monte_carlo_pi_test)

# Set test properties
set_tests_properties(monte_carlo_pi_test PROPERTIES
    PASS_REGULAR_EXPRESSION "All tests passed"
    FAIL_REGULAR_EXPRESSION "Failed"
)
# ----------------------------------------------------------------------------------

# Include the Google Test framework
# include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


# Utility tests
if(ENABLE_UTILITY)
	add_subdirectory(utility)
endif()

# teamManager tests
if(ENABLE_teamManager)
	add_subdirectory(teamManager)
endif()

