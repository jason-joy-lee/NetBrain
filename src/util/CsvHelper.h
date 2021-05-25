#pragma once

#include <vector>
#include "scheduler/ProbeLoad.h"

using namespace std;

namespace NetBrain {
	namespace CsvHelper {
		/**
		* 读取CSV文件数据，存入probeLoads中，忽略非法记录，限制最大行数
		* 
		* @return 返回-1，如果打开文件失败，否则为读入的行数
		*/
		int loadCSV(const string& filename, vector<ProbeLoad>& probeLoads, size_t maxLine);
	};
};
