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

		Vec2() : x(0), y(0) {}
		Vec2(float value) { x = y = value; }
		Vec2(float x, float y) : x(x), y(y) {}
	};

	struct Vec3
	{
		float x, y, z;

		Vec3() : x(0), y(0), z(0) {}
		Vec3(float value) { x = y = z = value; }
		Vec3(float x, float y) : x(x), y(y), z(0) {}
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	};

	struct Vec4
	{
		float x, y, z, w;

		Vec4() : x(0), y(0), z(0), w(0) {}
		Vec4(float value) { x = y = z = w = value; }
		Vec4(float x, float y) : x(x), y(y), z(0), w(0) {}
		Vec4(float x, float y, float z) : x(x), y(y), z(z), w(0) {}
		Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	};

	struct Vertex
	{
		Vec4 Position;
		Vec4 Color;
		Vec2 TexCoords;
		int TexID;
	};
}
