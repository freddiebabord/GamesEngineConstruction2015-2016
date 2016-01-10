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

#endif