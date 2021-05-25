#include "ProbeSelectValueMax.h"
#include "util/logger.h"
#include <memory>
#include <sstream>

using namespace NetBrain;
using namespace std;

/**
* 1、对于每个探针的每个资源切片，计算出其最大贡献值，形成最大贡献值表（状态转移表）。
	1、状态转移表可能很大，所以在堆内存创建
	2、资源切片与坐标的转换！！！
	3、如果没有负载，或者没有资源，则无探针可用
* 2、最后一个探针的最后一个资源切片即为在资源限定下的最大贡献值
* 3、根据最大贡献值在状态转移表的坐标，推到出其路径，即包含哪些探针
*/
list<ProbeLoad> ProbeSelectValueMax::selectProbes()
{
	// 获取探针负载列表、资源容量，如果没有负载，或者没有资源，则无探针可用
	const vector<ProbeLoad>& probeLoads = m_probeScheduler->getProbeLoads();
	int cap = m_probeScheduler->getCap();
	if (probeLoads.empty() || cap <= 0) {
		LOG_WARN("Warning! There is no probes or resource\n");
		return list<ProbeLoad>();
	}

	// 生成最大贡献表
	vector<vector<int>> dpTable(probeLoads.size(), vector<int>(cap, 0));
	generateDPTable(dpTable, probeLoads, cap);
	LOG_INFO("generated a DT table[%zu][%zu]\n", dpTable.size(), dpTable[0].size());

	// 根据最后一个探针的最后一个资源切片的最大贡献值，推算出其探针列表
	deduceProbes(dpTable, probeLoads.size() - 1, cap - 1, probeLoads);
	LOG_INFO("generated a selected list with size of %zu\n", m_probeSelected.size());
	
	return m_probeSelected;
}

void NetBrain::ProbeSelectValueMax::generateDPTable(vector<vector<int>>& dpTable, const vector<ProbeLoad>& probeLoads, int cap)
{
	LOG_DEBUG("forming dynamic programming talbe...<probe, value[1...cap]>\n");

	// 遍历每个探针和每个资源切片，计算出每个(pi, fi)的最大贡献值
	for (size_t i = 0; i < probeLoads.size(); i++) {
		ostringstream osm;
		osm << probeLoads[i].name;

		for (int j = 0; j < cap; j++) {
			// 坐标-->资源!!!
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
				// otherwise,  max（不加入的贡献值，加入本探针的贡献值）
				else {
					// 如果加入					
					int valueWith = probeLoads[i].value;
					int resLeft = res - probeLoads[i].load;
					if (resLeft > 0) {	// 有剩余资源
						valueWith += dpTable[i - 1][resLeft - 1];	// 资源-->坐标!!!
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
			if (resLeft <= 0) {	// 没有剩余资源
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
