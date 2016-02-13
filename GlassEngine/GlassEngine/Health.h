#if !defined (HEALTH)
#define HEALTH

#include "precomp.h"
#include "Component.h"

namespace GlassEngine
{
	class Health : public Component
	{
	public:
		Health(SmartPtr<GameObject> parentObj_) : Component(parentObj_){ id = HealthC; };
		Health() : Component() { id = HealthC; };
		virtual ~Health(){};

		void SetHealth(const int newHealth){ currentHealth = newHealth; };
		const int GetHealth() const { return currentHealth; };
		void AddToHealth(const int amount){ currentHealth += amount; };

	private:
		int currentHealth = 100;
	};
}

#endif //HEALTH