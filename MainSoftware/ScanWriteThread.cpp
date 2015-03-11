#include "ScanWriteThread.h"


ScanWriteThread::ScanWriteThread(std::vector<PlyCloud *> scannedSequence)
{
	m_scannedSequence = scannedSequence;
	filename = "writeThread_";
}


ScanWriteThread::~ScanWriteThread()
{
}

void ScanWriteThread::run()
{
	int i = 0;
	for (std::vector<PlyCloud *>::iterator ptIter = m_scannedSequence.begin(); ptIter != m_scannedSequence.end(); ptIter++, i++)
	{
		PlyCloud * currPtCloud = *ptIter;
		std::string newFilename = filename + std::to_string(i) + ".ply";
		currPtCloud->write_ply(newFilename.c_str());
	}

	stop();
}

void ScanWriteThread::stop()
{
	this->quit();
}
