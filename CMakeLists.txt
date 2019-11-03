cmake_minimum_required( VERSION 3.4.3 )
set(CMAKE_CXX_STANDARD 17)
project( raytracing CXX )
set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
add_executable( raytracing main.cpp
                            myVec3.cpp 
                           )

