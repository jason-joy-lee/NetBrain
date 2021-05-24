#include "ProbeLoad.h"
#include "ProbeScheduler.h"
#include "../util/CsvHelper.h"

#include <fstream>
#include <sstream>

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
	return CsvHelper::loadCSV(filename, m_probeLoads, MAX_PROBE_CAP);
}

bool NetBrain::ProbeScheduler::isProbeLoadValid(const ProbeLoad& probeLoad)
{
	return (!probeLoad.name.empty() && probeLoad.load >= 0 && probeLoad.value >= 0);
}

void NetBrain::ProbeScheduler::resetCap(int cap)
{
	if (cap < 0 || cap > MAX_RESOURCE_CAP) {
		LOG_WARN("Unsupported resource capability, ignored\n");
		return;
	}

	if (0 == cap) {
		LOG_WARN("resource capability was set to 0\n");
	}

	m_cap = cap;
}

const std::vector<ProbeLoad>& NetBrain::ProbeScheduler::getProbeLoads() const
{
	return m_probeLoads;
}

int NetBrain::ProbeScheduler::getCap() const
{
	return m_cap;
}

NetBrain::ProbeScheduler::ProbeScheduler(int cap)
	:m_pProbeSelectStrategy(new ProbeSelectStrategy())
{
	resetCap(cap);
}

NetBrain::ProbeScheduler::~ProbeScheduler()
{
	//delete m_pProbeSelectStrategy;
}
