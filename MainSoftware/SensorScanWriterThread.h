#pragma once

#include "meshviewer.h"
#include "OpenGLHeader.h"
#include <QThread>
#include <OpenNI.h>
#include <QTimer>
#include <fstream>

class SensorScanWriterThread : public QThread
{
	Q_OBJECT

public:
	SensorScanWriterThread(openni::VideoStream &depth, openni::VideoStream &color, bool rgbToDepthRegConverter, int _maxDepthRange);
	~SensorScanWriterThread();

	void stop();

	std::vector<PlyCloud *> getScannedSequence() { return scannedSequence; };

protected:
	// entry point
	void run();

private:
	std::ofstream timerecord;
	int maxDepthRange;
	int numFile;
	QTimer * scanTimer;
	bool stopped;
protected:
	openni::VideoStream & m_depthStream;
	openni::VideoStream & m_rgbStream;
	openni::VideoStream** m_streams;

	openni::VideoFrameRef m_depthFrame;
	openni::VideoFrameRef m_rgbFrame;

	bool m_rgbToDepthRegConverter;

	std::vector<PlyCloud *> scannedSequence;

private slots:
	void dataCollectionOneFrame();

};

