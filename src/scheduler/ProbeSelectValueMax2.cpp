#include "ProbeSelectValueMax2.h"
#include "util/logger.h"
#include <sstream>

using namespace NetBrain;
using namespace std;

/**
* 1、遍历每个探针的每个资源切片，计算出其最大贡献值，只保存最后一个探针的各个资源切片的最优贡献值数组
	1、状态转移表可能很大
	2、资源切片与坐标的转换！！！
	3、如果没有负载，或者没有资源，则无探针可用
* 2、最后一个探针的最后一个资源切片即为在资源限定下的最大贡献值
* 3、根据最大贡献值在状态转移表的坐标，回溯推到出其路径，即包含哪些探针
*/
list<ProbeLoad> ProbeSelectValueMax2::selectProbes()
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
	int maxValue = generateDPArray(dp, cap, probeLoads, probeLoads.size() - 1);

	// 根据dp[cap]递归倒推出路径
	m_probeSelected.clear();
	deduceProbes(maxValue, cap, probeLoads, probeLoads.size() - 1);

	return m_probeSelected;
}

int NetBrain::ProbeSelectValueMax2::generateDPArray(vector<int>& dp, int cap, const vector<ProbeLoad>& probeLoads, int idxP)
{
	// init dp
	std::fill(dp.begin(), dp.end(), 0);

	// generate dp array
	for (int i = 0; i <= idxP; i++) {
		// 从大到小遍历，因为在计算时，dp[i]存放的dp[i-1)]的数
		// 只需要遍历到Pi的负载数，因为再小不会加入
		for (int j = cap; j >= probeLoads[i].load; j--) {
			int resLeft = j - probeLoads[i].load;
			dp[j] = std::max(dp[j], probeLoads[i].value + dp[resLeft]);
		}
	}

	// dp[cap]即为最大贡献值
	LOG_DEBUG("The max value of [pi=%d, cap=%d] is:%d\n", idxP, cap, dp[cap]);
	LOG_DEBUG("%s\t", probeLoads[idxP].name.c_str());
	for (auto n : dp) {
		LOG_DEBUG("%d\t", n);
	}
	LOG_DEBUG("\n");
	
	return dp[cap];
}

int ProbeSelectValueMax2::deduceProbes(int maxValue, int cap, const vector<ProbeLoad>& probeLoads, int idxP)
{
	// recursion end condition
	if (0 == idxP) {
		m_probeSelected.push_back(probeLoads[0]);
		return (probeLoads[0].load > cap) ? 0 : probeLoads[0].value;
	}

	if (0 == cap) {
		return 0;
	}

	int resLeft = cap - probeLoads[idxP].load;
	if (resLeft < 0) {
		LOG_WARN("should not come here!\n");
		return -1;
	}

	// 获得前一个探针的最大贡献值数组
	vector<int> dpPreceding(cap + 1, 0);
	int precedingValue = generateDPArray(dpPreceding, cap, probeLoads, idxP - 1);

	// 没有加入本探针
	if (maxValue == dpPreceding[cap]) {
		return deduceProbes(dpPreceding[cap], cap, probeLoads, idxP - 1);
	}
	// 加入了本探针
	else {
		m_probeSelected.push_back(probeLoads[idxP]);
		return deduceProbes(dpPreceding[resLeft], resLeft, probeLoads, idxP - 1);
	}
}

ProbeSelectValueMax2::ProbeSelectValueMax2(const ProbeScheduler* probeScheduler)
	: m_probeScheduler(probeScheduler)
{
	if (NULL == m_probeScheduler) {
		throw std::invalid_argument("Lack of ProbeScheduler");
	}
}