/***********************************************************************
 * 动态规划算法，二维数组版
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
	class ProbeSelectValueMax : public ProbeSelectStrategy
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
		ProbeSelectValueMax(const ProbeScheduler* probeScheduler);

		virtual ~ProbeSelectValueMax();

	private:
		/**
		* 生成并返回最大贡献值表dpTable
		*/
		void generateDPTable(vector<vector<int>>& dpTable, const vector<ProbeLoad>& probeLoads, int cap);

		/**
		* 根据dpTable坐标<P(i), R(j)> ，即：第i个探针的第j个资源切片，推算出对应的探针列表
		*/
		size_t deduceProbes(vector<vector<int>>& dpTable, int idxP, int idxR, const vector<ProbeLoad>& probeLoads);

	private:

		// 缓存计算结果 - 选中的探针列表
		list<ProbeLoad> m_probeSelected;

		/**
		* 只读调度器器
		*/
		const ProbeScheduler* m_probeScheduler;
	};
}