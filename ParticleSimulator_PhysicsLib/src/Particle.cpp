/*
-------------------------------------
|	Software written by Cristian Niwelt (C)
|
|	The software is supplied as is and for educational purposes
|	without any guarantees nor responsibility of its use in any application.
-------------------------------------
*/


#include "Particle.h"

//	PSFunc namespace with global functions used in this program
namespace PSFunc {
	D distanceBetween(const DP& p1, const DP& p2)  {
		DP deltaPos = p1 - p2;
		return std::sqrt(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y);
	}
}

//	Particle class getters & setters
DP Particle::getPos() const {
	return pos;
}
void Particle::setPos(const DP& newPos) {
	pos = newPos;
}
DP Particle::getVel() const {
	return vel;
}
void Particle::setVel(const DP& newVel) {
	vel = newVel;
}
BB Particle::getBB() const {
	return bb;
}
void Particle::setBB(const BB& newBB) {
	bb = newBB;
}
prt::Color Particle::getColor() const {
	return col;
}
void Particle::setColor(const prt::Color newCol) {
	col = newCol;
}
void Particle::setShape(const DPV& newShape) {
	shape = newShape;
}
DPV Particle::getShape() const {
	return shape;
}
void Particle::setMass(const D& newMass) {
	mass = newMass;
}
D Particle::getMass() const {
	return mass;
}
void Particle::setSize(const D& newSize) {
	bb.e *= newSize;
	bb.w *= newSize;
	bb.n *= newSize;
	bb.s *= newSize;

	std::transform(shape.begin(), shape.end(), shape.begin(),
		[newSize](DP dp) {
			dp.x = dp.x * newSize;
			dp.y = dp.y * newSize;
			return dp;
		} );
}

/*	Particle( * )
|-----------------------------------
|	This constructor of the Particle class sets all parameters of the class
|-----------------------------------
|	INPUT: all Particle parameters
|-----------------------------------
|	OUTPUT: Particle
*/
Particle::Particle(const DP& pos, const DP& vel, const D& mass, const prt::Color& col, const BB& bb, const DPV& shape) :
	pos(pos), vel(vel), mass(mass), col(col), bb(bb), shape(shape) {
}

/*	Particle( const DP& pos )
|-----------------------------------
|	This constructor of the Particle class sets default parameters for all
|	parameters of the class apart from the pos struct, which is given
|	Useful for debugging
|-----------------------------------
|	INPUT: Particle's pos Data Point
|-----------------------------------
|	OUTPUT: Particle
*/
Particle::Particle(const DP& pos) : 
	pos(pos) {

	vel = DP( 0.0, 0.0 );
	mass = 1.1;
	col = prt::Red;
	bb.n = -10.0; bb.s = 10.0; bb.e = -10.0; bb.w = 10.0;
	shape = {
		DP(10.0, 0.0),
		DP(7.0, 7.0),
		DP(0.0, 10.0),
		DP(-7.0, 7.0),
		DP(-10.0, 0.0),
		DP(-7.0, -7.0),
		DP(0.0, -10.0),
		DP(7.0, -7.0),
	};	// approximation of an octagon :)
}

/*	~Particle( )
|-----------------------------------
|	Default destructor of the Particle. Leaves destruction to the compiler for now
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: void
*/
Particle::~Particle() {}

/*	DP
|-----------------------------------
|	Default constructors of DP struct
|-----------------------------------
|	INPUT: x & y D's or just one xy DP
|-----------------------------------
|	OUTPUT: void
*/
DP::DP(const D& x, const D& y) {
	this->x = x;
	this->y = y;
}
DP::DP(const DP& dp) {
	this->x = dp.x;
	this->y = dp.y;
}

/*	=,+,+=,-,-=,/,/=,*,*= operator overloads for the DP struct
|-----------------------------------
|	just that
|-----------------------------------
|	INPUT: DP's or DP and D
|-----------------------------------
|	OUTPUT: DP
*/
DP& DP::operator= (const DP& dp1) {// = default;
	this->x = dp1.x;
	this->y = dp1.y;
	return *this;
}
DP operator+(const DP& dp1, const DP& dp2) {
	DP summed{ dp1.x + dp2.x, dp1.y + dp2.y };
	return summed;
}
DP operator+=(const DP& dp1, const DP& dp2) {
	DP summed{ dp1.x + dp2.x, dp1.y + dp2.y };
	return summed;
}
DP operator-(const DP& dp1, const DP& dp2) {
	DP summed{ dp1.x - dp2.x, dp1.y - dp2.y };
	return summed;
}
DP operator-=(const DP& dp1, const DP& dp2) {
	DP summed{ dp1.x - dp2.x, dp1.y - dp2.y };
	return summed;
}
DP operator*(const DP& dp, const D& mult) {
	DP multiplied{ dp.x * mult, dp.y * mult };
	return multiplied;
}
DP operator*=(const DP& dp, const D& mult) {
	DP multiplied{ dp.x * mult, dp.y * mult };
	return multiplied;
}
DP operator/(const DP& dp, const D& div) {
	DP divided{ dp.x / div, dp.y / div };
	return divided;
}
DP operator/=(const DP& dp, const D& div) {
	DP divided{ dp.x / div, dp.y / div };
	return divided;
}

