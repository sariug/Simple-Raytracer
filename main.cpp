#include <iomanip> // std::setprecision
#include <limits>
#include <optional>
#include <algorithm>
#include <fstream> // std::filebuf
#include "utils.h"

std::optional<std::tuple<obj, myVec3, myVec3, myVec3>> trace_ray(myVec3 rayO, myVec3 rayD, std::vector<obj> &scene);

const int width = 400;
const int height = 300;

// Light position
myVec3 L(5, 5, -10);
myVec3 L_color(1); //White
float ambient = .05;
int specular_k = 50;
myVec3 O(0., 0.35, -1.); //Camera.
myVec3 Q(0);			 // Camera pointing to.



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
	os << std::setprecision(17);

	for (std::size_t i = 0; i < spaced_W.size(); i++)
	{
		if (i % 10 == 0)
			std::cout << i / float(width) * 100 << "%" << std::endl;

		for (std::size_t j = 0; j < spaced_H.size(); j++)
		{
			col = myVec3(0);
			Q[0] = spaced_W[i];
			Q[1] = spaced_H[j];
			auto D = (Q - O).normalize();;

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
				obj o;
				myVec3 M, N, col_ray;
				std::tie(o, M, N, col_ray) = *traced;

				rayO = M + N * .0001;
				rayD = (rayD - (N * rayD.dot(N) * 2)).normalize();
				depth += 1;
				col = col + col_ray * reflection;
				reflection *= o.reflection ? o.reflection : 1.0f;
			}
			os << int(255 * col.clip(0)) << " " << int(255 * col.clip(1)) << " " << int(255 * col.clip(2)) << " ";
		}
		os << "\n";
	}
	fb.close();
	std::cout<<"Finished."<<std::endl;
	return 0;
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

	auto toL = (L - M).normalize();
	auto toO = (O - M).normalize();


	// Shadow: find if the point is shadowed or not.
	std::vector<float> l;
	for (std::size_t cc = 0; cc < scene.size(); cc++)
	{
		if (cc != obj_idx)
			l.push_back(intersect(M + N * .0001, toL, scene[cc]));
	}
	if (l.size() > 0 && l[*std::min_element(l.begin(), l.end())] < inf)
		return std::nullopt;

	// Start computing the color.
	myVec3 col_ray(ambient);
	// Lambert shading (diffuse).
	col_ray = col_ray + color * scene[obj_idx].diffuse_c * std::max(N.dot(toL), 0.0f);
	// Blinn-Phong shading (specular).
	auto temp = (toL + toO).normalize();
	col_ray = col_ray + L_color * scene[obj_idx].specular_c * pow(std::max(N.dot(temp), 0.0f), specular_k);
	return std::make_tuple(scene[obj_idx], M, N, col_ray);
}
