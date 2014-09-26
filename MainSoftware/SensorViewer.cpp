#include "SensorViewer.h"
#include <string>
#include <fstream>
#include <QInputDialog>
#include "header\eigen\Eigen\Dense"

SensorViewer::SensorViewer(openni::VideoStream &depth, openni::VideoStream &color, bool rgbToDepthRegConverter) :
m_depthStream(depth), m_rgbStream(color), m_streams(NULL)
{
	videoWidth = videoHeight = 0;
	numFile = 0;
	m_rgbToDepthRegConverter = rgbToDepthRegConverter;
	bool ok;
	maxDepthRange = QInputDialog::getInt(this, tr("Input the Maximum Depth Range"), tr("Max Depth Range"), 0, 0, 3000, 1, &ok);
}

SensorViewer::~SensorViewer()
{
	std::cout << "SensorViewer:destructor..." << std::endl;
	//getchar();
}

void SensorViewer::initSensorViewer()
{
	std::cout << "SensorViewer:initSensorViewer..." << std::endl;
	openni::VideoMode depthMode, rgbMode;

	if (m_depthStream.isValid() && m_rgbStream.isValid())
	{
		depthMode = m_depthStream.getVideoMode();
		rgbMode = m_rgbStream.getVideoMode();
		int depthWidth = depthMode.getResolutionX();
		int depthHeight = depthMode.getResolutionY();
		int rgbWidth = rgbMode.getResolutionX();
		int rgbHeight = rgbMode.getResolutionY();

		if ((depthWidth == rgbWidth) && (depthHeight == rgbHeight))
		{
			videoWidth = depthWidth;
			videoHeight = depthHeight;
		}
		else
		{
			std::cout << "sensorWindow: Error: Rgb and depth stream expected to be the same resolution." << std::endl;
			std::cout << "depthWidth " << depthWidth << " depthHeight " << depthHeight << std::endl;
			std::cout << "rgbWidth " << rgbWidth << " rgbHeight " << rgbHeight << std::endl;

			//return openni::STATUS_ERROR;
		}
	}
	else if (m_depthStream.isValid())
	{
		depthMode = m_depthStream.getVideoMode();
		int depthWidth = depthMode.getResolutionX();
		int depthHeight = depthMode.getResolutionY();
		videoWidth = depthWidth;
		videoHeight = depthHeight;
	}
	else if (m_rgbStream.isValid())
	{
		rgbMode = m_rgbStream.getVideoMode();
		int rgbWidth = rgbMode.getResolutionX();
		int rgbHeight = rgbMode.getResolutionY();
		videoWidth = rgbWidth;
		videoHeight = rgbHeight;
	}
	else
	{
		std::cout << "sensorWindow: Error: No Valid Video stream..." << std::endl;
		//return openni::STATUS_ERROR;
	}
	std::cout << "SensorViewer:initSensorViewer...almost finished" << std::endl;

	m_streams = new openni::VideoStream*[2];
	m_streams[0] = &m_depthStream;
	m_streams[1] = &m_rgbStream;

	std::cout << "SensorViewer:initSensorViewer...finished" << std::endl;

}

// overwrite
void SensorViewer::drawAxis()
{
	return;
}

void SensorViewer::run()
{
	QTimer * timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateDisplay()));
	timer->start(50);
	//updateGL();
}

void SensorViewer::updateDisplay()
{
	updateGL();
}

