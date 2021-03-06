#if !defined (RENDER_MANAGER)
#define RENDER_MANAGER

#include "Manager.h"

namespace GlassEngine{

#define Renderer RenderManager::Instance()
	
	class Sprite;
	class SpriteSheet;
	class Text;
	class UIObject;

	struct DirtyRectangle
	{
		DirtyRectangle(Vec2i position_, Vec2i size_){ position = position_; size = size_; };
		Vec2i position;
		Vec2i size;
	};

	//A structure to hold the screens width, height and the pointer to the screens pixel data for the application
	struct Screen
	{
		Screen(int width, int height, BYTE* screenPointer)
		{
			screenDimentions.width = width;
			screenDimentions.height = height;
			screenData = screenPointer;
		};
		Screen(int width, int height)
		{
			screenDimentions.width = width;
			screenDimentions.height = height;
		};
		Screen()
		{
			screenDimentions.width = 500;
			screenDimentions.height = 500;
		};
		//Size fo the screem
		Vec2i screenDimentions;
		//Pointer to the screens pixel data
		BYTE* screenData;
	};

	class RenderManager : public Manager
	{
	public:
		static RenderManager &Instance();
		~RenderManager(){};

		//Overide the base class' start stop and update functions
		void Start() override{};
		void Start(int Width = 0, int Height = 0, bool fullscreen = true);
		void Update() override;
		void Stop() override;

		// Clear the screen to a blanket colour by copying all of the pixels that were already set and then pasting them into the next set of the array. Fills the screen exponentially.
		void ClearScreen(const HAPI_TColour& backgroundColour);

		void RenderBackground(const int& spriteIndex, const Vec3d& renderPos);

		//Render an image on screen
		//int spriteIndex: The index of the sprite in the render managers vector of sprites
		//Vec3 renderPos: The position in 3D space the image should be rendered at
		void Render(const int& spriteIndex, const Vec3d& renderPos);

		//Render an image on screen
		//int spriteIndex: The index of the spritesheet in the render managers vector of spritesheet 
		//int idvSpriteIndex: The index of the sprite frame for the spritesheet 
		//Vec3 renderPos: The position in 3D space the image should be rendered at
		void Render(const int& spriteIndex, const Vec3d& renderPos, const int& idvSpriteIndex);

		void RenderUI(const int& uiSpriteIndex, const Vec2d& renderPos);

		//Dirty Recatngles - clears a portion of the screen, based off of the screen position and size of the rectangle to render from
		void RenderDR(const Vec2i& pos, const Vec2i& size);

		//Returns a Vector2<int> of the screen dimentions
		const Vec2i GetScreenDimentions();

		// Add a sprite to the render manager either by a pointer to a block of memory which contains the sprite
		// or from the manager laoding in a sprite from a specified path string
		bool AddSprite(const std::string& path);
		bool AddSprite(SmartPtr<Sprite> spriteToAdd);

		// Add a spritesheet to the render manager either by a pointer to a block of memory which contains the spritesheet
		// or from the manager laoding in a spritesheet from a specified path string
		bool AddSpriteSheet(const std::string& path, Vec2i spriteSize);
		bool AddSpriteSheet(SmartPtr<SpriteSheet> spriteToAdd);

		SmartPtr<Sprite> GetSprite(const int& spriteIndex) const { return sprites[spriteIndex]; };
		// Returns a spritesheet from the vector or spritesheets that the render manager has
		SmartPtr<SpriteSheet> GetSpriteSheet(const int& spriteSheetIndex){ return spritesheets[spriteSheetIndex]; };
		void ClearRenderer();
		void ResetToPrevious();
		void ClearPrevious();

		void AddDirtyRectangle(DirtyRectangle r){ dirtyRectangles.push_back(r); };

		void RenderToBackground(SmartPtr<Sprite> sprite, Vec3d renderPos);

		int GetSpriteSheetStorageSize() const { return (int)spritesheets.size(); };
		int GetSpriteStorageSize() const { return (int)sprites.size(); };

		void DrawCollisionMask(int posX, int posY, DWORD* collisionMask, Rect sourceRct);

		void RenderText(Vec2d position, SmartPtr<Text> textToRender);
		void AddUISprite(SmartPtr<Sprite> spriteToAdd){ uiSprites.push_back(spriteToAdd); };

		Vec2i GetUISprite(int uiSpriteRefrance) const;

	protected:
		RenderManager(){};

	private:
		// The local instance of the render manager
		static RenderManager *instance;
		// Storage for local variables
		std::unique_ptr<Screen> screen;
		std::vector<SmartPtr<Sprite>> sprites;
		std::vector<SmartPtr<SpriteSheet>> spritesheets;
		std::vector<SmartPtr<Sprite>> previousSprites;
		std::vector<SmartPtr<SpriteSheet>> previousSpritesheets;
		std::vector<SmartPtr<Sprite>> uiSprites;
		std::vector<DirtyRectangle> dirtyRectangles;
	};

#define Background 0

}
#endif //RENDER_MANAGER
