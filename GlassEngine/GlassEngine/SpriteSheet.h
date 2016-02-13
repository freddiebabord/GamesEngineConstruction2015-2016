#if !defined (SPRITESHEET)
#define SPRITESHEET

#include "Component.h"

namespace GlassEngine{

	class SpriteSheet : public Component
	{
	public:
		SpriteSheet(SmartPtr<GameObject> parentObj) :Component(parentObj){ id = SpriteSheetC; };
		SpriteSheet(SmartPtr<GameObject> parentObj, const std::string& path) :Component(parentObj){ id = SpriteSheetC;  LoadSprite(path); name = path; };
		SpriteSheet(SmartPtr<GameObject> parentObj, const std::string& path, Vec2i idvSpriteSize) : Component(parentObj){
			id = SpriteSheetC;
			LoadSprite(path);
			individualSpriteSize = idvSpriteSize;
			name = path;
		};
		SpriteSheet(const std::string& path, Vec2i idvSpriteSize) : Component(){
			id = SpriteSheetC;
			LoadSprite(path);
			individualSpriteSize = idvSpriteSize;
			name = path;
		};
		SpriteSheet() : Component(){ id = SpriteSheetC; };
		virtual ~SpriteSheet();
		
		BYTE* GetImage();
		bool LoadSprite(std::string path);
		//Gets the Sprites dimentions in a Vec2 (width, height)
		Vec2i GetSpriteDims();
		
		const int GetRowCount(){ return rowCount; };
		const Vec2i GetIdvSpriteDims();
		void SetIdvSpriteSize(const Vec2i size){ individualSpriteSize = size; };
				
		void ID(const int id_){ id = id_; };
		int ID() const { return id; };

		void Name(const std::string& path)
		{
			name = path;
		};


	private:
		int id = 0;
		BYTE* imageData = nullptr;
		Vec2i spriteDims;
		Vec2i individualSpriteSize = Vec2i(0);
		int rowCount = 1;
		std::string name;
	};

}

#endif //SPRITESHEET