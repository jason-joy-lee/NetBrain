/***********************************************************************
 * 探针选择算法，从候选探针列表中选择一组合适的探针，可以根据最大贡献值、最大负载等，
 * 用策略模式封装算法变化
 *
 * @Author:  jason lee
 * @Date: 2021/5/21
 ***********************************************************************/
#pragma once

#include <list>
#include "ProbeLoad.h"

namespace NetBrain {
	class ProbeSelectStrategy {
	public:
		/**
		* 选择一组合适的探针
		* @return 所选探针列表
		*/
		virtual std::list<ProbeLoad> selectProbes() {
			return std::list<ProbeLoad>();
		}

		ProbeSelectStrategy() = default;

		virtual ~ProbeSelectStrategy() = default;
	};
}