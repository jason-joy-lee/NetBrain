#include "ProbeSelectValueMax2.h"
#include "util/logger.h"
#include <sstream>

using namespace NetBrain;
using namespace std;

/**
* 1������ÿ��̽���ÿ����Դ��Ƭ��������������ֵ��ֻ�������һ��̽��ĸ�����Դ��Ƭ�����Ź���ֵ����
	1��״̬ת�Ʊ���ܴܺ�
	2����Դ��Ƭ�������ת��������
	3�����û�и��أ�����û����Դ������̽�����
* 2�����һ��̽������һ����Դ��Ƭ��Ϊ����Դ�޶��µ������ֵ
* 3�����������ֵ��״̬ת�Ʊ�����꣬�����Ƶ�����·������������Щ̽��
*/
list<ProbeLoad> ProbeSelectValueMax2::selectProbes()
{
	// ��ȡ̽�븺���б���Դ���������û�и��أ�����û����Դ������̽�����
	const vector<ProbeLoad>& probeLoads = m_probeScheduler->getProbeLoads();
	int cap = m_probeScheduler->getCap();
	if (probeLoads.empty() || cap <= 0) {
		LOG_WARN("Warning! There is no probes or resource\n");
		return list<ProbeLoad>();
	}

	// dp��ŵ�ǰ̽��Pi�ڸ��ָ�����Ƭ�µ������ֵ����ʼֵΪ0�����������1��Ԫ�أ��Ա������븺��һ�£�dp[0]����
	vector<int> dp(cap + 1, 0);
	int maxValue = generateDPArray(dp, cap, probeLoads, probeLoads.size() - 1);

	// ����dp[cap]�ݹ鵹�Ƴ�·��
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
		// �Ӵ�С��������Ϊ�ڼ���ʱ��dp[i]��ŵ�dp[i-1)]����
		// ֻ��Ҫ������Pi�ĸ���������Ϊ��С�������
		for (int j = cap; j >= probeLoads[i].load; j--) {
			int resLeft = j - probeLoads[i].load;
			dp[j] = std::max(dp[j], probeLoads[i].value + dp[resLeft]);
		}
	}

	// dp[cap]��Ϊ�����ֵ
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

	// ���ǰһ��̽��������ֵ����
	vector<int> dpPreceding(cap + 1, 0);
	int precedingValue = generateDPArray(dpPreceding, cap, probeLoads, idxP - 1);

	// û�м��뱾̽��
	if (maxValue == dpPreceding[cap]) {
		return deduceProbes(dpPreceding[cap], cap, probeLoads, idxP - 1);
	}
	// �����˱�̽��
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