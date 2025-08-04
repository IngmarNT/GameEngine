#pragma once
#include "raylib.h"
#include "raymath.h"
#include "JoltSetup.h"
#include <cmath>

namespace MyEngine {

	struct Vec2;
	struct Vec3;
	struct Vec4;
	class Quat;
	class GameTransform;
	enum class PhysicsShapeType;
	struct PhysicsBodyConfig;

	struct Vec2
	{
		float x, y;

		Vec2() : x(0), y(0) {}
		Vec2(float _x, float _y) : x(_x), y(_y) {}
		Vec2(const Vector2& v) : x(v.x), y(v.y) {}

		operator Vector2() const { return { x, y }; }

		Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
		Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
		Vec2 operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }
		Vec2 operator/(float scalar) const { return Vec2(x / scalar, y / scalar); }
		bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
		bool operator!=(const Vec2& other) const { return !(*this == other); }

		float Magnitude() const { return sqrtf(x * x + y * y); }
		float Dot(const Vec2& other) const { return x * other.x + y * other.y; }

		Vec2 Normalized() const
		{
			float mag = Magnitude();
			return mag > 0 ? (*this) / mag : Vec2();
		}
	};

	struct Vec3
	{
		float x, y, z;

		Vec3() : x(0), y(0), z(0) {}
		Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
		Vec3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

		operator Vector3() const { return { x, y, z }; }

		Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
		Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
		Vec3 operator*(float scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
		Vec3 operator/(float scalar) const { return Vec3(x / scalar, y / scalar, z / scalar); }
		bool operator==(const Vec3& other) const { return x == other.x && y == other.y && z == other.z; }
		bool operator!=(const Vec3& other) const { return !(*this == other); }
		Vec3& operator+=(const Vec3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		Vec3& operator-=(const Vec3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		Vec3& operator*=(float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		// Component-wise multiplication.
		Vec3 Multiply(const Vec3& other) const { return Vec3(x * other.x, y * other.y, z * other.z); }

		float Magnitude() const { return sqrtf(x * x + y * y + z * z); }
		float Dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }

		static Vec3 Lerp(const Vec3& v1, const Vec3& v2, float amount) { return Vec3(Vector3Lerp(v1, v2, amount)); }

		Vec3 Normalized() const
		{
			float mag = Magnitude();
			return mag > 0 ? (*this) / mag : Vec3();
		}
	};

	struct Vec4
	{
		float x, y, z, w;

		Vec4() : x(0), y(0), z(0), w(0) {}
		Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
		Vec4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

		operator Vector4() const { return { x, y, z, w }; }

		Vec4 operator+(const Vec4& other) const { return Vec4(x + other.x, y + other.y, z + other.z, w + other.w); }
		Vec4 operator-(const Vec4& other) const { return Vec4(x - other.x, y - other.y, z - other.z, w - other.w); }
		Vec4 operator*(float scalar) const { return Vec4(x * scalar, y * scalar, z * scalar, w * scalar); }
		Vec4 operator/(float scalar) const { return Vec4(x / scalar, y / scalar, z / scalar, w * scalar); }
		bool operator==(const Vec4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool operator!=(const Vec4& other) const { return !(*this == other); }

		float Magnitude() const { return sqrtf(x * x + y * y + z * z + w * w); }
		float Dot(const Vec4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }

		Vec4 Normalized() const
		{
			float mag = Magnitude();
			return mag > 0 ? (*this) / mag : Vec4();
		}
	};

	class Quat
	{
	private:
		Quaternion data;

	public:
		Quat() : data{ 0, 0, 0, 1 } {}
		Quat(float x, float y, float z, float w) : data{ x, y, z, w } {}
		Quat(Quaternion _q) : data(_q) {}

		operator Quaternion() const { return data; }

		Quaternion Get() const
		{
			return data;
		}

		void Set(float _x, float _y, float _z, float _w)
		{
			data.x = _x;
			data.y = _y;
			data.z = _z;
			data.w = _w;
		}

		// RAYLIB FUNCTION WRAPPERS

		static Quat FromEuler(float pitch, float yaw, float roll)
		{
			return Quat(QuaternionFromEuler(pitch * DEG2RAD, yaw * DEG2RAD, roll * DEG2RAD));
		}

		static Quat Lerp(Quat q1, Quat q2, float amount) { return Quat(QuaternionLerp(q1.data, q2.data, amount)); }
		static Quat Slerp(Quat q1, Quat q2, float amount) { return Quat(QuaternionSlerp(q1.data, q2.data, amount)); }

		static Quat Identity() { return Quat(0, 0, 0, 1); }

		Quat operator+(const Quat& other) const { return Quat(QuaternionAdd(data, other.data)); }
		Quat operator-(const Quat& other) const { return Quat(QuaternionSubtract(data, other.data)); }
		Quat operator*(const Quat& other) const { return Quat(QuaternionMultiply(data, other.data)); }
		Quat& operator*=(const Quat& other)
		{
			data = QuaternionMultiply(data, other.data);
			return *this;
		}

		void Normalize() { data = QuaternionNormalize(data); }
		Quat Inverse() const { return Quat(QuaternionInvert(data)); }
		Matrix ToMatrix() const { return QuaternionToMatrix(data); }
		Vec3 RotateVector(Vec3 vec) const { return Vector3RotateByQuaternion(vec, data); }
	};

	class GameTransform
	{
	public:
		Vec3 position;
		Quat rotation;
		Vec3 scale;

		Vec3 previousPosition;
		Quat previousRotation;

		void SavePrevious() {
			previousPosition = position;
			previousRotation = rotation;
		}

		Vec3 GetInterpolatedPosition(float alpha) const {
			return Vec3::Lerp(previousPosition, position, alpha);
		}

		Quaternion GetInterpolatedRotation(float alpha) const {
			return Quat::Slerp(previousRotation, rotation, alpha);
		}

		GameTransform() : position(Vec3(0, 0, 0)), rotation(Quat(0, 0, 0, 1)), scale(Vec3(1, 1, 1)) {}

		void Translate(Vec3 delta) { position += delta; }
		void Rotate(Quat q) { rotation *= q; }
		void Scale(Vec3 s) { scale = scale.Multiply(s); }
	};

	enum class PhysicsShapeType
	{
		Box,
		Sphere,
		Capsule,
		Custom
	};

	struct PhysicsBodyConfig
	{
		PhysicsShapeType shapeType = PhysicsShapeType::Box;
		JPH::EActivation activationType = JPH::EActivation::Activate;
		JPH::ObjectLayer layer = Layers::MOVING;
		JPH::Vec3 dimensions = JPH::Vec3(1.0f, 1.0f, 1.0f);
		std::vector<Vec3> meshVertices;
		float radius = 1.0f; // used for spheres, capsules
		float height = 2.0f; // used for capsules
		float mass = 1.0f;
		JPH::EMotionType motionType = JPH::EMotionType::Dynamic;
		float friction = 0.7f;
		float restitution = 0.2f;
		JPH::uint64 userData = 0;
		JPH::EAllowedDOFs dof = JPH::EAllowedDOFs::All;
	};

	enum class DefaultShapes 
	{
		Box,
		Sphere,
		Torus,
		Cone
	};

	static std::vector<Vec3> GetVerticesFromModel(const Model& model)
	{
		std::vector<Vec3> points;

		for (int i = 0; i < model.meshCount; ++i)
		{
			const Mesh& mesh = model.meshes[i];

			if (!mesh.vertices || mesh.vertexCount == 0)
				continue;

			for (int v = 0; v < mesh.vertexCount; ++v)
			{
				float x = mesh.vertices[v * 3 + 0];
				float y = mesh.vertices[v * 3 + 1];
				float z = mesh.vertices[v * 3 + 2];

				points.emplace_back(x, y, z);
			}
		}

		return points;
	}
}