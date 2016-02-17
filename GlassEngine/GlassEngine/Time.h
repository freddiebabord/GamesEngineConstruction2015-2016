#if !defined (TIME_MANAGER)
#define TIME_MANAGER

#include "Manager.h"

namespace GlassEngine{

#define Time TimeManager::Instance()

	class TimeManager : public Manager
	{
	public:
		static TimeManager &Instance();
		~TimeManager(){};

		void Start() override;
		void Update() override;
		void Stop() override;

		double DeltaTime() const;
		double CurrentTime() const;

		void TimeScale(const double scale_){ scale = scale_; };
		double TimeScale() const { return scale; };

	protected:
		TimeManager(){};

	private:

		static TimeManager *instance;
		
		DWORD time;
		DWORD lastTime;
		double scale = 1.0;
	};

}

#endif
