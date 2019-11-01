#ifndef myVec3_h
#define myVec3_h
#include <iostream>
#include <math.h>

class myVec3
{
public:
    // Constructor
    myVec3();
    myVec3(float i)
    {for(auto a:myvector)a=i;}
    myVec3(float x, float y, float z);
    // Destructor
    ~myVec3(){};
    // Operator Overloading
    float& operator[](int idx);

    myVec3 operator+(myVec3 other)
    {
        myVec3 res;
        for(int i = 0;i<3;i++)
        res[i] = myvector[i] + other[i];
        return res;
    }
    myVec3 operator-(myVec3  other)
    {
        myVec3 res;
        for(int i = 0;i<3;i++)
        res[i] = myvector[i] - other[i];
        return res;
    }    
    myVec3 operator*(float scalar)
    {
        myVec3 res;
        for(int i = 0;i<3;i++)
        res[i] = myvector[i]*scalar;
        return res;
    }
    float dot(myVec3 &other);
    void normalize();

private:
    float myvector[3];
};

#endif