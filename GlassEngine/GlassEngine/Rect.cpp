#include "Rect.h"

namespace GlassEngine
{
	
	RectStates Rect::CheckState(const Rect& other)
	{
		if (right < other.right &&
			left > other.left &&
			top > other.top &&
			bottom < other.bottom)
		{
			return RectStates::Encapsulated;
		}
		else if (right > other.right &&
			left < other.left &&
			top < other.top &&
			bottom > other.bottom)
		{
			return RectStates::Surrounds;
		}
		else if (right > other.left ||
			left < other.right ||
			top < other.bottom ||
			bottom > other.top)
		{
			return RectStates::Separated;
		}
		else
			return RectStates::Overlaps;
	}
}
