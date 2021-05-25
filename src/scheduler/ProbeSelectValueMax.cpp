#include "ProbeSelectValueMax.h"
#include "util/logger.h"
#include <memory>
#include <sstream>

using namespace NetBrain;
using namespace std;

/**
* 1������ÿ��̽���ÿ����Դ��Ƭ��������������ֵ���γ������ֵ��״̬ת�Ʊ���
	1��״̬ת�Ʊ���ܴܺ������ڶ��ڴ洴��
	2����Դ��Ƭ�������ת��������
	3�����û�и��أ�����û����Դ������̽�����
* 2�����һ��̽������һ����Դ��Ƭ��Ϊ����Դ�޶��µ������ֵ
* 3�����������ֵ��״̬ת�Ʊ�����꣬�Ƶ�����·������������Щ̽��
*/
list<ProbeLoad> ProbeSelectValueMax::selectProbes()
{
	// ��ȡ̽�븺���б���Դ���������û�и��أ�����û����Դ������̽�����
	const vector<ProbeLoad>& probeLoads = m_probeScheduler->getProbeLoads();
	int cap = m_probeScheduler->getCap();
	if (probeLoads.empty() || cap <= 0) {
		LOG_WARN("Warning! There is no probes or resource\n");
		return list<ProbeLoad>();
	}

	// ��������ױ�
	vector<vector<int>> dpTable(probeLoads.size(), vector<int>(cap, 0));
	generateDPTable(dpTable, probeLoads, cap);
	LOG_INFO("generated a DT table[%zu][%zu]\n", dpTable.size(), dpTable[0].size());

	// �������һ��̽������һ����Դ��Ƭ�������ֵ���������̽���б�
	deduceProbes(dpTable, probeLoads.size() - 1, cap - 1, probeLoads);
	LOG_INFO("generated a selected list with size of %zu\n", m_probeSelected.size());
	
	return m_probeSelected;
}

void NetBrain::ProbeSelectValueMax::generateDPTable(vector<vector<int>>& dpTable, const vector<ProbeLoad>& probeLoads, int cap)
{
	LOG_DEBUG("forming dynamic programming talbe...<probe, value[1...cap]>\n");

	// ����ÿ��̽���ÿ����Դ��Ƭ�������ÿ��(pi, fi)�������ֵ
	for (size_t i = 0; i < probeLoads.size(); i++) {
		ostringstream osm;
		osm << probeLoads[i].name;

		for (int j = 0; j < cap; j++) {
			// ����-->��Դ!!!
			int res = j + 1;

			// it makes the process of rest probes easier to specially process the first probe
			if (0 == i) {
				dpTable[i][j] = (probeLoads[i].load > res) ? 0 : probeLoads[i].value;
			}
			else {
				// if there is not enough resource, then the max value is same as P(i-1)
				if (probeLoads[i].load > res) {
					dpTable[i][j] = dpTable[i - 1][j];
				}
				// otherwise,  max��������Ĺ���ֵ�����뱾̽��Ĺ���ֵ��
				else {
					// �������					
					int valueWith = probeLoads[i].value;
					int resLeft = res - probeLoads[i].load;
					if (resLeft > 0) {	// ��ʣ����Դ
						valueWith += dpTable[i - 1][resLeft - 1];	// ��Դ-->����!!!
					}

					dpTable[i][j] = std::max(dpTable[i - 1][j], valueWith);
				}
			}

			osm << "\t" << dpTable[i][j];
		}
		LOG_DEBUG("%s\n", osm.str().c_str());
	}
}

size_t ProbeSelectValueMax::deduceProbes(vector<vector<int>>& dpTable, int idxP, int idxR, const vector<ProbeLoad>& probeLoads)
{
	// clear old data
	m_probeSelected.clear();

	for (int i=idxP, j=idxR; i >= 0; i--) {
		// the first probe was selected in the optimal path
		if (0 == i) {
			if (dpTable[i][j] != 0) {
				m_probeSelected.push_back(probeLoads[i]);
			}
		}
		// was selected
		else if (dpTable[i][j] != dpTable[i - 1][j]) {
			m_probeSelected.push_back(probeLoads[i]);

			int res = j + 1;
			int resLeft = res - probeLoads[i].load;
			if (resLeft <= 0) {	// û��ʣ����Դ
				break;
			}
			else {
				j = resLeft - 1;	// resource --> index
			}
		}
	}

	//cout << "deduceProbes: " << probeSelected << endl;
	return m_probeSelected.size();
}

ProbeSelectValueMax::ProbeSelectValueMax(const ProbeScheduler* probeScheduler)
	: m_probeScheduler(probeScheduler)
{
	if (NULL == m_probeScheduler) {
		throw std::invalid_argument("Lack of ProbeScheduler");
	}

	LOG_DEBUG("constructing ProbeSelectValueMax with Scheduler %p\n", m_probeScheduler);
}

NetBrain::ProbeSelectValueMax::~ProbeSelectValueMax()
{
	LOG_DEBUG("deconstructing ProbeSelectValueMax with Scheduler %p\n", m_probeScheduler);
}
