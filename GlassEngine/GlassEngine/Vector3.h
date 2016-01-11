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

		inline float Length()
		{
			return sqrt(x * x + y * y + z * z);
		}

		inline Vector3<T> Normalize()
		{
			Vector3<T> vector;
			float length = this->Length();

			if (length != 0)
			{
				vector.x = this->x / length;
				vector.y = this->y / length;
				vector.z = this->z / length;
			}

			return vector;
		}

		inline Vector3<T> Distance(const Vector3<T>& p2)
		{
			float dx = p2.x - this->x;
			float dy = p2.y - this->y;
			float dz = p2.z - this->z;

			return sqrt((float)(dx * dx + dy * dy + dz * dz));
		}

		//Operator overloads
		inline Vector3<T> operator=(const Vector3<T>& rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			return *this;
		}

		inline Vector3<T> operator+=(const Vector3<T>& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			return *this;
		}

		inline Vector3<T> operator-=(const Vector3<T>& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			return *this;
		}

		inline Vector3<T> operator*=(const Vector3<T>& rhs)
		{
			this->x *= rhs.x;
			this->y *= rhs.y;
			this->z *= rhs.z;
			return *this;
		}

		inline Vector3<T> operator/=(const Vector3<T>& rhs)
		{
			this->x /= rhs.x;
			this->y /= rhs.y;
			this->z /= rhs.z;
			return *this;
		}

		inline Vector3<T> operator-(const Vector3<T>& rhs)
		{
			return Vector3<T>(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
		}

		inline Vector3<T> operator+(const Vector3<T>& rhs)
		{
			return Vector3<T>(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
		}

		inline Vector3<T> operator*(const Vector3<T>& rhs)
		{
			return Vector3<T>(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
		}

		inline Vector3<T> operator*(const T& rhs)
		{
			return Vector3<T>(this->x * rhs, this->y * rhs, this->z * rhs);
		}

		inline Vector3<T> operator/(const Vector3<T>& rhs)
		{
			return Vector3<T>(this->x / rhs.x, this->y / rhs.y, this->z * rhs.z);
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