#pragma once

namespace core
{
	static int gcd(int a, int b)
	{
		return b == 0 ? a : gcd(b, a % b);
	}

	struct Vec2
	{
		float x, y;
	};

	struct Vec3
	{
		float x, y, z;
	};

	struct Vec4
	{
		float x, y, z, w;
	};

	struct Vertex
	{
		Vec3 Position;
		Vec4 Color;
		Vec2 TexCoords;
		int TexID;
	};
}
