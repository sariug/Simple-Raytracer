#ifndef myVec3_h
#define myVec3_h
#include <iostream>
#include <vector>
#include <math.h>

class myVec3
{
public:
    // Constructor
    myVec3();
    myVec3(float i);
    myVec3(float x, float y, float z);
    myVec3(const myVec3 &p2);
    // Destructor
    ~myVec3(){};
    // Operator Overloading
    float &operator[](int idx);
    myVec3 operator+(myVec3 other);
    myVec3 operator-(myVec3 other);
    myVec3 operator*(float scalar);
    // Methods
    float dot(myVec3 &other);
    myVec3 normalize();
    float clip(int i);
    float norm();

private:
    std::vector<float> myvector;
};

#endif