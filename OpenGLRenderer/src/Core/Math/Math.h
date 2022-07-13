#pragma once

#include <glm/glm.hpp>

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

		inline Vec2 Negate() { return { -x, -y }; }

		inline float Magnitude() { return sqrt((x * x) + (y * y)); }

		inline void Normalize() { *this /= Magnitude(); }

		inline static float Cross(Vec2 a, Vec2 b) { return (a.x * b.y) - (b.x * a.y); }

		inline static float Dot(Vec2 a, Vec2 b) { return (a.x * b.x) + (a.y * b.y); }

#pragma region Add
		Vec2 operator + (const Vec2& v)
		{
			Vec2 ret { x + v.x, y + v.y };
			return ret;
		}

		void operator += (const Vec2& v)
		{
			x += v.x;
			y += v.y;
		}

		Vec2 operator + (const float& v)
		{
			Vec2 ret { x + v, y + v };
			return ret;
		}

		void operator += (const float& v)
		{
			x += v;
			y += v;
		}

		Vec2 operator + (const glm::vec2& v)
		{
			Vec2 ret { x + v.x, y + v.y };
			return ret;
		}

		void operator += (const glm::vec2& v)
		{
			x += v.x;
			y += v.y;
		}
#pragma endregion

#pragma region Subtract
		Vec2 operator - (const Vec2& v)
		{
			Vec2 ret { x - v.x, y - v.y };
			return ret;
		}

		void operator -= (const Vec2& v)
		{
			x -= v.x;
			y -= v.y;
		}

		Vec2 operator - (const float& v)
		{
			Vec2 ret { x - v, y - v };
			return ret;
		}

		void operator -= (const float& v)
		{
			x -= v;
			y -= v;
		}

		Vec2 operator - (const glm::vec2& v)
		{
			Vec2 ret { x - v.x, y - v.y };
			return ret;
		}

		void operator -= (const glm::vec2& v)
		{
			x -= v.x;
			y -= v.y;
		}
#pragma endregion

#pragma region Divide
		Vec2 operator / (const Vec2& v)
		{
			Vec2 ret { x / v.x, y / v.y };
			return ret;
		}

		void operator /= (const Vec2& v)
		{
			x /= v.x;
			y /= v.y;
		}

		Vec2 operator / (const float& v)
		{
			Vec2 ret { x / v, y / v };
			return ret;
		}

		void operator /= (const float& v)
		{
			x /= v;
			y /= v;
		}
#pragma endregion
	};

	struct Vec3
	{
		float x, y, z;

		Vec3() : x(0), y(0), z(0) {}
		Vec3(float value) { x = y = z = value; }
		Vec3(float x, float y) : x(x), y(y), z(0) {}
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

		inline Vec3 Negate() { return { -x, -y, -z }; }

		inline float Magnitude() { return sqrt((x * x) + (y * y) + (z * z)); }

		inline void Normalize() { *this /= Magnitude(); }

		static Vec3 Cross(Vec3 a, Vec3 b)
		{
			return Vec3(
				(a.y * b.z) - (a.z * b.y),
				(a.z * b.x) - (a.x * b.z),
				(a.x * b.y) - (a.y * b.x)
			);
		}

		inline static float Dot(Vec3 a, Vec3 b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }

#pragma region Add
		Vec3 operator + (const Vec3& v)
		{
			Vec3 ret { x + v.x, y + v.y, z + v.z };
			return ret;
		}

		void operator += (const Vec3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
		}

		Vec3 operator + (const float& v)
		{
			Vec3 ret { x + v, y + v, z + v };
			return ret;
		}

		void operator += (const float& v)
		{
			x += v;
			y += v;
			z += v;
		}

		Vec3 operator + (const glm::vec3& v)
		{
			Vec3 ret { x + v.x, y + v.y, z + v.z };
			return ret;
		}

		void operator += (const glm::vec3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
		}
#pragma endregion

#pragma region Subtract
		Vec3 operator - (const Vec3& v)
		{
			Vec3 ret { x - v.x, y - v.y, z - v.z };
			return ret;
		}

		void operator -= (const Vec3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}

		Vec3 operator - (const float& v)
		{
			Vec3 ret { x - v, y - v, z - v };
			return ret;
		}

		void operator -= (const float& v)
		{
			x -= v;
			y -= v;
			z -= v;
		}

		Vec3 operator - (const glm::vec3& v)
		{
			Vec3 ret { x = v.x, y - v.y, z - v.z };
			return ret;
		}

		void operator -= (const glm::vec3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}
#pragma endregion

#pragma region Divide
		Vec3 operator / (const Vec3& v)
		{
			Vec3 ret { x / v.x, y / v.y, z / v.z };
			return ret;
		}

		void operator /= (const Vec3& v)
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
		}

		Vec3 operator / (const float& v)
		{
			Vec3 ret { x / v, y / v, z / v };
			return ret;
		}

		void operator /= (const float& v)
		{
			x /= v;
			y /= v;
			z /= v;
		}
