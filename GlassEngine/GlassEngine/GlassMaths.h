#if !defined GLASS_MATHS
#define GLASS_MATHS

struct Vector4
{
	double x, y, z, w = 0.0;
};


struct Vector3
{
	double x, y, z = 0.0;
	
	Vector3():x(0.0),y(0.0), z(0.0){};
	Vector3(double ix, double iy, double iz) : x(ix), y(iy), z(iz){};

	
};

inline Vector3 operator +(const Vector3 &lhs, const Vector3 &rhs)
{
	Vector3 tmpVector = Vector3(0.0, 0.0, 0.0);
	tmpVector.x = lhs.x + rhs.x;
	tmpVector.y = lhs.x + rhs.y;
	tmpVector.z = lhs.x + rhs.z;

	return tmpVector;
};

inline void operator +=(Vector3 &lhs, const Vector3 &rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
};

inline Vector3 operator -(const Vector3 &lhs, const Vector3 &rhs)
{
	Vector3 tmpVector = Vector3(0, 0, 0);
	tmpVector.x = lhs.x - rhs.x;
	tmpVector.y = lhs.x - rhs.y;
	tmpVector.z = lhs.x - rhs.z;

	return tmpVector;
};

inline void operator -=(Vector3 &lhs, const Vector3 &rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
};

struct Vector2
{
	int x, y = 0;
	
	Vector2() : x(0), y(0){};
	Vector2(int ix, int iy) : x(ix), y(iy){};
	
};


class Transform
{
public:
	Transform(){};
	~Transform(){};

	Vector3 position;
	Vector3 rotaion;
	Vector3 scale;

private:

};

#endif