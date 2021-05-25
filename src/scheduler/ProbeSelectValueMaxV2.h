/***********************************************************************
 * 在系统资源有限的情况下，选择贡献值最大的一组探针。
 *
 * @Author:  jason lee
 * @Date: 2021/5/21
 ***********************************************************************/
#pragma once

#include <list>
#include <vector>
#include "ProbeSelectStrategy.h"
#include "ProbeScheduler.h"

using namespace std;

namespace NetBrain {
	class ProbeSelectVGreedy : public ProbeSelectStrategy
	{
	public:
		/**
		* 在系统资源有限的情况下，根据探针的贡献值最大化选择一组探针
		* @return 探针列表
		*/
		virtual list<ProbeLoad> selectProbes();

		/**
		* 绑定调度器
		* @param probeScheduler 只读探针调度器
		*/
		ProbeSelectVGreedy(const ProbeScheduler* probeScheduler);

		virtual ~ProbeSelectVGreedy() {}

	private:
		/**
		* 生成并返回最大贡献值表dpTable
		*/
		void generateDPTable(vector<vector<int>>& dpTable, const vector<ProbeLoad>& probeLoads, int cap);

		/**
		* 根据dp[idxR]递归倒推出路径
		*/
		list<ProbeLoad> deduceProbes(vector<int>& dp, int idxR, const vector<ProbeLoad>& probeLoads, int pi);

	private:

		// 缓存计算结果 - 选中的探针列表
		list<ProbeLoad> m_probeSelected;

		/**
		* 只读调度器器
		*/
		const ProbeScheduler* m_probeScheduler;
	};
}