#pragma endregion
	};

	struct Vec4
	{
		float x, y, z, w;

		Vec4() : x(0), y(0), z(0), w(0) {}
		Vec4(float value) { x = y = z = w = value; }
		Vec4(float x, float y) : x(x), y(y), z(0), w(0) {}
		Vec4(float x, float y, float z) : x(x), y(y), z(z), w(0) {}
		Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

		inline Vec4 Negate() { return { -x, -y, -z, -w }; }

		inline float Magnitude() { return sqrt((x * x) + (y * y) + (z * z) + (w * w)); }

		inline void Normalize() { *this /= Magnitude(); }

		inline static float Dot(Vec4 a, Vec4 b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w); }

#pragma region Add
		Vec4 operator + (const Vec4& v)
		{
			Vec4 ret { x + v.x, y + v.y, z + v.z, w + v.w };
			return ret;
		}

		void operator += (const Vec4& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
		}

		Vec4 operator + (const float& v)
		{
			Vec4 ret { x + v, y + v, z + v, w + v };
			return ret;
		}

		void operator += (const float& v)
		{
			x += v;
			y += v;
			z += v;
			w += v;
		}

		Vec4 operator + (const glm::vec4& v)
		{
			Vec4 ret { x + v.x, y + v.y, z + v.z, w + v.w };
			return ret;
		}

		void operator += (const glm::vec4& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
		}
#pragma endregion

#pragma region Subtract
		Vec4 operator - (const Vec4& v)
		{
			Vec4 ret { x - v.x, y - v.y, z - v.z, w - v.w };
			return ret;
		}

		void operator -= (const Vec4& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
		}

		Vec4 operator - (const float& v)
		{
			Vec4 ret { x - v, y - v, z - v, w - v };
			return ret;
		}

		void operator -= (const float& v)
		{
			x -= v;
			y -= v;
			z -= v;
			w -= v;
		}

		Vec4 operator - (const glm::vec4& v)
		{
			Vec4 ret { x - v.x, y - v.y, z - v.z, w - v.w };
			return ret;
		}

		void operator -= (const glm::vec4& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
		}
#pragma endregion

#pragma region Divide
		Vec4 operator / (const Vec4& v)
		{
			Vec4 ret { x / v.x, y / v.y, z / v.z, w / v.w };
			return ret;
		}

		void operator /= (const Vec4& v)
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			w /= v.w;
		}

		Vec4 operator / (const float& v)
		{
			Vec4 ret { x / v, y / v, z / v, w / v };
			return ret;
		}

		void operator /= (const float& v)
		{
			x /= v;
			y /= v;
			z /= v;
			w /= v;
		}
#pragma endregion

	};

	struct Vertex
	{
		Vec3 Position;
		Vec4 PixelPosition;
		Vec4 Color;
		Vec2 TexCoords;
		int TexID;
		Vec3 Normal;
	};
}
