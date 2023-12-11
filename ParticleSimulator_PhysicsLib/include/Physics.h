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
	void removeParticles(const int& index);
	void removeParticles(const int& startIndex, const int& length);
	void addParticle(const Particle& p);
	void setParticles(const int& index, const Particle& p);
	void setParticles(const PV& newParticles);
	PV getParticles();
	Particle getParticles(const int& index);

	void collisionReaction(Particle& p1, Particle& p2, const D& offsetX, const D& offsetY);
	bool collisionDetect(const Particle& p1, const Particle& p2, D& offsetX, D& offsetY);
	bool collisionDetect(const int& p1, const int& p2, D& offsetX, D& offsetY);
	void runPhysicsIteration();
	

	Physics() = default;
	Physics(const vector<Particle>& particles);
	~Physics();
};


