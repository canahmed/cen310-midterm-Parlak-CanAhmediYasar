//#define ENABLE_teamManager_TEST  // Uncomment this line to enable the teamManager tests

#include "gtest/gtest.h"
#include "../../teamManager/header/teamManager.h"  // Adjust this include path based on your project structure

using namespace Can::teamManager;

class teamManagerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Setup test data
  }

  void TearDown() override {
    // Clean up test data
  }
};

TEST_F(teamManagerTest, TestAdd) {
  double result = teamManager::add(5.0, 3.0);
  EXPECT_DOUBLE_EQ(result, 8.0);
}

TEST_F(teamManagerTest, TestSubtract) {
  double result = teamManager::subtract(5.0, 3.0);
  EXPECT_DOUBLE_EQ(result, 2.0);
}

TEST_F(teamManagerTest, TestMultiply) {
  double result = teamManager::multiply(5.0, 3.0);
  EXPECT_DOUBLE_EQ(result, 15.0);
}

TEST_F(teamManagerTest, TestDivide) {
  double result = teamManager::divide(6.0, 3.0);
  EXPECT_DOUBLE_EQ(result, 2.0);
}

TEST_F(teamManagerTest, TestDivideByZero) {
  EXPECT_THROW(teamManager::divide(5.0, 0.0), std::invalid_argument);
}

/**
 * @brief The main function of the test program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return int The exit status of the program.
 */
int main(int argc, char **argv) {
#ifdef ENABLE_teamManager_TEST
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
#else
  return 0;
#endif
}
