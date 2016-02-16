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

private:
	
	double timeToDie = 0.0;
	double amountOfTimeAlive = 5000.0;
	int strength = 10;
};

#endif