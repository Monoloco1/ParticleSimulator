/*
-------------------------------------
|	Software written by Cristian Niwelt (C)
|
|	The software is supplied as is and for educational purposes
|	without any guarantees nor responsibility of its use in any application.
-------------------------------------
*/


#pragma once		// include this header in the translation unit only once

#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include "Particle.h"

using std::vector;
//using Map = std::tuple
using PV = vector<Particle>;	//	ParticleVector

class Physics {
protected:
	PV particles{};
	//map

public:
	void removeParticles(int index);
	void removeParticles(int startIndex, int length);
	void addParticle(Particle& p);
	//void setParticle(int index, Particle& p);
	PV getParticles();
	Particle getParticles(int index);

	void collisionReaction(Particle& p1, Particle& p2, D& offsetX, D& offsetY);
	bool collisionDetect(Particle& p1, Particle& p2, D& offsetX, D& offsetY);
	void runPhysicsIteration();
	

	Physics() = default;
	Physics(const vector<Particle>& particles);
	~Physics();
};


