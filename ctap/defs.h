#pragma once
#include "libs.h"


struct Time
{
	//frame 1, frame 2 to create constant frame rate
	int fr1, fr2;
};

struct Keys
{
	// player look left, right, up, down
	int j, k, h, l;

	//player move forwards, backwards, left, right
	int w, s, a, d;

	//player move up, down
	int z, x;
};

struct Math
{
	float cos[360];
	float sin[360];
};

struct Player
{
	//player cordinates
	int x;
	int y;
	int z;

	int a;
	int l;
};