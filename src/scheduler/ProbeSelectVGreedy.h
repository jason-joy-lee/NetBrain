/***********************************************************************
 * ̰���㷨���ҳ��ӽ����ŵ�·�����ʺϸ����빱��ֵ�����ȹ�ϵ�������Խ�����ȣ�
 * Խ�ӽ����ŷ�����Ҫ��̽���б���洢
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
		* �󶨵���������������
		* @param probeScheduler ֻ��̽�������
		*/
		ProbeSelectVGreedy(const ProbeScheduler* probeScheduler);

		virtual ~ProbeSelectVGreedy();

	private:

		// ��������� - ѡ�е�̽���б�
		list<ProbeLoad> m_probeSelected;

		/**
		* ֻ��������
		*/
		const ProbeScheduler* m_probeScheduler;
	};
}