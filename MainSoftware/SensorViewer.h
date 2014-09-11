/* Used to view the scanned scene from PrimeSense Sensor
* Since we don't need the axis to view the scene
* Author: Jerome Jiang
* Date: 9/11/2014
*/

#pragma once
#include "meshviewer.h"

class SensorViewer :
	public MeshViewer
{
	Q_OBJECT
public:
	SensorViewer();
	~SensorViewer();

protected:
	// overwrite because we don't need axis in this scene
	void drawAxis();
};

