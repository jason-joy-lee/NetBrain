#include "ProbeSelectValueMaxV2.h"
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
list<ProbeLoad> ProbeSelectValueMaxV2::selectProbes()
{
	// 获取探针负载列表、资源容量，如果没有负载，或者没有资源，则无探针可用
	const vector<ProbeLoad>& probeLoads = m_probeScheduler->getProbeLoads();
	int cap = m_probeScheduler->getCap();
	if (probeLoads.empty() || cap <= 0) {
		LOG_WARN("Warning! There is no probes or resource\n");
		return list<ProbeLoad>();
	}

	// dp存放当前探针Pi在各种负载切片下的最大贡献值，初始值为0。数组多申请1个元素，以便索引与负载一致，dp[0]不用
	vector<int> dp(cap + 1, 0);

	for (int i = 0; i < probeLoads.size(); i++) {
		// 从大到小遍历，因为在计算时，dp[i]存放的dp[i-1)]的数
		// 只需要遍历到Pi的负载数，因为再小不会加入
		for (int j = cap; j >= probeLoads[i].load; j--) {
			int resLeft = j - probeLoads[i].load;
			dp[j] = std::max(dp[j], probeLoads[i].value + dp[resLeft]);
		}
	}

	// dp[cap]即为最大贡献值
	LOG_ERROR("最大贡献值为:%d\n", dp[cap]);

	// 根据dp[cap]递归倒推出路径
	m_probeSelected.clear();
	return deduceProbes(dp, cap, probeLoads);
}


list<ProbeLoad> ProbeSelectValueMaxV2::deduceProbes(vector<int>& dp, int fi, const vector<ProbeLoad>& probeLoads, int pi)
{
	// recursion end condition
	if (0 == pi) {
		if (probeLoads[pi].load > fi)
			return 0;
		else
			return probeLoads[pi].value;
	}

	for (int i=idxP, j=idxR; i >= 0; i--) {
		// the first probe was selected in the optimal path
		if (0 == i && dpTable[i][j] != 0) {
			m_probeSelected.push_back(probeLoads[i]);
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
	return m_probeSelected;
}

ProbeSelectValueMaxV2::ProbeSelectValueMaxV2(const ProbeScheduler* probeScheduler)
	: m_probeScheduler(probeScheduler)
{
	if (NULL == m_probeScheduler) {
		throw std::invalid_argument("Lack of ProbeScheduler");
	}
}