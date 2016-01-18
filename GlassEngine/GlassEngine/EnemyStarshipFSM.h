#pragma once
#include "FSMBase.h"
#include "GameObject.h"

using namespace GlassEngine;

class EnemyStarshipFSM : public GameObject
{
public:
	EnemyStarshipFSM();
	~EnemyStarshipFSM();

	void Start() override;
	void Update() override;
	void Stop() override;

private:
	FSMBase<EnemyStarshipFSM> FSM;
};

