#include "../scheduler/ProbeSelectValueMax.h"
#include "../scheduler/ProbeLoad.h"
#include "../util/CsvHelper.h"
#include "../scheduler/ProbeScheduler.h"
//#include "../scheduler/ProbeSelectValueMaxV2.h"

#include <fstream>
#include <sstream>

using namespace std;
using namespace NetBrain;

#define ASSERT(a, b, name) ( printf("%s --> %s\n", name, (a == b) ? "PASS" : "FAILED"))

void dump(const std::list<ProbeLoad>& path, const char* testCaseName)
{
	LOG_INFO("dump selected probes, test case name=%s:\n", testCaseName);
	for (ProbeLoad probeSelected : path) {
		ostringstream osm;
		osm << probeSelected;
		LOG_INFO("%s\n", osm.str().c_str());
	}
}

/*
* 1、如果加载文件失败，返回io系统定义的错误码
* 2、如果设置Cap失败，返回-cap
* 3、否则返回被选中的探针数量，以及被选中的探针列表
*/
int runTestCase(const string& testName, const string& filename, int cap, std::list<ProbeLoad>& probeSelected)
{
	LOG_INFO("\nUnit Test: %s, file name=%s, resource cap=%d...\n"
			, testName.c_str(), filename.c_str(), cap);

	// scheduler, load probe loads and set cap
	ProbeScheduler probeScheduler;
	probeScheduler.resetCap(cap);
	if (cap != probeScheduler.getCap()) {
		LOG_INFO("Set Probe Scheduler cap %d failed", cap);
		return -cap;
	}

	int numLine = probeScheduler.reloadProbeLoads(filename);
	if (numLine <= 0) {
		return numLine;
	}

	// ProbeSelectStrategy
	shared_ptr<ProbeSelectStrategy> p(new ProbeSelectValueMax(&probeScheduler));
	//shared_ptr<ProbeSelectStrategy> p(new ProbeSelectValueMaxV2(&probeScheduler));
	probeScheduler.setProbeSelectStrategy(p);

	// find the optimal path, count cpu clock
	clock_t start, finish;
	start = clock();
	probeSelected = probeScheduler.selectProbes();
	finish = clock();
	LOG_INFO("Algorithm time(second)=%.4f, starting clock=%ld, finishing clock=%ld\n"
			, (double)((finish - start) / CLOCKS_PER_SEC)
			, start
			, finish);

	return probeSelected.size();
}

// 生成大测试数据
string generateProbeLoads(const string& testDataDir, int numLine, double rate, int maxLoad)
{
	string filename(testDataDir);
	filename.append("performance_test_data.csv");
	ofstream osm(filename);
	if (!osm) {
		LOG_ERROR("Failed to create the file %s to wrinte data\n", filename.c_str());
		return "";
	}

	LOG_INFO("Generating performance test data that stored in %s\n", filename.c_str());
		
	srand((unsigned)time(NULL));
	for (int i = 0; i < numLine; i++) {
		ProbeLoad probeLoad;
		probeLoad.load = rand() % maxLoad;
		probeLoad.name.append("P").append(std::to_string(i));
		probeLoad.value = (int)(probeLoad.load * rate);

		osm << probeLoad << endl;
	}

	osm.close();
	return filename;
}

/*
* appname test_data_dir
*/
int main(int argc, char* argv[]) {
	std::list<ProbeLoad> probeSelected;
	int errCode = 0;
	const char* testCaseName = NULL;
	string testDataDir = "./";
	string testDataFileName;

	if (argc >= 2) {
		testDataDir = argv[1];
	}

	testCaseName = "File Not Exist";
	testDataFileName = testDataDir.append("test_data_xyz.csv");
	errCode = runTestCase(testCaseName, testDataFileName, 13, probeSelected);
	ASSERT(errCode, -1, testCaseName);

	testCaseName = "0 Probes Cap";
	testDataFileName = testDataDir.append("test_data_empty.csv");
	errCode = runTestCase(testCaseName, testDataFileName, 13, probeSelected);
	ASSERT(errCode, 0, testCaseName);
	dump(probeSelected, testCaseName);

	testCaseName = "-1 Resource Cap";
	testDataFileName = testDataDir.append("test_data_1.csv");
	errCode = runTestCase(testCaseName, testDataFileName, -1, probeSelected);
	ASSERT(errCode, 1, testCaseName);

	testCaseName = "0 Resource Cap";
	testDataFileName = testDataDir.append("test_data_1.csv");
	errCode = runTestCase(testCaseName, "test_data_1.csv", 0, probeSelected);
	ASSERT(errCode, 0, testCaseName);
	dump(probeSelected, testCaseName);

	testCaseName = "Over Resource Cap";
	testDataFileName = testDataDir.append("test_data_1.csv");
	errCode = runTestCase("Over Resource Cap", "test_data_1.csv", ProbeScheduler::MAX_RESOURCE_CAP + 1, probeSelected);
	ASSERT(errCode, 0-(ProbeScheduler::MAX_RESOURCE_CAP + 1), "Over Resource Cap");
	dump(probeSelected, testCaseName);
	

	// 正常场景-小规模
	testCaseName = "Normal Scenario-Small(P=3，T=5)";
	testDataFileName = testDataDir.append("test_data_2.csv");
	errCode = runTestCase(testCaseName, "./test_data_2.csv", 5, probeSelected);
	ASSERT(errCode, 2, testCaseName);
	dump(probeSelected, testCaseName);

	// 压力测试
	int numLine = 1000*10, maxLoad = 100, cap = 1000;
	ostringstream osm;
	osm << "Performance Test(P=" << numLine << ", T=" << cap << ")";
	string sTestCaseName = osm.str();
		
	testDataFileName = generateProbeLoads(testDataDir, numLine, 1.5, maxLoad);
	errCode = runTestCase(sTestCaseName, testDataFileName, cap, probeSelected);
	//ASSERT(errCode, 2, testCaseName);
	dump(probeSelected, sTestCaseName.c_str());

	return 0;
}
