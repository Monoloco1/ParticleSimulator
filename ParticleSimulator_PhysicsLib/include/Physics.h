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
	DP gravity{0.0, 1.0};
	bool gravityBool{ true };

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

	D distanceBetween(const DP& p1, const DP& p2) const;
	void collisionReaction(Particle& p1, Particle& p2, const D& offsetX, const D& offsetY);
	void collisionReaction(const int& p1, const int& p2, const D& offsetX, const D& offsetY);
	bool collisionDetect(const Particle& p1, const Particle& p2, D& offsetX, D& offsetY);
	bool collisionDetect(const int& p1, const int& p2, D& offsetX, D& offsetY);
	bool hoverDetect(const Particle& p, const DP& pos);
	bool hoverDetect(const int& p, const DP& pos);
	void runPhysicsIteration();

	
	

	Physics() = default;
	Physics(const vector<Particle>& particles);
	~Physics();
};


