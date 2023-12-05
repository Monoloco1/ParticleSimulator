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
using std::vector;

using D = double;	//	Data


struct DP {			//	Data Pair
	D x{};
	D y{};
};
DP operator+ (const DP& dp1, const DP& dp2);
DP operator* (const DP& dp, const D& mult);
DP operator-(const DP& dp1, const DP& dp2);

using DPV = vector<DP>;	//	Data Pair Vector

struct BB {			// Bounding Box
	D n{};	//North
	D s{};	//South
	D w{};	//West
	D e{};	//East
};

namespace prt {	//	Particle namespace
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
	//using White = {255, 255, 255, 255} ;
}
//using prt::Color;

class Particle {
protected:
	DP pos;				//position DP, already initialized
	DP vel;				//velocity DP
	BB bb;				//bounding box BB
	DPV shape;			//DPV representing Particle shape
	D mass{};
	prt::Color col{ {255, 255, 255, 255} };

public:
	DP getPos();
	void setPos(DP& newPos);
	DP getVel();
	void setVel(DP& newVel);
	void setColor(prt::Color newCol);
	prt::Color getColor();
	void setShape(DPV& newShape);
	DPV getShape();

	
	Particle() = default;	// moze do particle.cpp?
	Particle(const DP& pos, const DP& vel, const D& mass, const prt::Color& col, const BB& bb, const DPV& shape);
	Particle(const DP& pos); //Constructor for default Particles
	~Particle();
};