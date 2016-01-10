#include "precomp.h"
#include "Time.h"

namespace GlassEngine{


	TimeManager *TimeManager::instance{ nullptr };

	TimeManager &TimeManager::Instance()
	{
		if (instance == nullptr)
			instance = new TimeManager;
		return *instance;
	}

	void TimeManager::Start()
	{

	}

	void TimeManager::Update()
	{
		lastTime = time;
		time = HAPI->GetTime();
	}

	void TimeManager::Stop()
	{
		delete instance;
	}

	double TimeManager::DeltaTime() const
	{
		return (double)time - lastTime;
	}

	double TimeManager::CurrentTime() const
	{
		return (double)time;
	}
}