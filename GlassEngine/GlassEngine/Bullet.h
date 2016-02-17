#if !defined (BULLET)
#define BULLET

#include "GlassEngine.h"

using namespace GlassEngine;

class Bullet : public GameObject
{
public:
	Bullet() : GameObject(){};
	Bullet(int id_) : GameObject(id_){};
	virtual ~Bullet(){};

	void Start() override{ GameObject::Start(); };
	void OnEnable() override;
	void Update() override;

	void OnCollisionEnter(SmartPtr<GameObject> gameObject) override;
	void SetForce(Vec3d force_){ force = force_; };
	void UsePhysics(bool canUsePhysics){ usePhysics = canUsePhysics; };
	void SetDamageAmaount(const int newDamageAmount){ strength = newDamageAmount; };

private:
	Vec3d force = Vec3d(0.0);
	double timeToDie = 0.0;
	double amountOfTimeAlive = 5000.0;
	int strength = 10;
	bool usePhysics = true;
};

#endif