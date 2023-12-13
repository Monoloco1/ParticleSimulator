/*
-------------------------------------
|	Software written by Cristian Niwelt (C)
|
|	The software is supplied as is and for educational purposes
|	without any guarantees nor responsibility of its use in any application.
-------------------------------------
*/


#include "Particle.h"

/*	getPos()
|-----------------------------------
|	This function returns the pos struct of the Particle
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: DP pos
*/
DP Particle::getPos() const {
	return pos;
}

/*	setPos( DP& newPos )
|-----------------------------------
|	This function sets the Particle's pos to newPos
|-----------------------------------
|	INPUT: new pos struct
|-----------------------------------
|	OUTPUT: void
*/
void Particle::setPos(const DP& newPos) {
	pos = newPos;
}

/*	getVel()
|-----------------------------------
|	This function returns the vel struct of the Particle
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: DP vel
*/
DP Particle::getVel() const {
	return vel;
}

/*	setVel( DP& newVel )
|-----------------------------------
|	This function sets the Particle's vel to newVel
|-----------------------------------
|	INPUT: new vel struct
|-----------------------------------
|	OUTPUT: void
*/
void Particle::setVel(const DP& newVel) {
	vel = newVel;
}

/*	getBB()
|-----------------------------------
|	This function returns the vel struct of the Particle
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: DP vel
*/
BB Particle::getBB() const {
	return bb;
}

/*	setVel( DP& newVel )
|-----------------------------------
|	This function sets the Particle's vel to newVel
|-----------------------------------
|	INPUT: new vel struct
|-----------------------------------
|	OUTPUT: void
*/
void Particle::setBB(const BB& newBB) {
	bb = newBB;
}

/*	getCol()
|-----------------------------------
|	This function returns the col prt::Color of the Particle
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: prt::Color col
*/
prt::Color Particle::getColor() const {
	return col;
}


/*	setCol( prt::Color newCol )
|-----------------------------------
|	This function sets the Particle's col to newCol
|-----------------------------------
|	INPUT: new prt::Color col struct
|-----------------------------------
|	OUTPUT: void
*/
void Particle::setColor(const prt::Color newCol) {
	col = newCol;
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
	pos(pos), vel(vel), mass(mass), col(col), bb(bb), shape(shape) {}

/*	setShape( DPV& newShape )
|-----------------------------------
|	This function sets the Shape vector of the Particle
|-----------------------------------
|	INPUT: vector of Data Points representing shape
|-----------------------------------
|	OUTPUT: void
*/
void Particle::setShape(const DPV& newShape) {
	shape = newShape;
}

/*	getShape( )
|-----------------------------------
|	This function gets the Shape vector of the Particle
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: vector of Data Points representing shape
*/
DPV Particle::getShape() const {
	return shape;
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

/*	Particle( const DP& pos )
|-----------------------------------
|	This constructor of the Particle class sets default parameters for all
|	parameters of the class apart from the pos struct
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
	bb.n = -10.0; bb.s = 10.0; bb.e = 10.0; bb.w = -10.0;
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
|	Default destructor of the Particle. Leaves destruction to the compiler
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: void
*/
Particle::~Particle() {}

DP::DP(const D& x, const D& y) {
	this->x = x;
	this->y = y;
}

DP& DP::operator= (const DP& dp1) {// = default;
	this->x = dp1.x;
	this->y = dp1.y;
	return *this;
}
DP::DP(const DP& dp) {
	this->x = dp.x;
	this->y = dp.y;
}

/*	+,-,/,* operator overloads for the DP struct
|-----------------------------------
|	TODO
|-----------------------------------
|	INPUT: all Particle parameters
|-----------------------------------
|	OUTPUT: Particle
*/
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

