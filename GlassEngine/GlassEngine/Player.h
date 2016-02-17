#if !defined (PLAYER)
#define PLAYER

#include "GlassEngine.h"

using namespace GlassEngine;


class Player : public GameObject
{
public:
	Player() : GameObject() {};
	Player(int id_) : GameObject(id_){};
	virtual ~Player(){};

	void Start() override;
	void Update() override;

	void OnCollisionEnter(SmartPtr<GameObject> gameObject) override;

	void SimplifiedControl(const bool control){ simplifiedlControlScheme = control; };
private:
	float cascadeQuantity = 0;
	float bulletSpawnRate = 0;
	bool simplifiedlControlScheme = false;
	Vec3d respawnPosition;
	double maxTimeAfterReset = 0.0;
	double timeSinceReset = 0.0;
};

#endif