/*
-------------------------------------
|	Software written by Cristian Niwelt (C)
|
|	The software is supplied as is and for educational purposes
|	without any guarantees nor responsibility of its use in any application.
-------------------------------------
*/

#pragma once

#include <iostream>
#include <vector>
#include <algorithm>


using std::vector;
using D = double;	//	Data

struct DP {			//	Data Pair
	D x{};
	D y{};

public:
	DP& operator= (const DP& dp1);
	DP(const DP& dp);
	DP(const D& x, const D& y);
	DP() = default;
};

DP operator+ (const DP& dp1, const DP& dp2);
DP operator* (const DP& dp, const D& mult);
DP operator/ (const DP& dp, const D& div);
DP operator- (const DP& dp1, const DP& dp2);

DP operator+= (const DP& dp1, const DP& dp2);
DP operator*= (const DP& dp, const D& mult);
DP operator/= (const DP& dp, const D& div);
DP operator-= (const DP& dp1, const DP& dp2);




using DPV = vector<DP>;	//	Data Pair Vector

struct BB {			// Bounding Box
	D n{};	//North
	D s{};	//South
	D w{};	//West
	D e{};	//East
};

namespace prt {	//	Particle namespace
	//	This Color struct is created to add compatibility for graphics libraries other than SDL
	union Color {	//	Color union compatible with SDL_Color
		struct {
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		} str;
		uint8_t arr[4];

		//Color(uint8_t arr1[4]) : arr(arr1);
	};
	

	const Color Red = { {255, 0, 0, 255} };
	const Color White = { {255, 255, 255, 255} };
}

namespace PSFunc {
	D distanceBetween(const DP& p1, const DP& p2);
}

class Particle {
protected:
	DP pos;				//position DP, already initialized in DP definition
	DP vel;				//velocity DP
	BB bb;				//bounding box BB
	DPV shape;			//DPV representing Particle shape, relative to Particle pos
	D mass{};			//mass of the Particle
	prt::Color col{ {255, 255, 255, 255} };	//Color of the Particle

public:
	DP getPos() const;
	void setPos(const DP& newPos);
	DP getVel() const;
	void setVel(const DP& newVel);
	BB getBB() const;
	void setBB(const BB& newBB);
	void setColor(const prt::Color newCol);
	prt::Color getColor() const;
	void setShape(const DPV& newShape);
	DPV getShape() const;
	void setMass(const D& newMass);
	D getMass() const;
	void setSize(const D& newSize);

	
	Particle() = default;	// moze do particle.cpp?
	Particle(const DP& pos, const DP& vel, const D& mass, const prt::Color& col, const BB& bb, const DPV& shape);
	Particle(const DP& pos); //Constructor for default Particles
	~Particle();
};