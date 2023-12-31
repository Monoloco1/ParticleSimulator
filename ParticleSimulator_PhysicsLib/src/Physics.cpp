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

/*	setParticles( Particle& p )
|-----------------------------------
|	These functions replace one or all Particles in the particles vector
|-----------------------------------
|	INPUT: a valid Particle element & index or a Particle vector
|-----------------------------------
|	OUTPUT: void
*/
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

/*	getParticleVel; setParticleVel
|-----------------------------------
|	direct getter&setter of the vel of a Particle in the particles vector
|-----------------------------------
|	INPUT: index of the Particle wanted & optionally set vel
|-----------------------------------
|	OUTPUT: it's vel DP or void
*/
DP Physics::getParticleVel(const int& index) const {
	assert(index >= 0 && index < (signed)particles.size());
	return particles.at(index).getVel();
}
void Physics::setParticleVel(const int& index, const DP& newVel) {
	assert(index >= 0 && index < (signed)particles.size());
	particles.at(index).setVel(newVel);
}

/*	Gravity getters/setters
|-----------------------------------
|	These functions set or get the gravity in the physicsEngine. Also set&get the gravityBool boolean
|-----------------------------------
|	INPUT: void or new gravity or new gravity bool
|-----------------------------------
|	OUTPUT: it's vel DP or void
*/
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
void Physics::collisionReaction(Particle& p1, Particle& p2, const DP& offset) {
	//	The code below is inspired on an older project of Cristian Niwelt
	
	//	Velocity change
	D angle = atan2(p1.getPos().y - p2.getPos().y, p1.getPos().x - p2.getPos().x);
	D normalMomentumI = 1.0 * p1.getMass() * (cos(angle) * p1.getVel().x + sin(angle) * p1.getVel().y);
	D normalMomentumJ = 1.0 * p2.getMass() * (cos(angle) * p2.getVel().x + sin(angle) * p2.getVel().y);
	p1.setVel({
		p1.getVel().x + normalMomentumJ * cos(angle) / p1.getMass() - normalMomentumI * cos(angle) / p1.getMass() ,
		p1.getVel().y + normalMomentumJ * sin(angle) / p1.getMass() - normalMomentumI * sin(angle) / p1.getMass()
		} );
	p2.setVel({
		p2.getVel().x + normalMomentumI * cos(angle) / p2.getMass() - normalMomentumJ * cos(angle) / p2.getMass(),
		p2.getVel().y + normalMomentumI * sin(angle) / p2.getMass() - normalMomentumJ * sin(angle) / p2.getMass()
		});

	//	TODO: this moves the particles by a constant amount each time they touch, change it
	//	Position change
	p1.setPos(p1.getPos() + DP(cos(angle), sin(angle)) * p2.getMass() / p1.getMass());
	p2.setPos(p2.getPos() - DP(cos(angle), sin(angle)) * p2.getMass() / p1.getMass());
	//p1.setPos(p1.getPos() + offset/2.0);
	//p2.setPos(p2.getPos() - offset/2.0);
}
void Physics::collisionReaction(const int& p1, const int& p2, const DP& offset) {
	assert(p1 >= 0 && p1 < (signed)particles.size());
	assert(p2 >= 0 && p2 < (signed)particles.size());
	collisionReaction(particles.at(p1), particles.at(p2), offset);
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
bool Physics::collisionDetect(const Particle& p1, const Particle& p2, DP& offset) {
	//	Check if particles are colliding
	if (p1.getPos().x + p1.getBB().w < p2.getPos().x + p2.getBB().e) return false;
	else if (p1.getPos().x + p1.getBB().e > p2.getPos().x + p2.getBB().w) return false;
	else if (p1.getPos().y + p1.getBB().s < p2.getPos().y + p2.getBB().n) return false;
	else if (p1.getPos().y + p1.getBB().n > p2.getPos().y + p2.getBB().s) return false;

	//	Calculate offset( by how much the particles overlay)
	//DP overlay1{}, overlay2{};
	/*
	pos1 + {w, s}   - pos2 + {e, n}
	pos1 + {e, n}
	*/
	//overlay1 = p1.getPos() + DP(p1.getBB().w, p1.getBB().s) - (p2.getPos() + DP(p2.getBB().e, p2.getBB().n));
	//overlay1 = p1.getPos() + DP(p1.getBB().w, p1.getBB().s) - (p2.getPos() + DP(p2.getBB().e, p2.getBB().n));

	//	Calculate offset in each direction
	D overlay1w2e = p1.getPos().x + p1.getBB().w - (p2.getPos().x + p2.getBB().e);
	D overlay2w1e = p2.getPos().x + p2.getBB().w - (p1.getPos().x + p1.getBB().e);
	D overlay1s2n = p1.getPos().y + p1.getBB().s - (p2.getPos().y + p2.getBB().n);
	D overlay2s1n = p2.getPos().y + p2.getBB().s - (p1.getPos().y + p1.getBB().n);

	//	Select the biggest one ?
	offset.x = overlay1w2e > overlay2w1e ? overlay1w2e : -overlay2w1e;
	offset.y = overlay1s2n > overlay2s1n ? overlay1s2n : -overlay2s1n;

	return true;
}
bool Physics::collisionDetect(const int& p1, const int& p2, DP& offset) {
	assert( p1 >= 0 && p1 < (signed)particles.size() );
	assert( p2 >= 0 && p2 < (signed)particles.size() );

	return collisionDetect(particles[p1], particles[p2], offset);
}

/*	hoverDetect(Particle& p, DP& pos)
|	hoverDetect(int& p1, DP& pos)
|-----------------------------------
|	This function detects if Particle is "hovered" by point, ie. mouse
|-----------------------------------
|	INPUT: Particle & pos(mouse.pos in world coords)
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
	
	return hoverDetect(particles[p], pos);
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
	DP offset{};

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
				if (collisionDetect(particles[i1], particles[i2], offset)) {
					collisionReaction(particles[i1], particles[i2], offset);
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

//	Destructor of the Physics class, default
Physics::~Physics() {}