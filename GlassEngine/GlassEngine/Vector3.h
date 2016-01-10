#if !defined(GLASS_MATHS_VEC3)
#define GLASS_MATHS_VEC3

namespace GlassEngine
{
	// Templatised Vector 3 class
	template <typename T>
	class Vector3
	{
	public:
		//Constructors
		Vector3<T>() : x(0), y(0), z(0){};
		Vector3<T>(T ix) : x(ix), y(ix), z(ix){};
		Vector3<T>(Vector2<T> ixy, T iz) : x(ixy.x), y(ixy.y), z(iz){};
		Vector3<T>(T ix, T iy, T iz) : x(ix), y(iy), z(iz){};

		//Operator overloads
		inline Vec3<T> operator=(const Vec3<T>& rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			return *this;
		}

		inline Vec3<T> operator+=(const Vec3<T>& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			return *this;
		}

		inline Vec3<T> operator-=(const Vec3<T>& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			return *this;
		}

		inline Vec3<T> operator*=(const Vec3<T>& rhs)
		{
			this->x *= rhs.x;
			this->y *= rhs.y;
			this->z *= rhs.z;
			return *this;
		}

		inline Vec3<T> operator/=(const Vec3<T>& rhs)
		{
			this->x /= rhs.x;
			this->y /= rhs.y;
			this->z /= rhs.z;
			return *this;
		}

		inline Vec3<T> operator-(const Vec3<T>& rhs)
		{
			return Vec3<T>(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
		}

		inline Vec3<T> operator+(const Vec3<T>& rhs)
		{
			return Vec3<T>(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
		}

		inline Vec3<T> operator*(const Vec3<T>& rhs)
		{
			return Vec3<T>(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
		}

		inline Vec3<T> operator*(const T& rhs)
		{
			return Vec3<T>(this->x * rhs, this->y * rhs, this->z * rhs);
		}

		inline Vec3<T> operator/(const Vec3<T>& rhs)
		{
			return Vec3<T>(this->x / rhs.x, this->y / rhs.y, this->z * rhs.z);
		}

		// versions of vec3 variables
		union {
			struct {
				T x, y, z;
			};
			struct {
				T r, g, b;
			};
			struct {
				T width, height, depth;
			};
		};
	};

//Common types
#define Vec3 Vector3
#define Vec3i Vec3 <int>
#define Vec3d Vec3<double>
#define Vec3f Vec3<float>
#define Vec3ui Vec3<unsigned int>
#define Vec3UP  Vec3d(0,-1,0)
#define Vec3DOWN  Vec3d(0,1,0)
#define Vec3LEFT  Vec3d(-1,0,0)
#define Vec3RIGHT  Vec3d(1,0,0)

}

#endif //GLASS_MATHS_VEC2