/*
-------------------------------------
|	Software written by Cristian Niwelt (C)
|	
|	The software is supplied as is and for educational purposes
|	without any guarantees nor responsibility of its use in any application. 
-------------------------------------
*/


#include "Physics.h"






/*	addParticle( Particle& p )
|-----------------------------------
|	This function adds a new particle to the Particle container
|-----------------------------------	
|	INPUT: a valid Particle element
|-----------------------------------
|	OUTPUT: void
*/
void Physics::addParticle(Particle& p) {
	particles.push_back(p);
}

PV Physics::getParticles() {
	return particles;
}

Particle Physics::getParticles(int index) {
	return particles[index];
}

/*	collisionDetect(Particle& p1, Particle& p2, D& offsetX, D& offsetY)
|-----------------------------------
|	This function detects if two Particles collide
|-----------------------------------
|	INPUT: two Particles
|-----------------------------------
|	OUTPUT: boolean true if they collide
|			distances in X,Y dims between centers(change referenced D's)
*/
bool Physics::collisionDetect(Particle& p1, Particle& p2, D& offsetX, D& offsetY) {
	if( p1.getPos().x - p1.)
	return false;
}

void Physics::runPhysicsIteration() {
	D offX{}, offY{};
	for (auto it1 = particles.begin(); it1 != particles.end() - 1; ++it1) {
		for (auto it2 = it1 + 1; it2 != particles.end(); ++it2) {
			if (collisionDetect(*it1, *it2, offX, offY)) {
				it1->setColor(prt::Red);
				it2->setColor(prt::Red);
			}
			else {
				it1->setColor(prt::White);
				it2->setColor(prt::White);
			}
		}
	}
}

/*	removeParticles( index ), removeParticles(int startIndex, int length)
|-----------------------------------
|	This function removes Particles from the Particle container
|-----------------------------------
|	INPUT: valid Particle index, valid Particle index & how many Particles to remove
|-----------------------------------
|	OUTPUT: void
*/
void Physics::removeParticles(int index) {
	particles.erase(particles.begin() + index);
}
void Physics::removeParticles(int startIndex, int length) {
	particles.erase(particles.begin() + startIndex, particles.begin() + startIndex + length);
}



//	Parametric constructor
Physics::Physics(const vector<Particle>& particles) : particles(particles) {}

Physics::~Physics() {}