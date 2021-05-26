#include "ProbeSelectVGreedy.h"
#include "util/logger.h"
#include <sstream>

using namespace NetBrain;
using namespace std;

/**
* 认为前N个探针即为最优解，且负载之和 <= 资源容量
*/
list<ProbeLoad> ProbeSelectVGreedy::selectProbes()
{
	// clear the cached path，先清空以消除语义的二义性
	m_probeSelected.clear();

	// 获取探针负载列表、资源容量，如果没有负载，或者没有资源，则无探针可选
	const vector<ProbeLoad>& probeLoads = m_probeScheduler->getProbeLoads();
	int cap = m_probeScheduler->getCap();
	if (probeLoads.empty() || cap <= 0) {
		LOG_WARN("Warning! There is no probes or resource\n");
		return list<ProbeLoad>();
	}

	// 从大到小，能装的全装
	for (vector<ProbeLoad>::const_iterator itr = probeLoads.begin(); itr != probeLoads.end(); itr++) {
		// 没有资源了
		if (0 == cap) {
			break;
		}
		// 有资源但是资源不足，或非法负载
		else if (itr->load > cap || itr->load <= 0) {
			continue;
		}
		else {
			m_probeSelected.push_back(*itr);
			cap -= itr->load;
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

	LOG_DEBUG("constructing ProbeSelectVGreedy with Scheduler %p\n", m_probeScheduler);
}

NetBrain::ProbeSelectVGreedy::~ProbeSelectVGreedy()
{
	LOG_DEBUG("deconstructing ProbeSelectVGreedy with Scheduler %p\n", m_probeScheduler);
}
