/***********************************************************************
 * ��ϵͳ��Դ���޵�����£�ѡ����ֵ����һ��̽�롣
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
		* ��ϵͳ��Դ���޵�����£�����̽��Ĺ���ֵ���ѡ��һ��̽��
		* @return ̽���б�
		*/
		virtual list<ProbeLoad> selectProbes();

		/**
		* �󶨵�����
		* @param probeScheduler ֻ��̽�������
		*/
		ProbeSelectVGreedy(const ProbeScheduler* probeScheduler);

		virtual ~ProbeSelectVGreedy() {}

	private:
		/**
		* ���ɲ����������ֵ��dpTable
		*/
		void generateDPTable(vector<vector<int>>& dpTable, const vector<ProbeLoad>& probeLoads, int cap);

		/**
		* ����dp[idxR]�ݹ鵹�Ƴ�·��
		*/
		list<ProbeLoad> deduceProbes(vector<int>& dp, int idxR, const vector<ProbeLoad>& probeLoads, int pi);

	private:

		// ��������� - ѡ�е�̽���б�
		list<ProbeLoad> m_probeSelected;

		/**
		* ֻ����������
		*/
		const ProbeScheduler* m_probeScheduler;
	};
}