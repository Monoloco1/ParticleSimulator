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

/*	getParticles( )
|-----------------------------------
|	This function returns the particles vector from the Physics class
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: PV particles
*/
PV Physics::getParticles() {
	return particles;
}

/*	getParticle( int index )
|-----------------------------------
|	This function  returns the particle indexed from the Physics class
|-----------------------------------
|	INPUT: index of the Particle wanted
|-----------------------------------
|	OUTPUT: Particle
*/
Particle Physics::getParticles(int index) {
	return particles[index];
}

/*	collisionReaction(Particle& p1, Particle& p2, D& offsetX, D& offsetY)
|-----------------------------------
|	This function applies a reaction function to the colliding particles
|-----------------------------------
|	INPUT: two Particles and offsets by which they collide by
|-----------------------------------
|	OUTPUT: void
*/
void Physics::collisionReaction(Particle& p1, Particle& p2, D& offsetX, D& offsetY) {
	p1.setColor(prt::Red);
	p2.setColor(prt::Red);
}

/*	collisionDetect(Particle& p1, Particle& p2, D& offsetX, D& offsetY)
|-----------------------------------
|	This function detects if two Particles collide
|-----------------------------------
|	INPUT: two Particles
|-----------------------------------
|	OUTPUT: boolean true if they collide
|			distances in X,Y dims between centers of Particles(change referenced D's)
*/
bool Physics::collisionDetect(Particle& p1, Particle& p2, D& offsetX, D& offsetY) {
	//if( p1.getPos().x - p1.)
	return false;
}

/*	runPhysicsIteration()
|-----------------------------------
|	This function applies the collision detection/reaction function to every pair of particles
|	in the particle vector
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: void
*/
void Physics::runPhysicsIteration() {
	D offX{}, offY{};

	for (unsigned i1{ 0 }; i1 < particles.size() - 1; ++i1) {
		for (unsigned i2{ i1+1 }; i2 < particles.size() ; ++i2) {
			if (collisionDetect(particles[i1], particles[i2], offX, offY)) {
				collisionReaction(particles[i1], particles[i2], offX, offY);
			}
		}
	}
	/*for (auto it1 = particles.begin(); it1 != particles.end() - 1; ++it1) {
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
	}*/
}

/*	removeParticles( index ); removeParticles(int startIndex, int length)
|-----------------------------------
|	This function removes Particles from the Particle container
|-----------------------------------
|	INPUT: valid Particle index; valid Particle index & how many Particles to remove
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