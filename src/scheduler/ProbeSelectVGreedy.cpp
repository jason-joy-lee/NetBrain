#include "ProbeSelectVGreedy.h"
#include "util/logger.h"
#include <sstream>

using namespace NetBrain;
using namespace std;

/**
* ��ΪǰN��̽�뼴Ϊ���Ž⣬�Ҹ���֮�� <= ��Դ����
*/
list<ProbeLoad> ProbeSelectVGreedy::selectProbes()
{
	// clear the cached path�����������������Ķ�����
	m_probeSelected.clear();

	// ��ȡ̽�븺���б���Դ���������û�и��أ�����û����Դ������̽���ѡ
	const vector<ProbeLoad>& probeLoads = m_probeScheduler->getProbeLoads();
	int cap = m_probeScheduler->getCap();
	if (probeLoads.empty() || cap <= 0) {
		LOG_WARN("Warning! There is no probes or resource\n");
		return list<ProbeLoad>();
	}

	// �Ӵ�С����װ��ȫװ
	for (vector<ProbeLoad>::const_iterator itr = probeLoads.begin(); itr != probeLoads.end(); itr++) {
		// û����Դ��
		if (0 == cap) {
			break;
		}
		// ����Դ������Դ���㣬��Ƿ�����
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
