#pragma once

#include <vector>
#include "scheduler/ProbeLoad.h"

using namespace std;

namespace NetBrain {
	namespace CsvHelper {
		/**
		* 读取CSV文件数据，存入probeLoads中，忽略非法记录，限制最大行数，可以擦除原有数据
		* 如果打开文件失败，则原有数据不受影响
		* 
		* @param probeLoads，IN/OUT，探针负载列表
		* @param bTrunc，若true则在加载前清空原有内容，否则追加数据
		* @return 返回-1，如果打开文件失败，否则为读入的行数
		*/
		int loadCSV(const string& filename, vector<ProbeLoad>& probeLoads, size_t maxLine, bool bTrunc = true);
	};
};
