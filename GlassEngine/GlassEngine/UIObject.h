#if !defined (UIOBJECT)
#define UIOBJECT

namespace GlassEngine{

	class UIObject
	{
	public:  
		UIObject(){};
		virtual ~UIObject(){};

		virtual void Update(){};

		std::vector<SmartPtr<UIObject>> GetChildren() const;
		void AddUIObject(SmartPtr<UIObject> newUIObject);
		
		Vec2d Position() const;
		void Position(const Vec2d newPosition);

		Vec2d Offest() const;
		void Offest(const Vec2d newOffset);
		
		template <typename T>
		T Type() const { 
			return std::dynamic_pointer_cast<T, UIObject>(*this); 
		};
		
		void SetName(std::string name_){ name = name_; };
		std::string GetName() const { return name; };

	protected:
		std::string name;
		std::vector<SmartPtr<UIObject>> childUIObjects;
		Vec2d position = Vec2d(0.0);
		Vec2d offset = Vec2d(0.0);
	};

}
#endif