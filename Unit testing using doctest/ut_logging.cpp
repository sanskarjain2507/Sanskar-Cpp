#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"



#include <vector>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <vector>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

TEST_CASE("logging the counter of a loop") {
    std::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(4);
    vec.push_back(8);
    vec.push_back(16);
    
    INFO("current iteration of loop:");
    for(unsigned i = 0; i < vec.size(); ++i) {
        CAPTURE(i);
        CHECK(vec[i] == (1 << i));
    }
}
