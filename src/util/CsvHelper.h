#pragma once

#include <vector>
#include "scheduler/ProbeLoad.h"

using namespace std;

namespace NetBrain {
	namespace CsvHelper {
		/**
		* ��ȡCSV�ļ����ݣ�����probeLoads�У����ԷǷ���¼������������������Բ���ԭ������
		* ������ļ�ʧ�ܣ���ԭ�����ݲ���Ӱ��
		* 
		* @param probeLoads��IN/OUT��̽�븺���б�
		* @param bTrunc����true���ڼ���ǰ���ԭ�����ݣ�����׷������
		* @return ����-1��������ļ�ʧ�ܣ�����Ϊ���������
		*/
		int loadCSV(const string& filename, vector<ProbeLoad>& probeLoads, size_t maxLine, bool bTrunc = true);
	};
};
