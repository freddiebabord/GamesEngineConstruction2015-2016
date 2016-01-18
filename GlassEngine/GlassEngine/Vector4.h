#if !defined(GLASS_MATHS_VEC4)
#define GLASS_MATHS_VEC4

namespace GlassEngine
{
	// Templatised Vector 4 class
	template <typename T>
	class Vector4
	{
	public:
		//Constructors
		Vector4<T>() : x(0), y(0), z(0), w(0){};
		Vector4<T>(T ix) : x(ix), y(ix), z(ix), w(ix){};
		Vector4<T>(T ix, T iy, T iz, T iw) : x(ix), y(iy), z(iz), w(iw){};

		inline float Length()
		{
			return sqrt(x * x + y * y + z * z + w * w);
		}

		inline Vector4<T> Normalize()
		{
			Vector4<T> vector;
			float length = this->Length();

			if (length != 0)
			{
				vector.x = this->x / length;
				vector.y = this->y / length;
				vector.z = this->z / length;
				vector.w = this->w / length;
			}

			return vector;
		}

		inline Vector4<T> Distance(const Vector4<T>& p2)
		{
			float dx = p2.x - this->x;
			float dy = p2.y - this->y;
			float dz = p2.z - this->z;
			float dw = p2.w - this->w;
			return sqrt((float)(dx * dx + dy * dy + dz * dz + dw * dw));
		}

		//Operator overloads
		inline Vector4<T> operator=(const Vector4<T>& rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			this->w = rhs.w;
			return *this;
		}

		inline Vector4<T> operator+=(const Vector4<T>& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			this->w += rhs.w;
			return *this;
		}

		inline Vector4<T> operator-=(const Vector4<T>& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			this->w -= rhs.w;
			return *this;
		}

		inline Vector4<T> operator*=(const Vector4<T>& rhs)
		{
			this->x *= rhs.x;
			this->y *= rhs.y;
			this->z *= rhs.z;
			this->w *= rhs.w;
			return *this;
		}

		inline Vector4<T> operator/=(const Vector4<T>& rhs)
		{
			this->x /= rhs.x;
			this->y /= rhs.y;
			this->z /= rhs.z;
			this->w /= rhs.w;
			return *this;
		}

		inline Vector4<T> operator-(const Vector4<T>& rhs)
		{
			return Vector4<T>(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z, this->w - rhs.w);
		}

		inline Vector4<T> operator+(const Vector4<T>& rhs)
		{
			return Vector4<T>(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z, this->w + rhs.w);
		}

		inline Vector4<T> operator*(const Vector4<T>& rhs)
		{
			return Vector4<T>(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z, this->w * rhs.w);
		}

		inline Vector4<T> operator*(const T& rhs)
		{
			return Vector4<T>(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z, this->w * rhs.w);
		}

		inline Vector4<T> operator/(const Vector4<T>& rhs)
		{
			return Vector4<T>(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z, this->w / rhs.w);
		}

		// versions of vec4 variables
		union {
			struct {
				T x, y, z, w;
			};
			struct {
				T r, g, b, a;
			};
			struct {
				T left, right, top, bottom;
			};
		};
	};

	//Common types
#define Vec4 Vector4
#define Vec4i Vec4 <int>
#define Vec4d Vec4<double>
#define Vec4f Vec4<float>
#define Vec4ui Vec4<unsigned int>
#define Vec4UP  Vec4d(0,-1,0,1)
#define Vec4DOWN  Vec4d(0,1,0,1)
#define Vec4LEFT  Vec4d(-1,0,0,1)
#define Vec4RIGHT  Vec4d(1,0,0,1)

}

#endif //GLASS_MATHS_VEC2