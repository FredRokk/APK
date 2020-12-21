#ifndef _MESSAGES_
#define _MESSAGES_

#include <string>
#include "runway.hpp"
//#include "gate.hpp"

enum position
{
	in_air,
	taking_off,
	entering_runway,
	exiting_runway
};

namespace Messages
{
	struct PlaneToTower
	{
		PlaneToTower(position pos = in_air, bool rq = false, runway rw = runway(), std::string tn = "unknown")
			: TailNr(tn), Position(pos), RequestForLanding(rq), RW(rw){};
		std::string TailNr;
		position Position;
		bool RequestForLanding;
		runway& RW;
	};

	struct TowerToPlane
	{
		TowerToPlane(bool perm = false, runway rw = runway())
			: Permission(perm), RW(rw) {};
		bool Permission = false;
		runway& RW;
	};
};
#endif // !_MESSAGES_