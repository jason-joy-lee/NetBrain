#include "ProbeSelectVGreedy.h"
#include "util/logger.h"
#include <sstream>

using namespace NetBrain;
using namespace std;

/**
* 1、先对探针列表倒序排序
* 2、认为前N个探针即为最优解，且负载之和 <= 资源容量
*/
list<ProbeLoad> ProbeSelectVGreedy::selectProbes()
{
	// 获取探针负载列表、资源容量，如果没有负载，或者没有资源，则无探针可用
	const vector<ProbeLoad>& probeLoads = m_probeScheduler->getProbeLoads();
	int cap = m_probeScheduler->getCap();
	if (probeLoads.empty() || cap <= 0) {
		LOG_WARN("Warning! There is no probes or resource\n");
		return list<ProbeLoad>();
	}

	for (vector<ProbeLoad>::const_iterator itr = probeLoads.begin(); itr != probeLoads.end(); itr++) {
		if (itr->load > cap || itr->load <= 0) {
			continue;
		}
		else if (itr->load == cap) {
			m_probeSelected.push_back(*itr);
			break;
		}

		// find the first group
		int sum = 0;
		for (vector<ProbeLoad>::const_iterator p2 = itr; p2 != probeLoads.end(); p2++) {
			if ((sum + p2->load) <= cap) {
				m_probeSelected.push_back(*p2);
				sum += p2->load;
			}
			else {
				return m_probeSelected;
			}
		}
	}
	
	return m_probeSelected;
}


ProbeSelectVGreedy::ProbeSelectVGreedy(const ProbeScheduler* probeScheduler)
	: m_probeScheduler(probeScheduler)
{
	if (NULL == m_probeScheduler) {
		throw std::invalid_argument("Lack of ProbeScheduler");
	}
}

int NetBrain::ProbeSelectVGreedy::sum(vector<ProbeLoad>::iterator begin, vector<ProbeLoad>::iterator end)
{
	int sum = 0;
	while (begin != end) {
		sum += begin->load;
	}

	return sum;
}
