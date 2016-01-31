#pragma once
#include <math.h>
#include <random>
#include <cstdio>

//#define FORCE_RELOAD

const int MAP_SIZE = 256;

struct Texel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

Texel* texels;

bool in_box(int coord) {
	return (coord > 128 - 20 && coord < 128 + 20);
}

bool in_box_2(int coord) {
	return (coord > 128 - 10 && coord < 128 + 10);
}

int min(int a, int b) {
	return a < b ? a : b;
}

bool solid_at_coord(int x, int y, int z) {
	//std::cout << "col test @ " << x << ", " << y << ", " << z << std::endl;
	return texels[z*(MAP_SIZE * MAP_SIZE) + y*(MAP_SIZE) +x].a > 0x08;
}

bool solid_at(float x, float y, float z) {
	return solid_at_coord((x - floor(x)) * MAP_SIZE,
		(y - floor(y)) * MAP_SIZE,
		(z - floor(z)) * MAP_SIZE);
}

void inline set_texel(int x, int y, int z, Texel value) {
	texels[x*(MAP_SIZE * MAP_SIZE) + y*(MAP_SIZE) +z] = value;
}

void inline set_texel_color(int x, int y, int z, Texel value) {
	auto pos = x*(MAP_SIZE * MAP_SIZE) + y*(MAP_SIZE) +z;
	texels[pos].r = value.r;
	texels[pos].g = value.g;
	texels[pos].b = value.b;
}
void inline set_texel_alpha(int x, int y, int z, Texel value) {
	texels[x*(MAP_SIZE * MAP_SIZE) + y*(MAP_SIZE) +z].a = value.a;
}

void add_box(int x0, int y0, int z0, int x1, int y1, int z1, Texel value) {
	for (int x = x0; x < x1; x++) {
		for (int y = y0; y < y1; y++) {
			for (int z = z0; z < z1; z++) {
				if (x == x0 || x == x1-1 || y == y0 || y == y1-1 || z == z0 || z == z1-1)
					set_texel_color(x, y, z, value);
				else
					set_texel(x, y, z, value);
			}
		}
	}
}

unsigned char* generateTexels(bool city) {
	texels = new Texel[MAP_SIZE * MAP_SIZE * MAP_SIZE];

#ifndef FORCE_RELOAD
	FILE* file;
	auto err = fopen_s(&file, "map.data", "rb");
	if (err == 0) {
		std::cout << "map.data found, using that." << std::endl;
		fread(texels, 1, MAP_SIZE * MAP_SIZE * MAP_SIZE * sizeof(Texel), file);
		fclose(file);
	} else
#endif
	{
		std::cout << "map.data not found, generating..." << std::endl;

		for (int x = 0; x < MAP_SIZE; x++) {
			for (int y = 0; y < MAP_SIZE; y++) {
				for (int z = 0; z < MAP_SIZE; z++) {
					unsigned char alpha = 0x00; //y%2==0?0x00:0xff;
					//alpha = 0x10 * (x / (float) MAP_SIZE);
					if (y < sin(x / (float) MAP_SIZE * 2 * 3.14f) * 20 + 40) alpha = 0xff;
					if (in_box(x) && in_box(y) && in_box(z)) alpha = 0x08;
					if (in_box_2(x) && in_box_2(y) && in_box_2(z)) alpha = 0xff;
					//if (x < 128 && y < 128) alpha = 0x00;
					texels[x*(MAP_SIZE * MAP_SIZE) + y*(MAP_SIZE) +z] = { static_cast<unsigned char>(min(0xff, 128 * sin(x / 50.0f) + 128 /*+ (rand() % 40)*/)),
						static_cast<unsigned char>(128 * sin(y / 20.0f) + 128),
						static_cast<unsigned char>(128 * sin(z / 200.0f) + 128), alpha };
					/*unsigned char red = x < 128 ? 0xff : 0x00;
					unsigned char green = y < 128 ? 0xff : 0x00;
					unsigned char blue = z < 128 ? 0xff : 0x00;
					texels[x*(MAP_SIZE * MAP_SIZE) + y*(MAP_SIZE) +z] = { red, green, blue, alpha };*/
				}
			}
		}
		if (city)
			for (int i = 0; i < 100; i++) {
				int x = rand() % (256-26);
				int y = rand() % (256 - 26);
				int z = sin(x / (float) MAP_SIZE * 2 * 3.14f) * 20 + 30;
				int w = rand() % 18 + 8;
				int h = rand() % 18 + 8;
				int d = rand() % 40 + 16;
				unsigned char r = rand() % 256;
				unsigned char g = rand() % 256;
				unsigned char b = rand() % 256;
				unsigned char a = rand() % 256;
				add_box(x, z, y, x + w, z + d, y + h, { r, g, b, a });
			}

		FILE* file;
		fopen_s(&file, "map.data", "wb");
		fwrite(texels, 1, MAP_SIZE * MAP_SIZE * MAP_SIZE * sizeof(Texel), file);
		fclose(file);
	}

	return (unsigned char*) texels;
}