#if !defined (ENEMYBOSS)
#define ENEMYBOSS

#include "Enemy.h"

using namespace GlassEngine;

class BossAI : public EnemyAI
{
public:
	BossAI(int id_) : EnemyAI(id_) {};
	BossAI() : EnemyAI(){};

	virtual ~BossAI(){};

	void Start() override;
	void Update() override;

	void Explode() override;

private:
	bool ShouldDie = false;
	bool gameWon = false;
};

#endif