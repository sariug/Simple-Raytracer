#include "myVec3.h"
myVec3::myVec3() {}
myVec3::myVec3(float x, float y, float z)
{
    myvector[0] = x;
    myvector[1] = y;
    myvector[2] = z;
}
float myVec3::dot(myVec3 &other)
{
    float temp = 0;
    for (int i = 0; i < 3; i++)
    {
        temp += myvector[i] * other[i];
    }
    return temp;
}
float &myVec3::operator[](int idx)
{
    return myvector[idx];
}

void myVec3::normalize()
{
    float norm = 0;
    for (int i = 0; i < 3; i++)
        norm += pow(myvector[i], 2);
    norm = sqrt(norm);
    for (int i = 0; i < 3; i++)
        myvector[i] /= norm;
}