#if !defined (SPRITE)
#define SPRITE

#include "Component.h"

namespace GlassEngine{

	class GameObject;

	class Sprite: public Component
	{
	public:
		Sprite(SmartPtr<GameObject> parentObj) : Component(parentObj){ id = SpriteC; };
		Sprite(SmartPtr<GameObject> parentObj, std::string path) : Component(parentObj){ id = SpriteC; LoadSprite(path); name = path; };
		Sprite(std::string path){ id = SpriteC; LoadSprite(path); };
		Sprite() :Component(){};
		virtual ~Sprite();

		virtual void Update() override{};

		void Stop() override;

		BYTE* GetImage();
		bool LoadSprite(std::string path);
		//Gets the Sprites dimentions in a Vec2 (width, height)
		Vec2i GetSpriteDims();

		void Renderable(bool canBeRendered){ renderable = canBeRendered; };
		bool Renderable() const { return renderable; };

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
		bool renderable = true;
		std::string name = "";
	};

}

#endif //SPRITE