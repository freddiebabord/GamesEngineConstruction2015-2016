#if !defined(GLASS_MATHS_RECT)
#define GLASS_MATHS_RECT

#include "Vector2.h"
#include "Vector4.h"

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
