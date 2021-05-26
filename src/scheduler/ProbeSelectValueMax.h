/***********************************************************************
 * ��̬�滮�㷨����ά�����
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
		* ��ϵͳ��Դ���޵�����£�����̽��Ĺ���ֵ���ѡ��һ��̽��
		* @return ̽���б�
		*/
		virtual list<ProbeLoad> selectProbes();

		/**
		* �󶨵�����
		* @param probeScheduler ֻ��̽�������
		*/
		ProbeSelectValueMax(const ProbeScheduler* probeScheduler);

		virtual ~ProbeSelectValueMax();

	private:
		/**
		* ���ɲ����������ֵ��dpTable
		*/
		void generateDPTable(vector<vector<int>>& dpTable, const vector<ProbeLoad>& probeLoads, int cap);

		/**
		* ����dpTable����<P(i), R(j)> ��������i��̽��ĵ�j����Դ��Ƭ���������Ӧ��̽���б�
		*/
		size_t deduceProbes(vector<vector<int>>& dpTable, int idxP, int idxR, const vector<ProbeLoad>& probeLoads);

	private:

		// ��������� - ѡ�е�̽���б�
		list<ProbeLoad> m_probeSelected;

		/**
		* ֻ����������
		*/
		const ProbeScheduler* m_probeScheduler;
	};
}