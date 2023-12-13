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
void Physics::addParticle(const Particle& p) {
	particles.push_back(p);
}

void Physics::setParticles(const int& index, const Particle& p) {
	particles.at(index) = p;
}
void Physics::setParticles(const PV& newParticles) {
	particles = newParticles;
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
Particle Physics::getParticles(const int& index) {
	return particles[index];
}

DP Physics::getGravity() const {
	return gravity;
}
void Physics::setGravity(const DP& newGravity) {
	gravity = newGravity;
}
bool Physics::getGravityBool() const {
	return gravityBool;
}
void Physics::setGravityBool(const bool& newGravityBool) {
	gravityBool = newGravityBool;
}

/*	collisionReaction(Particle& p1, Particle& p2, D& offsetX, D& offsetY)
|-----------------------------------
|	This function applies a reaction function to the colliding particles
|-----------------------------------
|	INPUT: two Particles and offsets by which they collide by
|-----------------------------------
|	OUTPUT: void
*/
void Physics::collisionReaction(Particle& p1, Particle& p2, const D& offsetX, const D& offsetY) {
	p1.setColor(prt::Red);
	p2.setColor(prt::Red);
}
void Physics::collisionReaction(const int& p1, const int& p2, const D& offsetX, const D& offsetY) {
	assert(p1 >= 0 && p1 < (signed)particles.size());
	assert(p2 >= 0 && p2 < (signed)particles.size());
	particles.at(p1).setColor(prt::Red);
	particles.at(p2).setColor(prt::Red);
}

/*	collisionDetect(Particle& p1, Particle& p2, D& offsetX, D& offsetY)
|	collisionDetect(int& p1, int& p2, D& offsetX, D& offsetY)
|-----------------------------------
|	This function detects if two Particles collide
|-----------------------------------
|	INPUT: two Particles
|-----------------------------------
|	OUTPUT: boolean true if they collide
|			distances in X,Y dims between centers of Particles(change referenced D's)
*/
bool Physics::collisionDetect(const Particle& p1, const Particle& p2, D& offsetX, D& offsetY) {
	//if( p1.getPos().x - p1.)
	return false;
}
bool Physics::collisionDetect(const int& p1, const int& p2, D& offsetX, D& offsetY) {
	assert( p1 >= 0 && p1 < (signed)particles.size() );
	assert( p2 >= 0 && p2 < (signed)particles.size() );
	return false;
}

/*	hoverDetect(Particle& p, DP& pos)
|	hoverDetect(int& p1, DP& pos)
|-----------------------------------
|	This function detects if Particle is "hovered" by point, ie. mouse
|-----------------------------------
|	INPUT: Particle & pos
|-----------------------------------
|	OUTPUT: boolean true if hovered
*/
bool Physics::hoverDetect(const Particle& p, const DP& pos) {
	if (p.getPos().x + p.getBB().w < pos.x) return false;
	else if (p.getPos().x + p.getBB().e > pos.x) return false;
	else if (p.getPos().y + p.getBB().s < pos.y) return false;
	else if (p.getPos().y + p.getBB().n > pos.y) return false;
	else return true;
}
bool Physics::hoverDetect(const int& p, const DP& pos) {
	assert(p >= 0 && p < (signed)particles.size());
	if (particles.at(p).getPos().x + particles.at(p).getBB().w < pos.x) return false;
	else if (particles.at(p).getPos().x + particles.at(p).getBB().e > pos.x) return false;
	else if (particles.at(p).getPos().y + particles.at(p).getBB().s < pos.y) return false;
	else if (particles.at(p).getPos().y + particles.at(p).getBB().n > pos.y) return false;
	else return true;
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

	//	Apply gravity acceleration
	if(getGravityBool())
		for (auto& p : particles)
			p.setVel(
				p.getVel() + gravity
			);

	//	Apply velocity position change
	for (auto& p : particles) {
		p.setPos(
			p.getPos() + p.getVel()
		);
	}

	//	Apply collision reaction
	if (particles.size() > 0)
		for (unsigned i1{ 0 }; i1 < particles.size() - 1; ++i1) {

			for (unsigned i2{ i1+1 }; i2 < particles.size() ; ++i2) {
				if (collisionDetect(particles[i1], particles[i2], offX, offY)) {
					collisionReaction(particles[i1], particles[i2], offX, offY);
				}
			}
		}

}

/*	removeParticles( index ); removeParticles(int startIndex, int length)
|-----------------------------------
|	This function removes Particles from the Particle container
|-----------------------------------
|	INPUT: valid Particle index; valid Particle index & how many Particles to remove
|-----------------------------------
|	OUTPUT: void
*/
void Physics::removeParticles(const int& index) {
	particles.erase(particles.begin() + index);
}
void Physics::removeParticles(const int& startIndex, const int& length) {
	particles.erase(particles.begin() + startIndex, particles.begin() + startIndex + length);
}



//	Parametric constructor
Physics::Physics(const vector<Particle>& particles) : particles(particles) {}

Physics::~Physics() {}