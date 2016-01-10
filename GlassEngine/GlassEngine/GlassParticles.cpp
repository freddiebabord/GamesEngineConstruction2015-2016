#include "GlassParticles.h"

//Default particle system constructor
//particlesToSpawn: Amount of particles to spawn.
//spawnBounds: Either the box volume constraints of the location in 3D space to spawn the particles in
//randomSpawn: Makes the spawn position a (volumetric) area
GlassParticleSystem::GlassParticleSystem(int particlesToSpawn, Vector3 spawnBounds, bool randomSpawn, int& width, int& height)
{
	spawnStartPos = spawnBounds;
	randSpawn = randomSpawn;
	//Find the center of the screen
	transform.position.x = width / 2;
	transform.position.y = height / 2;

	//sets the amount of particles the system should be responsible holds
	particlesToDraw = particlesToSpawn;
	//Create a new dynamic array with the ammount of particles that should be rendered
	particles = new GlassParticle[particlesToSpawn];

	//Loop through all of the particles and set its spawn position and velocity
	for (int i = 0; i < particlesToSpawn; i++)
	{		
		Vector3 rSpawn = Vector3(rand() % (int)spawnBounds.x, rand() % (int)spawnBounds.y, rand() % (int)spawnBounds.z);
		Vector3 vel = Vector3( 1, 1, 1 );
		particles[i].velocity = vel;
		particles[i].particleColour = particleColour;
		//if it should spawn randomly the spawn at the random ly assigned pos defined above
		//otherwise spawn at the position requested
		if (randomSpawn)
			particles[i].transform.position = rSpawn;
		else
			particles[i].transform.position = spawnBounds;
	}
	
}

//Update the particle system each frame
void GlassParticleSystem::Update()
{
	//Loop through all of the particles
	for (int i = 0; i < particlesToDraw; i++)
	{
		//If theparticle is beyond the established boundaries, reset the particle
		if (particles[i].transform.position.z <= -500)
		{
			particles[i].transform.position.z = rand() % 100 + 400;
			particles[i].lifetime = 0;
		}
		//increase the particles lifetime by 1 frame
		particles[i].lifetime += 1;
	}
}

//Update the velocity of the particle which overrides the components base
void Velocity::Update()
{
	//Loop through the particles in the system and increment their position by their velocity
	for (int i = 0; i < ParentParticleSystem->particlesToDraw; i++)
	{
		ParentParticleSystem->particles[i].transform.position += velocity;
	}
}

void ColourOverLife::Update()
{

	//Loop through all the particles and alter the size of the particle
	for (int i = 0; i < ParentParticleSystem->particlesToDraw; i++)
	{
		if (ParentParticleSystem->particles[i].lifetime < 100)
		{
			if (ParentParticleSystem->particles[i].particleColour.red != tColour.red && ParentParticleSystem->particles[i].particleColour.green != tColour.green && ParentParticleSystem->particles[i].particleColour.blue != tColour.blue){
				if (tColour.red > ParentParticleSystem->particles[i].particleColour.red)
					ParentParticleSystem->particles[i].particleColour.red += (int)(1 + (ParentParticleSystem->particles[i].lifetime / 50));
				else if (tColour.red < ParentParticleSystem->particles[i].particleColour.red)
					ParentParticleSystem->particles[i].particleColour.red -= (int)(1 - (ParentParticleSystem->particles[i].lifetime / 50));

				if (tColour.green > ParentParticleSystem->particles[i].particleColour.green)
					ParentParticleSystem->particles[i].particleColour.green += (int)(1 + (ParentParticleSystem->particles[i].lifetime / 50));
				else if (tColour.green < ParentParticleSystem->particles[i].particleColour.green)
					ParentParticleSystem->particles[i].particleColour.green -= (int)(1 - (ParentParticleSystem->particles[i].lifetime / 50));

				if (tColour.blue > ParentParticleSystem->particles[i].particleColour.blue)
					ParentParticleSystem->particles[i].particleColour.blue += (int)(1 + (ParentParticleSystem->particles[i].lifetime / 50));
				else if (tColour.blue < ParentParticleSystem->particles[i].particleColour.blue)
					ParentParticleSystem->particles[i].particleColour.blue -= (int)(1 - (ParentParticleSystem->particles[i].lifetime / 50));
			}
		}
	}
}

