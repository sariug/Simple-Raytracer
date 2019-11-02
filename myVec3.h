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
    {myvector[0]=i;myvector[1]=i;myvector[2]=i;}
    myVec3(float x, float y, float z);
    // Destructor
    ~myVec3(){};
    // Operator Overloading
    float& operator[](int idx);
    myVec3(const myVec3 &p2) {
        for(int i = 0;i<3;i++)
        myvector[i] = p2.myvector[i];  } 
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

    void print()
    {
                for(int i = 0;i<3;i++)
       std::cout<<myvector[i]<<" ";
       std::cout<<std::endl;
    }
    myVec3 clip()
    {
        myVec3 res;
        for(int i = 0;i<3;i++)
        res[i] = myvector[i]<0? 0.0:myvector[i] ;
                for(int i = 0;i<3;i++)
        res[i] = myvector[i]>1? 1.0:myvector[i] ;
        return res;
    }

private:
    float myvector[3];
};

#endif