/***********************************************************************
 * 探针调度器，或许可以参考Yarn/Memos，本项目中不展开设计
 * 
 * @Author:  jason lee
 * @Date: 2021/5/21
 ***********************************************************************/
#pragma once

#include <vector>
#include <list>
#include "ProbeSelectStrategy.h"
#include "ProbeLoad.h"

using namespace std;

namespace NetBrain {
	class ProbeScheduler
	{
	public:
		// 最大系统资源容量，最大探针数量，未来从配置文件读取
		static const size_t MAX_RESOURCE_CAP = 100000;
		static const size_t MAX_PROBE_CAP = 100000;

	public:
		/**
		* 选择要调度的一组探针，根据选择策略
		* @return 所选探针列表
		*/
		list<ProbeLoad> selectProbes() const;

		/**
		* 设置新策略。将释放老策略对象，接管新策略对象
		* @param probeSelectStrategy 新策略对象
		*/
		void setProbeSelectStrategy(shared_ptr<ProbeSelectStrategy> probeSelectStrategy);

		/**
		* 重新加载探针负载列表，会清空之前列表，返回更新数量
		* @return -1 if open file error
		*/
		int reloadProbeLoads(const string& filename);

		// 判断一个资源负载是否符合本调度器要求
		bool isProbeLoadValid(const ProbeLoad& probeLoad);

		/**
		* 重置资源容量
		*/
		void resetCap(int cap);

		/**
		* @return 获得只读的探针列表
		*/
		const vector<ProbeLoad>& getProbeLoads() const;

		// get current system resource capability
		int getCap() const;

		ProbeScheduler(int cap = 10);

		virtual ~ProbeScheduler();

	private:
		ProbeScheduler(const ProbeScheduler&) = delete;
		ProbeScheduler& operator = (const ProbeScheduler&) = delete;

	private:
		/**
		* 探针选择策略
		*/
		shared_ptr<ProbeSelectStrategy> m_pProbeSelectStrategy;

		/**
		* 探针负载列表
		*/
		vector<ProbeLoad> m_probeLoads;

		/**
		* 系统资源容量
		*/
		int m_cap;
	};
}