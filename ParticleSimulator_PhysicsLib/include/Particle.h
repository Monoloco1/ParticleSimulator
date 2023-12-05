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

using D = double;	//	Data


struct DP {			//	DataPair
	D x{};
	D y{};
};
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
	DP pos;		//position DP, already initialized
	DP vel;		//velocity DP
	BB bb;		//bounding box BB
	D mass{};
	prt::Color col{ {255, 255, 255, 255} };
	//shape

public:
	DP getPos();
	void setPos(DP& newPos);
	DP getVel();
	void setVel(DP& newVel);
	void setColor(prt::Color newColor);

	
	Particle() = default;	// moze do particle.cpp?
	Particle(const DP& pos, const DP& vel, const D& mass, const prt::Color& col, const BB& bb);
	~Particle();
};