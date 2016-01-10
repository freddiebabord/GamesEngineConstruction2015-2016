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
		Rect(double top_, double bottom_, double left_, double right_) :
			top(top_), bottom(bottom_), left(left_), right(right_){};
		~Rect(){};

		Rect* GetRect() { return this; };
		RectStates CheckState(const Rect& other);

	private:
		double top, bottom, left, right;

		
	};

}

#endif //GLASS_MATHS_RECT
