#include <vector>
#include <thread>
#include <math.h>
#include <limits>
#include <optional>
#include <algorithm>
#include <fstream> // std::filebuf

#include "myVec3.h"
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

const float inf = std::numeric_limits<float>::infinity();
const int height = 400;
const int width = 300;
const myVec3 color_plane0 = myVec3(1);
const myVec3 color_plane1 = myVec3(0);
// Light position
myVec3 L(5, 5, -10);
myVec3 L_color(1); //White
float ambient = .05, diffuse_c = 1, specular_c = 1;
int specular_k = 50;
myVec3 O(0., 0.35, -1.); //Camera.
myVec3 Q(0);			 // Camera pointing to.
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
	float diffuse_c = .75, specular_c = .5, reflection = .25;
};

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
	auto T = O - S;
	auto b = 2 * D.dot(T);
	auto c = T.dot(T) - R * R;
	auto disc = b * b - 4 * a * c;
	if (disc > 0)
	{
		auto distSqrt = sqrt(disc);
		auto q = b < 0 ? (-b - distSqrt) / 2.0 : (-b + distSqrt) / 2.0;
		auto t0 = q / a;
		auto t1 = c / q;
		t0 = std::min(t0, t1);
		t1 = std::max(t0, t1);
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
		auto T = M - Obj.position;
		T.normalize();
		return T;
	}
}

myVec3 get_color(obj o, myVec3 M)
{
	if (o.type == 0)
	{
		o.color = o.get_color(M);
	}
	return o.color;
}

obj add_sphere(myVec3 pos, float rad, myVec3 col)
{
	obj o;
	o.type = 1;
	o.position = pos;
	o.radius = rad;
	o.reflection = .5f;
	o.color = col;
	return o;
}

obj add_plane(myVec3 pos, myVec3 col)
{
	obj o;
	o.type = 0;
	o.position = pos;
	o.normal = myVec3(0, 1, 0);
	return o;
}
std::optional<std::tuple<obj, myVec3, myVec3, myVec3>> trace_ray(myVec3 rayO, myVec3 rayD, std::vector<obj> &scene)
{
	float t = inf;
	int obj_idx = -1;
	for (std::size_t c = 0; c < scene.size(); c++)
	{

		float t_obj = intersect(rayO, rayD, scene[c]);
		if (t_obj < t)
		{
			t = t_obj;
			obj_idx = c;
		}
	}

	// Return None if the ray does not intersect any object.
	if (t == inf)
	{
		return std::nullopt;
	}

	// Find the point of intersection on the object.
	auto M = rayO + (rayD * t);
	auto N = get_normal(scene[obj_idx], M);
	auto color = get_color(scene[obj_idx], M);

	auto toL = (L - M);
	auto toO = (O - M);

	toL.normalize();
	toO.normalize();

	// Shadow: find if the point is shadowed or not.
	std::vector<float> l;
	for (std::size_t cc = 0; cc < scene.size(); cc++)
	{
		if (cc != obj_idx)
		l.push_back(intersect(M + N * .0001, toL, scene[cc]));
	}
	if (l.size() > 0 && l[*std::min_element(l.begin(), l.end())] < inf)
	{
		printf("%f \n",l[*std::min_element(l.begin(), l.end())]);
		M.print();
		N.print();
		toL.print();
		int a;
		//std::cin>>a;
		return std::nullopt;
	}
	// Start computing the color.
	myVec3 col_ray(ambient);
	// Lambert shading (diffuse).

	col_ray = col_ray + color * scene[obj_idx].diffuse_c * std::max(N.dot(toL), 0.0f);
	// Blinn-Phong shading (specular).
	auto temp = toL + toO;
	temp.normalize();
	col_ray = col_ray + L_color * scene[obj_idx].specular_c * pow(std::max(N.dot(temp), 0.0f), specular_k);
	return std::make_tuple(scene[obj_idx], M, N, col_ray);
}

int main()
{
	//Test_Vector_Class();
	// Create Scene
	std::vector<obj> scene = {add_sphere(myVec3(.75, .1, 1.), .6, myVec3(0., 0., 1.)),
							  add_sphere(myVec3(-.75, .1, 2.25), .6, myVec3(.5, .223, .5)),
							  add_sphere(myVec3(-2.75, .1, 3.5), .6, myVec3(1., .572, .184)),
							  add_plane(myVec3(0., -.5, 0.), myVec3(0., 1., 0.))};

	int depth_max = 5; // Maximum number of light reflections.
	myVec3 col(0);	 // current color

	std::vector<myVec3> img(width * height); // img

	float r = float(width) / height;
	// Screen coordinates: x0, y0, x1, y1.
	float S[4] = {-1.f, -1.f / r + .25f, 1.f, 1.f / r + .25f};
	std::vector<float> spaced_W = linspace(S[0], S[2], width);
	std::vector<float> spaced_H = linspace(S[1], S[3], height);

	std::filebuf fb;
	fb.open("fig.ppm", std::ios::out);
	std::ostream os(&fb);
	os << "P3\n";
	os << height << " " << width << std::endl;
	os << "255\n";
		for (std::size_t i = 0; i < spaced_W.size(); i++)
		{
		if (i % 10 == 0)
			std::cout << i / float(width) * 100 << "%" << std::endl;

			for (std::size_t j = 0; j <spaced_H.size() ; j++)
	{
			col = myVec3(0);
			Q[0] = spaced_W[i];
			Q[1] = spaced_H[j];
			printf("Q: ");
			Q.print();
			auto D = (Q - O);
			D.normalize();
			int depth = 0;
			auto rayO = O;
			auto rayD = D;
			float reflection = 1.0f;

			while (depth < depth_max)
			{
				auto traced = trace_ray(rayO, rayD, scene);
				if (!traced)
				{
					break;
				}
				obj o = std::get<0>(*traced);
				myVec3 M, N, col_ray;
				std::tie(o, M, N, col_ray) = *traced;
				rayO = M + N * .0001;
				rayD = rayD - (N * rayD.dot(N) * 2);
				rayD.normalize();
				depth += 1;
				col = col + col_ray * reflection;
				reflection *= o.reflection ? o.reflection : 1.0f;
			}
			img[(height - j) * (width) + i] = col.clip();
			os << int(255 * col.clip()[0]) << " " << int(255 * col.clip()[1]) << " " << int(255 * col.clip()[2]) << " ";
			std::cout<<i<<" "<<j <<  col.clip()[0]  << " " <<  col.clip()[1]  << " " << col.clip()[2] <<std::endl;
		}
		os << "\n";
	}
	fb.close();

	std::cout << img[242][0] << " " << img[242][1] << " " << img[242][2] << std::endl;

	std::cout << "hello" << std::endl;
	return 0;
}

void Test_Vector_Class()
{
	myVec3 A(1, 2, 3);
	myVec3 B(3, 5, 6);
	auto c = A + B;
	for (int i = 0; i < 3; i++)
	{
		std::cout << c[i] << std::endl;
	}

	std::cout << A.dot(B) << std::endl;
}