#if !defined(GLASS_MATHS)
#define GLASS_MATHS

#include "MathConsts.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Rect.h"

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
#endif