#include "scheduler/ProbeSelectValueMax.h"
#include "scheduler/ProbeLoad.h"
#include "util/CsvHelper.h"
#include "scheduler/ProbeScheduler.h"
#include "util/logger.h"
//#include "scheduler/ProbeSelectValueMaxV2.h"
#include "scheduler/ProbeSelectVGreedy.h"

#include <fstream>
#include <sstream>
#include <string.h>

using namespace std;
using namespace NetBrain;

#define ASSERT(a, b, name) ( printf("%s --> %s\n", name, (a == b) ? "PASS" : "FAILED"))

// 计算引擎名称
string g_engine = "Greedy";
int g_maxLoad = 100, g_cap = 15;

string g_configFileName = "ProbeScheduler.config";
string g_testDataDir = "./";

void initConfig()
{
	ifstream ism(g_testDataDir + g_configFileName);
	if (!ism) {
		LOG_ERROR("Config file %s missing\n", g_configFileName.c_str());
		return;
	}

	ism >> g_maxLoad >> g_cap;
}

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
	LOG_INFO("\nTest: %s, resource cap=%d, file name=%s...\n"
		, testName.c_str()		
		, cap
		, filename.c_str());

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
	std::shared_ptr<ProbeSelectStrategy> pStrategy = nullptr;
	if (g_engine == "ValueMax") {
		pStrategy.reset( new ProbeSelectValueMax(&probeScheduler) );
	}
	else if (g_engine == "Greedy") {
		pStrategy.reset( new ProbeSelectVGreedy(&probeScheduler) );
	}
	else {
		LOG_ERROR("Unknown Strategy engine %s\n", g_engine.c_str());
		return -11;
	}
	probeScheduler.setProbeSelectStrategy(pStrategy);

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
void generateProbeLoads(const string& filename, int maxLoad, double rate)
{
	ofstream osm(filename);
	if (!osm) {
		LOG_ERROR("Failed to create the file %s to wrinte data\n", filename.c_str());
		return;
	}
		
	srand((unsigned)time(NULL));
	for (int i = 0; i < maxLoad; i++) {
		ProbeLoad probeLoad;
		probeLoad.load = rand() % maxLoad;
		probeLoad.name.append("P").append(std::to_string(i));
		probeLoad.value = (int)(probeLoad.load * rate);

		osm << probeLoad << endl;
	}

	osm.close();
}

bool isFileExist(const string& filename) {
	ifstream ism(filename);
	return ism.is_open();
}

int testPerformance(std::list<ProbeLoad>& probeSelected) {
	ostringstream osm;
	osm << "performance_test_" << g_maxLoad << "_" << g_cap;
	string sTestCaseName = osm.str();

	// test data
	string testDataFileName = g_testDataDir + sTestCaseName + ".csv";
	if (!isFileExist(testDataFileName)) {
		generateProbeLoads(testDataFileName, g_maxLoad, 1.5);
	}

	// run test case
	int errCode = runTestCase(sTestCaseName, testDataFileName, g_cap, probeSelected);
	//ASSERT(errCode, 2, testCaseName);
	dump(probeSelected, sTestCaseName.c_str());

	return errCode;
}


const char* usage = "test_data_dir=./ engine=Greedy  max_load  max_resource\n"
					"\t\tengine = ValueMax, Greedy";

int main(int argc, char* argv[]) {
	initConfig();

	// usage
	if (argc == 2 && strcmp("-h", argv[1]) == 0) {
		printf("Usage: %s %s\n", argv[0], usage);
		return 0;
	}

	// test data dir
	if (argc >= 2) {
		g_testDataDir = argv[1];
	}
	// engine
	if (argc >= 3) {
		g_engine = argv[2];
	}

	std::list<ProbeLoad> probeSelected;
	int errCode = 0;
	const char* testCaseName = NULL;
	string testDataFileName;

	testCaseName = "File Not Exist";
	testDataFileName = g_testDataDir + "test_data_xyz.csv";
	errCode = runTestCase(testCaseName, testDataFileName, 13, probeSelected);
	ASSERT(errCode, -1, testCaseName);

	testCaseName = "0 Probes Cap";
	testDataFileName = g_testDataDir + "test_data_empty.csv";
	errCode = runTestCase(testCaseName, testDataFileName, 13, probeSelected);
	dump(probeSelected, testCaseName);
	ASSERT(errCode, 0, testCaseName);

	testCaseName = "-1 Resource Cap";
	testDataFileName = g_testDataDir + "test_data_1.csv";
	errCode = runTestCase(testCaseName, testDataFileName, -1, probeSelected);
	ASSERT(errCode, 1, testCaseName);

	testCaseName = "0 Resource Cap";
	testDataFileName = g_testDataDir + "test_data_1.csv";
	errCode = runTestCase(testCaseName, "test_data_1.csv", 0, probeSelected);
	dump(probeSelected, testCaseName);
	ASSERT(errCode, 0, testCaseName);

	testCaseName = "Over Resource Cap";
	testDataFileName = g_testDataDir + "test_data_1.csv";
	int maxCap = ProbeScheduler::MAX_RESOURCE_CAP + 1;
	errCode = runTestCase("Over Resource Cap", "test_data_1.csv", maxCap, probeSelected);
	dump(probeSelected, testCaseName);
	ASSERT(errCode, -maxCap, "Over Resource Cap");

	// 正常场景-小规模
	testCaseName = "normal_scenario_small_3_5";
	testDataFileName = g_testDataDir + "test_data_2.csv";
	errCode = runTestCase(testCaseName, "./test_data_2.csv", 5, probeSelected);
	dump(probeSelected, testCaseName);
	ASSERT(errCode, 2, testCaseName);

	// extended performance test
	if (argc >= 5) {
		g_maxLoad = std::atoi(argv[3]);
		g_cap = std::atoi(argv[4]);
		testPerformance(probeSelected);
	}

	return 0;
}
