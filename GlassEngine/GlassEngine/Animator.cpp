#include "precomp.h"
#include "Animator.h"
#include "Animation.h"
#include "GameObject.h"

namespace GlassEngine{

	AnimatorManager *AnimatorManager::instance{ nullptr };

	// Creates / get a singleton instance of the rendering manager
	AnimatorManager &AnimatorManager::Instance()
	{
		if (instance == nullptr)
			instance = new AnimatorManager;
		return *instance;
	}

	void AnimatorManager::Start()
	{
		for (const auto a : animations)
		{
			a->Start();
			a->SetProperties();
		}
	}

	void AnimatorManager::Update()
	{
		for (const auto a : animations)
		{
			a->Update();
		}
	}

	void AnimatorManager::Stop()
	{
		delete instance;
	}
}