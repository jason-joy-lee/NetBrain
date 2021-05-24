/***********************************************************************
 * 探针负载与贡献值情况
 *
 * @Author:  jason lee
 * @Date: 2021/5/21
 ***********************************************************************/
#pragma once

#include <string>
#include <iostream>

namespace NetBrain {
	struct ProbeLoad
	{
		// probe name
		std::string name;

		// load
		int load;

		// contribution point
		int value;

		// default constructor
		ProbeLoad(int load = 0, int value = 0) {
			this->load = load;
			this->value = value;
		}
	};

	std::ostream& operator << (std::ostream& osm, const ProbeLoad& probeLoad);

	std::istream& operator >> (std::istream& ism, ProbeLoad& probeLoad);
}