void SensorViewer::paintGL()
{
	//int changedIdx;
	std::cout << "SensorViewer: paintGL..." << std::endl;
	//openni::Status rc = openni::OpenNI::waitForAnyStream(m_streams, 2, &changedIdx);
	//std::cout << "3333333" << std::endl;

	//if (rc != openni::STATUS_OK)
	//{
	//	printf("Wait failed\n");
	//	return;
	//}

	//switch (changedIdx)
	//{
	//case 0:
	//	m_depthStream.readFrame(&m_depthFrame); break;
	//case 1:
	//	m_rgbStream.readFrame(&m_rgbFrame); break;
	//default:
	//	printf("Error in wait\n");
	//}

	/*std::ofstream depthOutput;
	depthOutput.open("depthOutput.txt");*/

	const openni::DepthPixel * depthCoorArray = NULL;
	if (m_depthStream.readFrame(&m_depthFrame) == openni::STATUS_OK)
	{
		depthCoorArray = static_cast<const openni::DepthPixel*>(m_depthFrame.getData());
	}
	else
	{
		std::cout << "SensorViewer: Can't read depth frame. Return." << std::endl;
		return;
	}

	const openni::RGB888Pixel * rgbCoorArray = NULL;
	if (m_rgbStream.readFrame(&m_rgbFrame) == openni::STATUS_OK)
	{	
		std::cout << "SensorViewer: Use RGB camera and Depth camera!" << std::endl;
		std::vector<CPoint> vertexList;
		std::vector<openni::RGB888Pixel> colorList;

		rgbCoorArray = static_cast<const openni::RGB888Pixel*>(m_rgbFrame.getData());
		for (int y = 0; y < m_depthFrame.getHeight(); y++)
		{
			for (int x = 0; x < m_depthFrame.getWidth(); x++)
			{
				int idx = x + y * m_depthFrame.getWidth();
				const openni::DepthPixel & zValue = depthCoorArray[idx];
				if (zValue != 0)
				{
					if (!m_rgbToDepthRegConverter)
					{
						int colorX, colorY;
						if (openni::CoordinateConverter::convertDepthToColor(m_depthStream, m_rgbStream, x, y, zValue, &colorX, &colorY) == openni::STATUS_OK)
						{
							if (colorX >= 0 && colorX<m_depthFrame.getWidth() && colorY >=0 && colorY<m_depthFrame.getHeight())
							{
								const openni::RGB888Pixel & colorValue = rgbCoorArray[colorX + colorY * m_depthFrame.getWidth()];

								float fx, fy, fz;
								openni::CoordinateConverter::convertDepthToWorld(m_depthStream, x, y, zValue, &fx, &fy, &fz);
								if (fz <= maxDepthRange)
								{
									CPoint newVertex((double)fx, (double)fy, (double)fz);
									//depthOutput << fz << std::endl;
									vertexList.push_back(newVertex);
									colorList.push_back(colorValue);
								}
							}
						}
					}
					else
					{
						const openni::RGB888Pixel &colorValue = rgbCoorArray[idx];

						float fx, fy, fz;
						openni::CoordinateConverter::convertDepthToWorld(m_depthStream, x, y, zValue, &fx, &fy, &fz);
						// limit the depth range
						if (fz <= maxDepthRange)
						{
							CPoint newVertex((double)fx, (double)fy, (double)fz);
							//depthOutput << fz << std::endl;
							vertexList.push_back(newVertex);
							colorList.push_back(colorValue);
						}
					}
				}
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		pointCloud = new PlyCloud(vertexList, colorList);
		setDefaultDrawMode();
		drawMesh();
		glPopMatrix();
	}
	// no rgb frame, draw the scene just without color
	else
	{
		//int numPixels = videoWidth * videoHeight;
		std::vector<CPoint> vertexList;

		if (depthCoorArray != NULL)
		{
			for (int y = 0; y < m_depthFrame.getHeight(); ++y)
			{
				for (int x = 0; x < m_depthFrame.getWidth(); ++x)
				{
					int idx = x + y * m_depthFrame.getWidth();
					const openni::DepthPixel & zValue = depthCoorArray[idx];
					if (zValue != 0)
					{
						float fx, fy, fz;
						openni::CoordinateConverter::convertDepthToWorld(m_depthStream, x, y, zValue, &fx, &fy, &fz);
						if (fz <= maxDepthRange)
						{
							CPoint newVertex((double)fx, (double)fy, (double)fz);
							//depthOutput << fz << std::endl;
							vertexList.push_back(newVertex);
						}
					}
				}
			}
		}

		pointCloud = new PlyCloud(vertexList);
		setDefaultDrawMode();
		std::cout << "SensorViewer: start drawing..." << std::endl;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		drawMesh();
		glPopMatrix();
	}
	//depthOutput.close();
	//system("PAUSE");
}

void SensorViewer::drawMesh()
{
	std::cout << "SensorViewer:drawmesh start" << std::endl;
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matProjection);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(matModelView);

	glDisable(GL_LIGHTING);

	if (pointCloud->hasColor())
	{
		drawMeshColorPoints();
	}
	else
	{
		drawMeshPoints();
	}
}

void SensorViewer::drawMeshColorPoints()
{
	std::vector<JVertex*> jVertexList = pointCloud->getJVertexList();
	std::vector<openni::RGB888Pixel> colorList = pointCloud->getColorList();

	std::vector<bool> deletedVertexList = pointCloud->get_deleted_vertex_list();

	for (size_t i = 0; i < jVertexList.size(); i++)
	{
		if (!deletedVertexList[i])
		{
			CPoint vert = jVertexList[i]->getPoint();
			openni::RGB888Pixel colorValue = colorList[i];

			if (jVertexList[i]->hasNormal())
			{
				CPoint norl = jVertexList[i]->getNormal();

				glPointSize(1.0f);
				glColor3d(colorValue.r/255.0, colorValue.g/255.0, colorValue.b/255.0);
				glBegin(GL_POINTS);
				glVertex3d(vert[0], vert[1], vert[2]);
				glNormal3d(norl[0], norl[1], norl[2]);
				glEnd();
			}
			else
			{
				glPointSize(1.0f);
				glColor3d(colorValue.r/255.0, colorValue.g/255.0, colorValue.b/255.0);
				glBegin(GL_POINTS);
				glVertex3d(vert[0], vert[1], vert[2]);
				glEnd();
			}
		}
	}
}

void SensorViewer::viewerStartScan()
{
	std::cout << "SensorViewer:startScan..." << std::endl;
	scanTimer = new QTimer(this);
	connect(scanTimer, SIGNAL(timeout()), this, SLOT(dataCollectionOneFrame()));
	scanTimer->start(500); 
}

void SensorViewer::viewerStopScan()
{
	scanTimer->stop();
}

void SensorViewer::dataCollectionOneFrame()
{
	std::cout << "SensorViewer:dataCollectionOneFrame" << std::endl;

	const openni::DepthPixel * depthCoorArray = NULL;
	if (m_depthStream.readFrame(&m_depthFrame) == openni::STATUS_OK)
	{
		depthCoorArray = static_cast<const openni::DepthPixel*>(m_depthFrame.getData());
	}
	else
	{
		std::cout << "SensorViewer: Can't read depth frame. Return." << std::endl;
		return;
	}

	// const openni::RGB888Pixel * rgbCoorArray = NULL;
	PlyCloud * pCloudToBeScanned;

	const openni::RGB888Pixel * rgbCoorArray = NULL;
	if (m_rgbStream.readFrame(&m_rgbFrame) == openni::STATUS_OK)
	{
		std::vector<CPoint> vertexList;
		std::vector<openni::RGB888Pixel> colorList;
		rgbCoorArray = static_cast<const openni::RGB888Pixel*>(m_rgbFrame.getData());

		if (depthCoorArray != NULL)
		{
			for (int y = 0; y < m_depthFrame.getHeight(); y++)
			{
				for (int x = 0; x < m_depthFrame.getWidth(); x++)
				{
					int idx = x + y * m_depthFrame.getWidth();
					const openni::DepthPixel & zValue = depthCoorArray[idx];
					if (zValue != 0)
					{
						if (! m_rgbToDepthRegConverter)
						{
							int colorX, colorY;
							if (openni::CoordinateConverter::convertDepthToColor(m_depthStream, m_rgbStream, x, y, zValue, &colorX, &colorY) == openni::STATUS_OK)
							{
								if (colorX >= 0 && colorX < m_depthFrame.getWidth() && colorY >= 0 && colorY < m_depthFrame.getHeight())
								{
									const openni::RGB888Pixel & colorValue = rgbCoorArray[colorX + colorY * m_depthFrame.getWidth()];
									float fx, fy, fz;
									openni::CoordinateConverter::convertDepthToWorld(m_depthStream, x, y, zValue, &fx, &fy, &fz);
									if (fz <= maxDepthRange)
									{
										CPoint newVertex((double)fx, (double)fy, (double)fz);
										vertexList.push_back(newVertex);
										colorList.push_back(colorValue);
									}
								}
							}
						}
						else
						{
							const openni::RGB888Pixel &colorValue = rgbCoorArray[idx];

							float fx, fy, fz;
							openni::CoordinateConverter::convertDepthToWorld(m_depthStream, x, y, zValue, &fx, &fy, &fz);
							// limit the depth range
							if (fz <= maxDepthRange)
							{
								CPoint newVertex((double)fx, (double)fy, (double)fz);
								//depthOutput << fz << std::endl;
								vertexList.push_back(newVertex);
								colorList.push_back(colorValue);
							}
						}
					}
				}
			}
		}

		pCloudToBeScanned = new PlyCloud(vertexList, colorList);
	}
	else
	{
		std::vector<CPoint> vertexList;

		if (depthCoorArray != NULL)
		{
			for (int y = 0; y < m_depthFrame.getHeight(); ++y)
			{
				for (int x = 0; x < m_depthFrame.getWidth(); ++x)
				{
					int idx = x + y * m_depthFrame.getWidth();
					const openni::DepthPixel & zValue = depthCoorArray[idx];
					if (zValue != 0)
					{
						float fx, fy, fz;
						openni::CoordinateConverter::convertDepthToWorld(m_depthStream, x, y, zValue, &fx, &fy, &fz);
						if (fz <= maxDepthRange)
						{
							CPoint newVertex((double)fx, (double)fy, (double)fz);
							//depthOutput << fz << std::endl;
							vertexList.push_back(newVertex);
						}
					}
				}
			}
		}

		pCloudToBeScanned = new PlyCloud(vertexList);
	}
	
	string scanName = "plyCloud";
	std::cout << scanName + std::to_string(numFile) << std::endl;
	pCloudToBeScanned->write_ply((scanName + std::to_string(numFile) + ".ply").c_str());
	std::cout << "Saved one frame to " << scanName + std::to_string(numFile) + ".ply" << std::endl;
	numFile++;
	getchar();
}