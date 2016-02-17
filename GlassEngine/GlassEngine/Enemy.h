#pragma once

#include "GlassEngine.h"

using namespace GlassEngine;

class EnemyAI : public GameObject
{
public:
	EnemyAI(int id_) : GameObject(id_){};
	EnemyAI() : GameObject(){};
	virtual ~EnemyAI(){};

	virtual void Start() override;
	virtual void Update() override;

	virtual void Explode() override;
};