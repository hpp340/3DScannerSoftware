#pragma once

#include "OpenGLHeader.h"
#include <QThread>
#include <QTimer>
#include <fstream>
#include "header\PlyCloud.h"

class ScanWriteThread : public QThread
{
	Q_OBJECT
public:
	ScanWriteThread(std::vector<PlyCloud *> scannedSequence);
	~ScanWriteThread();

	void stop();

protected:
	void run();

private:

	std::vector<PlyCloud *> m_scannedSequence;
};

