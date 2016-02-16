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

	void Update() override;

	void OnCollisionEnter(SmartPtr<GameObject> gameObject) override;

};

#endif