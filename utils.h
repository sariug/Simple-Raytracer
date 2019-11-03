#include <vector>
#include <math.h>

#include "myVec3.h"
const float inf = std::numeric_limits<float>::infinity();
const myVec3 color_plane0 = myVec3(1);
const myVec3 color_plane1 = myVec3(0);
struct obj
{
	int type; // 0 = plane, 1 = sphere
	myVec3 position;
	// Sphere
	float radius;
	// Plane
	myVec3 normal;

	// color
	myVec3 color; // No opacity  !
	myVec3 get_color(myVec3 M)
	{
		return (int(M[0] * 2) % 2) == (int(M[2] * 2) % 2) ? color_plane0 : color_plane1;
	}
	float diffuse_c, specular_c, reflection;
};

template <typename T>
std::vector<T> linspace(T start, T end, int num)
{
	std::vector<T> linspaced;
	if (num < 2)
		return linspaced;

	T delta = (end - start) / (num - 1);

	for (int i = 0; i < num - 1; ++i)
		linspaced.push_back(start + delta * i);
	linspaced.push_back(end);
	return linspaced;
}

float intersect_plane(myVec3 O, myVec3 D, myVec3 P, myVec3 N)
{
	auto denom = D.dot(N);
	if (fabs(denom) < 1e-6)
		return inf;
	auto T = P - O;
	auto d = T.dot(N) / denom;
	if (d < 0)
		return inf;
	return d;
}

float intersect_sphere(myVec3 O, myVec3 D, myVec3 S, float R)
{
	auto a = D.dot(D);
	auto OS = O - S;
	auto b = 2 * D.dot(OS);
	auto c = OS.dot(OS) - (R * R);

	auto disc = b * b - 4 * a * c;
	if (disc > 0)
	{
		auto distSqrt = sqrt(disc);
		auto q = b < 0 ? (-b - distSqrt) / 2.0 : (-b + distSqrt) / 2.0;
		auto t0 = q / a;
		auto t1 = c / q;
		t0 = std::min(t0, t1);
		t1 = std::max(t0, t1);
		if (t1 >= 0)
			return t0 < 0 ? t1 : t0;
	}
	return inf;
}

float intersect(myVec3 O, myVec3 D, obj o)
{
	if (o.type == 0) //plane
		return intersect_plane(O, D, o.position, o.normal);
	else //sphere
		return intersect_sphere(O, D, o.position, o.radius);
}


myVec3 get_normal(obj Obj, myVec3 M)
{

	if (Obj.type == 0) //Plane
	{
		return Obj.normal;
	}
	else
	{
		auto T = (M - Obj.position).normalize();;
		return T;
	}
}

myVec3 get_color(obj o, myVec3 M)
{
	return o.type == 0 ? o.color = o.get_color(M) : o.color;
}

obj add_sphere(myVec3 pos, float rad, myVec3 col)
{
	obj o;
	o.type = 1;
	o.position = pos;
	o.radius = rad;
	o.color = col;
	o.diffuse_c = 1.f;
	o.specular_c = 1.f;
	o.reflection = .5f;

	return o;
}

obj add_plane(myVec3 pos, myVec3 col)
{
	obj o;
	o.type = 0;
	o.position = pos;
	o.diffuse_c = .75;
	o.specular_c = .5f;
	o.reflection = .25f;
	o.normal = myVec3(0, 1, 0);
	return o;
}

