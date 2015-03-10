#pragma once

#include "meshviewer.h"
#include "OpenGLHeader.h"
#include <OpenNI.h>
#include <QTimer>
#include <fstream>

class SensorScanner : public QObject
{
	Q_OBJECT

public:
	SensorScanner(openni::VideoStream &depth, openni::VideoStream &color, bool rgbToDepthRegConverter, int _maxDepthRange);
	~SensorScanner();

	std::vector<PlyCloud *> getScannedSequence() { return scannedSequence; };

private:
	std::ofstream timerecord;
	int maxDepthRange;
	int numFile;

	bool stopped;

	QTimer * scanTimer;
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
	void printDebug();

};

