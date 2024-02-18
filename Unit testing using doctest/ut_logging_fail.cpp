#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("Example of FAIL and FAIL_CHECK") {
    int value = 42;

    // Example 1: FAIL
    // This will immediately mark the test case as failed and terminate its execution.
    if (value != 42) {
        FAIL("Test case failed because the value is not 42.");
    }

    // Example 2: FAIL_CHECK
    // This will mark the test case as failed but continue its execution.
    CHECK(value == 42);  // This condition passes, so the test continues.
    FAIL_CHECK("This message will be printed, and the test case is marked as failed.");

    // The following code will still be executed even after FAIL_CHECK.
    // It's useful to log additional information or perform cleanup.


}
