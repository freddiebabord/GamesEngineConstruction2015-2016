#include "GlassUtilities.h"

namespace GlassEngine
{

	int Rect::Width() const
	{
		return right - left;
	}

	int Rect::Height() const
	{
		return bottom - top;
	}

	void Rect::Translate(int x, int y)
	{
		left += x;
		right += x;
		top += y;
		bottom += y;
	}

	void Rect::ClipTo(const Rect &other)
	{
		left = MAX(other.left, left);
		right = MIN(other.right, right);
		top = MAX(other.top, top);
		bottom = MIN(other.bottom, bottom);
	}

	bool Rect::Intersects(const Rect &other)
	{
		if (left>other.right || right<other.left || top>other.bottom || bottom<other.top)
			return false;
		return true;
	}

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

	RectStates Rect::CheckState(const Vec2i& pos)
	{
		if (right > pos.x && left < pos.x && top > pos.y && bottom < pos.y)
			return RectStates::Encapsulated;
		else
			return RectStates::Separated;
	}
}
