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
using PV = vector<Particle>;	//	ParticleVector

class Physics {
protected:
	PV particles{};				//	Particles in this Physics instance
	DP gravity{0.0, 1.0};		//	global gravity amount
	bool gravityBool{ false };	//	gravity true or false

public:
	void removeParticles(const int& index);
	void removeParticles(const int& startIndex, const int& length);
	void addParticle(const Particle& p);
	void setParticles(const int& index, const Particle& p);
	void setParticles(const PV& newParticles);
	PV getParticles();
	Particle getParticles(const int& index);
	DP getParticleVel(const int& index) const;
	void setParticleVel(const int& index, const DP& newVel);

	DP getGravity() const;
	void setGravity(const DP& newGravity);
	bool getGravityBool() const;
	void setGravityBool(const bool& newGravityBool);

	void collisionReaction(Particle& p1, Particle& p2, const DP& offset);
	void collisionReaction(const int& p1, const int& p2, const DP& offset);
	bool collisionDetect(const Particle& p1, const Particle& p2, DP& offset);
	bool collisionDetect(const int& p1, const int& p2, DP& offset);
	bool hoverDetect(const Particle& p, const DP& pos);
	bool hoverDetect(const int& p, const DP& pos);
	void runPhysicsIteration();

	Physics() = default;
	Physics(const vector<Particle>& particles);
	~Physics();
};


