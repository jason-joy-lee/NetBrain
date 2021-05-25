/***********************************************************************
 * ̽��ѡ���㷨���Ӻ�ѡ̽���б���ѡ��һ����ʵ�̽�룬���Ը��������ֵ������صȣ�
 * �ò���ģʽ��װ�㷨�仯
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
		* ѡ��һ����ʵ�̽��
		* @return ��ѡ̽���б�
		*/
		virtual std::list<ProbeLoad> selectProbes() {
			return std::list<ProbeLoad>();
		}

		ProbeSelectStrategy() = default;

		virtual ~ProbeSelectStrategy() = default;
	};
}