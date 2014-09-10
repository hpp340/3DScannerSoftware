#pragma once
#include <QWidget>
#include "meshviewer.h"
#include <QVboxLayout>
#include <OpenNI.h>

class sensorWindow :
	public QWidget
{
	Q_OBJECT
public:
	sensorWindow();
	~sensorWindow();

private:
	void initSensorWindow();
	void startSensor();

private:
	MeshViewer * sensorViewer;
};

