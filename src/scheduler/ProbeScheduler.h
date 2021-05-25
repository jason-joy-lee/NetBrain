/***********************************************************************
 * ̽���������������Բο�Yarn/Memos������Ŀ�в�չ�����
 * 
 * @Author:  jason lee
 * @Date: 2021/5/21
 ***********************************************************************/
#pragma once

#include <vector>
#include <list>
#include <memory>
#include "ProbeSelectStrategy.h"
#include "ProbeLoad.h"

using namespace std;

namespace NetBrain {
	class ProbeScheduler
	{
	public:
		// ���ϵͳ��Դ���������̽��������δ���������ļ���ȡ
		static const size_t MAX_RESOURCE_CAP = 1000*100;
		static const size_t MAX_PROBE_CAP = 1000*100;

	public:
		/**
		* ѡ��Ҫ���ȵ�һ��̽�룬����ѡ�����
		* @return ��ѡ̽���б�
		*/
		list<ProbeLoad> selectProbes() const;

		/**
		* �����²��ԡ����ͷ��ϲ��Զ��󣬽ӹ��²��Զ���
		* @param probeSelectStrategy �²��Զ���
		*/
		void setProbeSelectStrategy(shared_ptr<ProbeSelectStrategy> probeSelectStrategy);

		/**
		* ���¼���̽�븺���б������֮ǰ�б����ظ�������
		* 1��̽�벻������
		* 2��Ĭ�ϰ��ո��ص�������
		* @return -1 if open file error
		*/
		int reloadProbeLoads(const string& filename);

		// �ж�һ����Դ�����Ƿ���ϱ�������Ҫ��
		bool isProbeLoadValid(const ProbeLoad& probeLoad);

		/**
		* ������Դ����
		*/
		void resetCap(size_t cap);

		/**
		* @return ���ֻ����̽���б�
		*/
		const vector<ProbeLoad>& getProbeLoads() const;

		// get current system resource capability
		size_t getCap() const;

		ProbeScheduler(size_t cap = 10);

		virtual ~ProbeScheduler();

	private:
		ProbeScheduler(const ProbeScheduler&) = delete;
		ProbeScheduler& operator = (const ProbeScheduler&) = delete;

	private:
		/**
		* ̽��ѡ�����
		*/
		shared_ptr<ProbeSelectStrategy> m_pProbeSelectStrategy;

		/**
		* ̽�븺���б�
		*/
		vector<ProbeLoad> m_probeLoads;

		/**
		* ϵͳ��Դ����
		*/
		size_t m_cap;
	};
}