/***********************************************************************
 * ��̬�滮�㷨��һά�����
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
	class ProbeSelectValueMax2 : public ProbeSelectStrategy
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
		ProbeSelectValueMax2(const ProbeScheduler* probeScheduler);

		virtual ~ProbeSelectValueMax2() {}

	private:
		/**
		* ���ɲ����������ֵ����dp���Լ����ֵ
		*/
		int generateDPArray(vector<int>& dp, int cap, const vector<ProbeLoad>& probeLoads, int idxP);

		/**
		* ����dp[idxR]�ݹ鵹�Ƴ�·��
		*/
		int deduceProbes(int maxValue, int cap, const vector<ProbeLoad>& probeLoads, int idxP);

	private:

		// ��������� - ѡ�е�̽���б�
		list<ProbeLoad> m_probeSelected;

		/**
		* ֻ����������
		*/
		const ProbeScheduler* m_probeScheduler;
	};
}