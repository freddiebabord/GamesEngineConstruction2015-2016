#if !defined (GLASSUTILITES)
#define GLASSUTILITIES

#if !defined (GLASS_MATHS_CONSTS) 
#define GLASS_MATHS_CONSTS

#define PI 3.14159265358979323846264338327950288419716939937510582;
#define INF = std::numeric_limits<unsigned double>::infinity();
#define NINFINITY = -std::numeric_limits<unsigned double>::infinity();

template <typename T>
inline T Clamp(T value, T max, T min)
{
	if (value > max)
		return max;
	if (value < min)
		return min;

	return value;
}

template <typename T>
inline T Rad2Deg(T rad)
{
	return (T)(rad * (180 / PI));
}

template <typename T>
inline T Deg2Rad(T deg)
{
	return (T)(deg * (PI / 180));
}


template <typename T>
inline T lerp(T a, T b, float s)
{
	return a + (b - a) * s;
}

#endif //GLASSMATHSCONSTANTS



#if !defined(GLASS_MATHS_VEC2)
#define GLASS_MATHS_VEC2

namespace GlassEngine
{

#define Vec2 Vector2

	// Templatised Vector 2 class
	template <typename T>
	class Vector2
	{
	public:
		//Constructors
		Vector2<T>() : x(0), y(0){};
		Vector2<T>(T ix) : x(ix), y(ix){};
		Vector2<T>(T ix, T iy) : x(ix), y(iy){};

		inline T Length()
		{
			return sqrt(x * x + y * y);
		}

		inline Vec2<float> Normalize()
		{
			Vec2<float> vector;
			float length = this->Length();

			if (length != 0)
			{
				vector.x = this->x / length;
				vector.y = this->y / length;
			}

			return vector;
		}

		//Operator overloads
		inline Vec2<T> operator=(const Vec2<T>& rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
			return *this;
		}

		inline Vec2<T> operator+=(const Vec2<T>& rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			return *this;
		}

		inline Vec2<T> operator-=(const Vec2<T>& rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			return *this;
		}

		inline Vec2<T> operator*=(const Vec2<T>& rhs)
		{
			this->x *= rhs.x;
			this->y *= rhs.y;
			return *this;
		}

		inline Vec2<T> operator/=(const Vec2<T>& rhs)
		{
			this->x /= rhs.x;
			this->y /= rhs.y;
			return *this;
		}

		inline Vec2<T> operator-(const Vec2<T>& rhs)
		{
			return Vec2<T>(this->x - rhs.x, this->y - rhs.y);
		}

		inline Vec2<T> operator+(const Vec2<T>& rhs)
		{
			return Vec2<T>(this->x + rhs.x, this->y + rhs.y);
		}

		inline Vec2<T> operator*(const Vec2<T>& rhs)
		{
			return Vec2<T>(this->x * rhs.x, this->y * rhs.y);
		}

		inline Vec2<T> operator/(const Vec2<T>& rhs)
		{
			return Vec2<T>(this->x / rhs.x, this->y / rhs.y);
		}

		// versions of vec2 variables
		union {
			struct {
				T x, y;
			};
			struct {
				T r, g;
			};
			struct
			{
				T width, height;
			};
			struct
			{
				T start, end;
			};
		};
	};

	//Common types
#define Vec2i Vec2<int>
#define Vec2d Vec2<double>
#define Vec2f Vec2<float>
#define Vec2ui Vec2<unsigned int>
}

#endif //GLASS_MATHS_VEC2


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

		inline bool operator > (const Vector3<T>& rhs)
		{
			if (this->Length() > rhs->Length())
				return true;

			return false;
		}

		inline bool operator >= (const Vector3<T>& rhs)
		{
			if (this->Length() >= rhs->Length())
				return true;

			return false;
		}

		inline bool operator < (const Vector3<T>& rhs)
		{
			if (this->Length() > rhs->Length())
				return true;

			return false;
		}

		inline bool operator <= (const Vector3<T>& rhs)
		{
			if (this->Length() >= rhs->Length())
				return true;

			return false;
		}

		inline bool operator != (const Vector3<T>& rhs)
		{
			if (this->x == rhs.x)
			{
				if (this->y == rhs.y)
				{
					if (this->z == rhs.z)
					{
						return true;
					}
				}
			}

			return false;
		}

		inline bool operator != (const T& rhs)
		{
			if (this->x == rhs)
			{
				if (this->y == rhs)
				{
					if (this->z == rhs)
					{
						return true;
					}
				}
			}

			return false;
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

#endif //GLASS_MATHS_VEC3

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
				T top, bottom, left, right;
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

#endif //GLASS_MATHS_VEC4


#if !defined(GLASS_MATHS_RECT)
#define GLASS_MATHS_RECT

namespace GlassEngine
{
	enum RectStates
	{
		Overlaps,
		Encapsulated,
		Surrounds,
		Separated
	};

	class Rect
	{
	public:
		Rect(){};
		Rect(int top_, int bottom_, int left_, int right_) :
			top(top_), bottom(bottom_), left(left_), right(right_){};
		~Rect(){};

		Rect* GetRect() { return this; };
		RectStates CheckState(const Rect& other);
		RectStates CheckState(const Vec2i& pos);
		Vec4i  GetRectDims(){ return Vec4i(top, bottom, left, right); };
	private:
		int top, bottom, left, right;


	};

}

#endif //GLASS_MATHS_RECT

#if !defined (GLASSDISTANCE)
#define GLASSDISTANCE
namespace GlassEngine
{
	template <typename T>
	T Distance(const Vec3<T>& p1, const Vec3<T>& p2)
	{
		T dx = p2.x - p1.x;
		T dy = p2.y - p1.y;
		T dz = p2.z - p1.z;

		return (T)sqrt((T)(dx * dx + dy * dy + dz * dz));
	}

	template <typename T>
	T Distance(const Vec2<T>& p1, const Vec2<T>& p2)
	{
		T dx = p2.x - p1.x;
		T dy = p2.y - p1.y;

		return (T)sqrt((T)(dx * dx + dy * dy));
	}

}
#endif //GLASSDISTANCE

#endif