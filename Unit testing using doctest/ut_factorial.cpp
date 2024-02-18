#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "factorial.cpp"


int main()
{
     doctest::Context context;
     context.setOption("order-by", "name"); 

     int res = context.run(); // run

    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests
    
}

TEST_CASE("testing the factorial function") {
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
    CHECK(factorial(0) == 1);
    CHECK_THROWS_WITH_AS(factorial(-2), "negative number not allowed", std::invalid_argument);
}





