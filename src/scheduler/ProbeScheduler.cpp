#include "ProbeLoad.h"
#include "ProbeScheduler.h"
#include "util/CsvHelper.h"
#include "util/logger.h"

#include <fstream>
#include <sstream>
#include <algorithm>

using namespace NetBrain;

std::list<ProbeLoad> ProbeScheduler::selectProbes() const
{
	return m_pProbeSelectStrategy->selectProbes();
}

void ProbeScheduler::setProbeSelectStrategy(shared_ptr<ProbeSelectStrategy> probeSelectStrategy)
{	
	//delete m_pProbeSelectStrategy;
	m_pProbeSelectStrategy = probeSelectStrategy;
}

int ProbeScheduler::reloadProbeLoads(const string& filename)
{
	int num = CsvHelper::loadCSV(filename, m_probeLoads, MAX_PROBE_CAP, true);

	// 删除重名探针，先按名称排列，再删除相邻相同元素
	std::sort(m_probeLoads.begin(), m_probeLoads.end(), NetBrain::isSameProbe);
	m_probeLoads.erase( std::unique(m_probeLoads.begin(), m_probeLoads.end(), NetBrain::isSameProbe), m_probeLoads.end() );
	
	// 按照探针负载降序排列
	std::sort(m_probeLoads.begin(), m_probeLoads.end(), NetBrain::isGreatByLoad);

	return num;
}

bool NetBrain::ProbeScheduler::isProbeLoadValid(const ProbeLoad& probeLoad)
{
	return (!probeLoad.name.empty() && probeLoad.load >= 0 && probeLoad.value >= 0);
}

void NetBrain::ProbeScheduler::resetCap(size_t cap)
{
	if (cap > MAX_RESOURCE_CAP) {
		LOG_WARN("Unsupported resource capability, ignored\n");
		return;
	}

	if (0 == cap) {
		LOG_WARN("Resource capability was set to 0\n");
	}

	m_cap = cap;
}

const std::vector<ProbeLoad>& NetBrain::ProbeScheduler::getProbeLoads() const
{
	return m_probeLoads;
}

size_t NetBrain::ProbeScheduler::getCap() const
{
	return m_cap;
}

NetBrain::ProbeScheduler::ProbeScheduler(size_t cap)
	:m_pProbeSelectStrategy(new ProbeSelectStrategy())
	, m_cap(0)
{
	resetCap(cap);
}

NetBrain::ProbeScheduler::~ProbeScheduler()
{
	//delete m_pProbeSelectStrategy;
}
