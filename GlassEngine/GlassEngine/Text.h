#if !defined (TEXTCOMPONENT)
#define TEXTCOMPONENT

#include "Component.h"

namespace GlassEngine{

	class GameObject;

	class Text : public Component
	{
	public:
		Text(SmartPtr<GameObject> parentObj) : Component(parentObj){ };
		Text() :Component(){};
		virtual ~Text(){};

		void SetContent(std::string content_){ content = content_; };
		std::string GetContent() const { return content; };


	private:
		std::string content;
	};

}

#endif //TEXTCOMPONENT