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
		Vector2<T>(): x(0), y(0){};
		Vector2<T>(T ix) : x(ix), y(ix){};
		Vector2<T>(T ix, T iy) : x(ix), y(iy){};

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