#if !defined GLASS_PARTICLES
#define GLASS_PARTICLES

#include <string>
#include <list>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>
#include <vector>
#include "HAPI/HAPI_InputCodes.h"
#include <HAPI_lib.h>

#include "GlassMaths.h"

//Foreward decleration of the particle system and its components
class GlassParticleSystem;
class Emission;
class Velocity;
class ColourOverLife;
class SpriteSheet;
class ExternalForces;
class Collision;
class SubEmitters;
class SizeBySpeed;
class SizeByLifetime;
class Renderer;

//The class for a particle
class GlassParticle
{
public:
	//Default constructor for a particle
	GlassParticle(){};
	//Default destructor for a particle
	~GlassParticle(){};

	Transform transform;

	Vector3 velocity;
	HAPI_TColour particleColour;
	unsigned int lifetime = 0;
	int size = 1;
	int speed = 3;
};

//The base class of a particle system component
class GlassParticleComponent
{
public:
	//A pointer to the owning particle system 
	GlassParticleSystem *ParentParticleSystem;

	//A function which is overriden by every inheriting component to be called as a component
	virtual void Update(){};
};


//Class for a particle system
class GlassParticleSystem
{
public:
	//Default particle system constructor
	//particlesToSpawn: Amount of particles to spawn.
	//spawnBounds: Either the box volume constraints of the location in 3D space to spawn the particles in
	//randomSpawn: Makes the spawn position a (volumetric) area
	GlassParticleSystem(int particlesToSpawn, Vector3 spawnBounds, bool randomSpawn, int& width, int& height);
	~GlassParticleSystem()
	{
		delete[] particles;
		for each (GlassParticleComponent* c in particleComponents)
		{
			delete c;
		}
	};

	//Updates the particles
	void Update();
	
	//General settings
	bool useGravity = true;
	bool isLooping = false;
	int particlesToDraw = 50;
	GlassParticle *particles;
	Transform transform;

	//The components the particle system can use
	Emission *emission = nullptr;
	Velocity *velocity = nullptr;
	ColourOverLife *colourOverLife = nullptr;
	SpriteSheet *spriteSheet = nullptr;
	ExternalForces *externalForces = nullptr;
	Collision *collision = nullptr;
	SubEmitters *subEmitters = nullptr;
	SizeBySpeed *sizeBySpeed = nullptr;
	SizeByLifetime *sizeByLifetime = nullptr;
	Renderer *renderer = nullptr;

	//A list of attatched componants for the instance of the particle system
	std::list<GlassParticleComponent*> particleComponents;

	//More general settings
	HAPI_TColour particleColour = HAPI_TColour(0, 0, 0);
	int eyeDistx = 300;
	int eyeDisty = 300;
	Vector3 spawnStartPos;
	bool randSpawn;
};

#pragma region 
//A emission component handles the location the particles in the system spawn at.
//TO ADD: Rotation spawning and move colour spawning here
class Emission : public GlassParticleComponent{
public:
	Emission(Vector3 pos)
	{
		//Loop through all particles and set thier transform position
		for (int i = 0; i < ParentParticleSystem->particlesToDraw; i++)
		{
			ParentParticleSystem->particles[i].transform.position = pos;
		}
	};

	~Emission();

};

//The velocity component of the particle system which handles moving the particle
class Velocity : public GlassParticleComponent{

public:
	//The velocity particle system constructor
	//initialVelocity: the initial speed and direction of the particle
	Velocity(Vector3 initialVelocity){ velocity = initialVelocity; };
	~Velocity();

	//Function to change the particles velocity
	void UpdateVelocity(Vector3 newVelocity){ velocity = newVelocity; };

	//Update the velocity of the particle which overrides the components base
	void Update() override;

private:
	//The current velocity of the particles
	Vector3 velocity;
};

//Lerp the particles from the initial colour to the target colour regardless of the colour the particles spawn with
class ColourOverLife : public GlassParticleComponent{
public:
	ColourOverLife(HAPI_TColour initialColour, HAPI_TColour targetColour)
	{
		initColour = initialColour;
		tColour = targetColour;
		currentColour = initialColour;
	};
	~ColourOverLife();

	void Update() override;


	HAPI_TColour currentColour;
	HAPI_TColour initColour;
	HAPI_TColour tColour;
};

class SpritSheet : public GlassParticleComponent
{
	Vector2 uv = Vector2(0, 0);
};

class ExternalForces : public GlassParticleComponent{
public:
	ExternalForces(Vector3 velocity);
	~ExternalForces();

	void UpdateForce(Vector3 newForce);

private:
	Vector3 force;
};

class Collision : public GlassParticleComponent{
public:
	bool collidesWithWorld = false;
	bool collidesWithSelf = false;
};

class SubEmitters : public GlassParticleComponent{
public:
	SubEmitters();
	~SubEmitters();
	void OnAlive();
	void OnCollision();
	void OnDeath();

	std::vector<GlassParticleSystem*> subParticleSystemAlive;
	std::vector<GlassParticleSystem*> subParticleSystemCollision;
	std::vector<GlassParticleSystem*> subParticleSystemdeath;
private:
	bool aliveSystems = false;
	bool collisionSystems = false;
	bool deathSystems = false;
};

//Lerp the size of the particle by the speed of the particle
class SizeBySpeed : public GlassParticleComponent{
public:
	//Constructor for the size by speed component with 1 argument
	//targetSize: the scaling parameter of the particle
	SizeBySpeed(int targetSize) { tSize = targetSize; };
	~SizeBySpeed();

	//Update the size of the particle based on speed
	void Update() override;

private:
	int tSize;
};

//Increase the size of the particle over the lifetime of the particle
class SizeByLifetime : public GlassParticleComponent{
	
public:
	//Constructor for the size by lifetime component with 2 arguments
	//initialSize: the start size of the particle
	//finalSize: the final size of the particle
	SizeByLifetime(int initalSize, int finalSize)
	{
		initSize = initalSize;
		fSize = finalSize;
	}
	~SizeByLifetime();

	//Update the size of the particle on each call
	void Update() override;

private:
	//The start size of the particle
	int initSize;
	//The final size of the particle
	int fSize;
};

//The renderer of the particle system responsible of rendering each particle that the system ownes
class Renderer : public GlassParticleComponent{
public:
	Renderer(BYTE* screenP, int& widthP, int&heightP ): screen(screenP), width(widthP), height(heightP){};
	~Renderer();

	//Renders the particle each frame
	void Update() override;

	BYTE* screen;
	const int width;
	const int height;

};

#pragma endregion Particle System Components
#endif