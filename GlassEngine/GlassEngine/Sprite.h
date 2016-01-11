#if !defined (SPRITE)
#define SPRITE

#include "Component.h"

namespace GlassEngine{

	class GameObject;

	class Sprite: public Component
	{
	public:
		Sprite(std::shared_ptr<GameObject> parentObj) : Component(parentObj){ id = SpriteC; };
		Sprite(std::shared_ptr<GameObject> parentObj, std::string path) : Component(parentObj){ id = SpriteC; LoadSprite(path); };
		Sprite(std::string path){ id = SpriteC; LoadSprite(path); };
		Sprite() :Component(){};
		virtual ~Sprite();

		virtual void Update() override{};

		void Stop() override;

		BYTE* GetImage();
		bool LoadSprite(std::string path);
		//Gets the Sprites dimentions in a Vec2 (width, height)
		Vec2i GetSpriteDims();

	private:
		BYTE* imageData = nullptr;
		Vec2i spriteDims;
	};

}

#endif //SPRITE