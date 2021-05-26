/***********************************************************************
 * 贪心算法，找出接近最优的路径，适合负载与贡献值成正比关系的情况，越成正比，
 * 越接近最优方案。要求探针列表倒序存储
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
		* 绑定调度器，但不持有
		* @param probeScheduler 只读探针调度器
		*/
		ProbeSelectVGreedy(const ProbeScheduler* probeScheduler);

		virtual ~ProbeSelectVGreedy();

	private:

		// 缓存计算结果 - 选中的探针列表
		list<ProbeLoad> m_probeSelected;

		/**
		* 只读调度器
		*/
		const ProbeScheduler* m_probeScheduler;
	};
}