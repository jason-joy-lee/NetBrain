#pragma once

#include <vector>
#include "scheduler/ProbeLoad.h"

using namespace std;

namespace NetBrain {
	namespace CsvHelper {
		/**
		* ��ȡCSV�ļ����ݣ�����probeLoads�У����ԷǷ���¼�������������
		* 
		* @return ����-1��������ļ�ʧ�ܣ�����Ϊ���������
		*/
		int loadCSV(const string& filename, vector<ProbeLoad>& probeLoads, size_t maxLine);
	};
};
