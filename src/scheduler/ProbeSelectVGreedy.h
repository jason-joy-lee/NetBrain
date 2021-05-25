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
		* ������ͣ�����end
		*/
		int sum(vector<ProbeLoad>::iterator begin, vector<ProbeLoad>::iterator end);

	private:

		// ��������� - ѡ�е�̽���б�
		list<ProbeLoad> m_probeSelected;

		/**
		* ֻ����������
		*/
		const ProbeScheduler* m_probeScheduler;
	};
}