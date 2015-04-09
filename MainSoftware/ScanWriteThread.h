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

	void setFilename(std::string _filename) { filename = _filename; };

protected:
	void run();

private:
	std::string filename;
	std::vector<PlyCloud *> m_scannedSequence;
};

