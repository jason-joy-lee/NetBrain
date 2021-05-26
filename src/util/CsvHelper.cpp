#include "CsvHelper.h"
#include "logger.h"

#include <fstream>
#include <iostream>
#include <sstream>

int NetBrain::CsvHelper::loadCSV(const string& filename, vector<ProbeLoad>& probeLoads, size_t maxLine, bool bTrunc)
{
	// open file
	ifstream ism(filename);
	if (!ism) {
		LOG_ERROR("Failed to open file %s\n", filename.c_str());
		return -1;
	}

	// how to deal with original probe loads
	if (bTrunc) {
		probeLoads.clear();
	}

	// load all lines, ignore invalid lines
	size_t numRead = 0;
	string line = "";		
	ProbeLoad probeLoad;
	while (!ism.eof() && numRead <= maxLine) {
		// read a line
		getline(ism, line);

		// extract fields
		istringstream ssm(line);
		if (ssm >> probeLoad) {
			if (probeLoad.load < 0 || probeLoad.value < 0) {
				LOG_DEBUG("Negative load or value in [%s]\n", line.c_str());
			}
			else
			{
				probeLoads.push_back(probeLoad);
				numRead++;
			}
		}
		else {
			LOG_DEBUG("Ignored invalid line [%s]\n", line.c_str());
		}
	}

	return numRead;
}