//Update the velocity of the particle which overrides the components base
ExternalForces::ExternalForces(Vector3 velocity)
{
	force = velocity;

	//Loop through the particles in the system and increment their position by their velocity
	for (int i = 0; i < ParentParticleSystem->particlesToDraw; i++)
	{
		ParentParticleSystem->particles[i].velocity += force;
	}
}

//Update the velocity of the particle which overrides the components base
void ExternalForces::UpdateForce(Vector3 newForce)
{
	force = newForce;
	//Loop through the particles in the system and increment their position by their velocity
	for (int i = 0; i < ParentParticleSystem->particlesToDraw; i++)
	{
		ParentParticleSystem->particles[i].velocity += force;
	}
}

//Update the size of the particle based on speed
void SizeBySpeed::Update()
{
	//Loop through all the particles and alter the size of the particle
	for (int i = 0; i < ParentParticleSystem->particlesToDraw; i++)
	{
		//Convert velocity into a scalar speed
		double scalerSpeed = abs((4 * ParentParticleSystem->particles[i].velocity.x) - (4 * ParentParticleSystem->particles[i].velocity.y) - (4 * ParentParticleSystem->particles[i].velocity.z));
		//Trnasform the size of the particle
		ParentParticleSystem->particles[i].size = (int)(scalerSpeed / (this->tSize * 10));
	}
}

//Update the size of the particle on each call
void SizeByLifetime::Update()
{
	//Loop through all the particles and alter the size of the particle
	for (int i = 0; i < ParentParticleSystem->particlesToDraw; i++)
	{
		ParentParticleSystem->particles[i].size = (int)abs(-500 + ParentParticleSystem->particles[i].transform.position.z + (int)ParentParticleSystem->particles[i].lifetime) / (this->fSize*10);
	}
}

//Renders the particle each frame
void Renderer::Update()
{
	//Loop through all the particles in the system
	for (int i = 0; i < ParentParticleSystem->particlesToDraw; i++)
	{
		//If the particle is still in the viewport
		//if (ParentParticleSystem->eyeDistx + ParentParticleSystem->particles[i].transform.position.z != 0 && ParentParticleSystem->eyeDisty + ParentParticleSystem->particles[i].transform.position.z){
			//Convert the particles world space into the 2D screen space
			int screenX = (int)((ParentParticleSystem->eyeDistx * (ParentParticleSystem->particles[i].transform.position.x - (width / 2))) / (ParentParticleSystem->eyeDistx + ParentParticleSystem->particles[i].transform.position.z)) + width / 2;
			int screenY = (int)((ParentParticleSystem->eyeDisty * (ParentParticleSystem->particles[i].transform.position.y - (height / 2))) / (ParentParticleSystem->eyeDisty + ParentParticleSystem->particles[i].transform.position.z)) + height / 2;

			//If the particle wont go off screen...
			if (screenX + ParentParticleSystem->particles[i].size < width && screenX >= 0 && screenY + ParentParticleSystem->particles[i].size < height && screenY >= 0)
			{
				//...loop through the particles x and y points
				for (int a = 0; a < ParentParticleSystem->particles[i].size; a++){ //a = x
					for (int b = 0; b < ParentParticleSystem->particles[i].size; b++) //b = y
					{
						//find the screen position and set its colour to the particle colour
						int offset = ((screenX + a) + (screenY + b)*width) * 4;
						memcpy(screen + offset, &ParentParticleSystem->particles[i].particleColour, 4);
					}
				}
			}
			//otherwise reset the particle
			else
			{
				//ParentParticleSystem->particles[i].transform.position.z = rand() % 100 + 400;
				
				Vector3 rSpawn;
				if (ParentParticleSystem->randSpawn)
					rSpawn = Vector3((int)ParentParticleSystem->spawnStartPos.x +50, rand() % (int)(ParentParticleSystem->spawnStartPos.y), rand() % (int)(ParentParticleSystem->spawnStartPos.z));
				else
				{
					rSpawn = ParentParticleSystem->spawnStartPos;
				}
				ParentParticleSystem->particles[i].transform.position = rSpawn;
				ParentParticleSystem->particles[i].lifetime = 0;
				ParentParticleSystem->particles[i].particleColour = ParentParticleSystem->particleColour;
			}
		//}
		//Otherwise reset the particle
		/*else
		{
			ParentParticleSystem->particles[i].transform.position.z = rand() % 100 + 400;
			ParentParticleSystem->particles[i].lifetime = 0;
			ParentParticleSystem->particles[i].particleColour = ParentParticleSystem->particleColour;
		}*/
	}
}