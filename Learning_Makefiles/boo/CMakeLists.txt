#CMakeLists.txt  , this is a comment 
cmake_minimum_required(VERSION 3.0) # this line tells about the CMake version for which this file is written

project(boo) #With the declaration of the project foo, the Visual Studio solution will have name foo.sln, and the Xcode project name will be foo.xcodeproj

add_library(boo_static STATIC boo.cpp)
# target_link_libraries(foo foo)
# add_executable(foo foo.cpp) #Adding executable foo with source foo.cpp
install(TARGETS boo_static
    ARCHIVE DESTINATION lib
)
message("Processing CMakeLists.txt") #Command line message
