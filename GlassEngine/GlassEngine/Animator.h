#if !defined (ANIMATOR_MANAGER)
#define ANIMATOR_MANAGER

#include "Manager.h"

namespace GlassEngine{

#define Animator AnimatorManager::Instance()

	class Animation;

	class AnimatorManager : public Manager
	{
	public:
		static AnimatorManager &Instance();
		~AnimatorManager(){};

		//Overide the base class' start stop and update functions
		void Start() override{};
		void Update() override;
		void Stop() override;

		void AddAnimation(SmartPtr<Animation> animation){ animations.push_back(animation); };

	protected:
		AnimatorManager(){};

	private:
		// The local instance of the render manager
		static AnimatorManager *instance;
		std::vector<SmartPtr<Animation>> animations;
	};
} 

#endif //ANIMATOR_MANAGER