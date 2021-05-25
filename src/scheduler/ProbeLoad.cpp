#include "ProbeLoad.h"
#include <string>
#include <iostream>

namespace NetBrain {	
	std::ostream& operator << (std::ostream& osm, const ProbeLoad& probeLoad) {
		osm << probeLoad.name << "\t" << probeLoad.load << "\t" << probeLoad.value;
		return osm;
	}

	std::istream& operator >> (std::istream& ism, ProbeLoad& probeLoad) {
		ism >> probeLoad.name >> probeLoad.load >> probeLoad.value;
		return ism;
	}

	bool isGreatByLoad(const ProbeLoad& a, const ProbeLoad& b)
	{
		return a.load > b.load;
	}

	bool isSameProbe(const ProbeLoad& a, const ProbeLoad& b)
	{
		return a.name == b.name;
	}
}
