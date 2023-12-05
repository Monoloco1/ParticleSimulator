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


Particle::Particle(const DP& pos, const DP& vel, const D& mass, const prt::Color& col, const BB& bb, const DPV& shape) :
	pos(pos), vel(vel), mass(mass), col(col), bb(bb), shape(shape) {}

void Particle::setShape(DPV& newShape) {
	shape = newShape;
}

DPV Particle::getShape() {
	return shape;
}

Particle::Particle(const DP& pos) : pos(pos) {
	vel = { 0.0, 0.0 };
	mass = 1.1;
	col = prt::Red;
	bb.n = 10.0; bb.s = -10.0; bb.e = 10.0; bb.w = -10.0;
	shape = {
		{10.0, 0.0},
		{7.0, 7.0},
		{0.0, 10.0},
		{-7.0, 7.0},
		{-10.0, 0.0},
		{-7.0, -7.0},
		{0.0, -10.0},
		{7.0, -7.0},
	};	// approximation of an octagon :)
}

Particle::~Particle() {}
