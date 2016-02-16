#if !defined (TEXTCOMPONENT)
#define TEXTCOMPONENT

#include "precomp.h"
#include "UIObject.h"
#include "Font.h"

namespace GlassEngine{

	class Text : public UIObject
	{
	public:
		Text() : UIObject(){};
		virtual ~Text(){};

		void Content(std::string content_){ content = content_; };
		std::string Content() const { return content; };

		Font GetFont(){ return textFont; };
		void SetFont(const Font newFont){ textFont = newFont; };

	private:
		std::string content;
		Font textFont;
	};

}

#endif //TEXTCOMPONENT