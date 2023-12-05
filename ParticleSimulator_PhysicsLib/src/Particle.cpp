/*
-------------------------------------
|	Software written by Cristian Niwelt (C)
|
|	The software is supplied as is and for educational purposes
|	without any guarantees nor responsibility of its use in any application.
-------------------------------------
*/


#include "Particle.h"

DP Particle::getPos() {
	return pos;
}

void Particle::setPos(DP& newPos) {
	pos = newPos;
}

DP Particle::getVel() {
	return vel;
}

void Particle::setVel(DP& newVel) {
	vel = newVel;
}

void Particle::setColor(prt::Color newCol) {
	col = newCol;
}

prt::Color Particle::getColor() {
	return col;
}


Particle::Particle(const DP& pos, const DP& vel, const D& mass, const prt::Color& col, const BB& bb) :
	pos(pos), vel(vel), mass(mass), col(col), bb(bb) {}

Particle::~Particle() {}
