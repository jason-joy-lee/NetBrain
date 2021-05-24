#include "ProbeSelectValueMaxV2.h"
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
list<ProbeLoad> ProbeSelectValueMaxV2::selectProbes()
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

	for (int i = 0; i < probeLoads.size(); i++) {
		// �Ӵ�С��������Ϊ�ڼ���ʱ��dp[i]��ŵ�dp[i-1)]����
		// ֻ��Ҫ������Pi�ĸ���������Ϊ��С�������
		for (int j = cap; j >= probeLoads[i].load; j--) {
			int resLeft = j - probeLoads[i].load;
			dp[j] = std::max(dp[j], probeLoads[i].value + dp[resLeft]);
		}
	}

	// dp[cap]��Ϊ�����ֵ
	LOG_ERROR("�����ֵΪ:%d\n", dp[cap]);

	// ����dp[cap]�ݹ鵹�Ƴ�·��
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
			if (resLeft <= 0) {	// û��ʣ����Դ
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