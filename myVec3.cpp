#include "myVec3.h"

myVec3::myVec3() { myvector.resize(3); }
myVec3::myVec3(float x, float y, float z)
{
    myvector.resize(3);
    myvector[0] = x;
    myvector[1] = y;
    myvector[2] = z;
}
myVec3::myVec3(float i)
{
    myvector.resize(3, i);
}
float myVec3::clip(int i)
{
    return myvector[i] < 0 ? 0.0 : (myvector[i] > 1 ? 1.0 : myvector[i]);
}
myVec3 myVec3::operator*(float scalar)
{
    myVec3 res;
    for (int i = 0; i < 3; i++)
        res[i] = myvector[i] * scalar;
    return res;
}
myVec3 myVec3::operator+(myVec3 other)
{
    myVec3 res;
    for (int i = 0; i < 3; i++)
        res[i] = myvector[i] + other[i];
    return res;
}
myVec3 myVec3::operator-(myVec3 other)
{
    myVec3 res;
    for (int i = 0; i < 3; i++)
        res[i] = myvector[i] - other[i];
    return res;
}
myVec3::myVec3(const myVec3 &p2)
{
    myvector.resize(3);
    for (int i = 0; i < 3; i++)
        myvector[i] = p2.myvector[i];
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
float myVec3::norm(){
    float norm = 0;
    for (int i = 0; i < 3; i++)
        norm += pow(myvector[i], 2);
    norm = sqrt(norm);
    return norm;
}
myVec3 myVec3::normalize()
{
    myVec3 res;

    for (int i = 0; i < 3; i++)
        res[i] = myvector[i] / this->norm();
    
    return res;
}