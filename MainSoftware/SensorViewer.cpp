#include "SensorViewer.h"
#include <QTimer>

SensorViewer::SensorViewer(openni::VideoStream &depth, openni::VideoStream &color, bool rgbToDepthRegConverter) :
m_depthStream(depth), m_rgbStream(color), m_streams(NULL)
{
	videoWidth = videoHeight = 0;
	m_rgbToDepthRegConverter = rgbToDepthRegConverter;
}

SensorViewer::~SensorViewer()
{
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(matProjection);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(matModelView);

		if (isLightOn)
		{
			glEnable(GL_LIGHTING);
		}
		else
		{
			glDisable(GL_LIGHTING);
		}

		rgbCoorArray = static_cast<const openni::RGB888Pixel*>(m_rgbFrame.getData());
		for (int y = 0; y < m_depthFrame.getHeight(); y++)
		{
			for (int x = 0; x < m_depthFrame.getWidth(); x++)
			{
				int idx = x + y * m_depthFrame.getWidth();
				const openni::DepthPixel & zValue = depthCoorArray[idx];
				if (zValue != 0)
				{
					if (m_rgbToDepthRegConverter)
					{
						int colorX, colorY;
						if (openni::CoordinateConverter::convertDepthToColor(m_depthStream, m_rgbStream, x, y, zValue, &colorX, &colorY) == openni::STATUS_OK)
						{
							if (colorX >= 0 && colorX<m_depthFrame.getWidth() && colorY >=0 && colorY<m_depthFrame.getHeight())
							{
								const openni::RGB888Pixel & colorValue = rgbCoorArray[colorX + colorY * m_depthFrame.getWidth()];

								float fx, fy, fz;
								openni::CoordinateConverter::convertDepthToWorld(m_depthStream, x, y, zValue, &fx, &fy, &fz);

								std::cout << "SensorViewer: start drawing..." << std::endl;
								glPointSize(10);
								glColor3ub(colorValue.r, colorValue.g, colorValue.b);
								glBegin(GL_POINTS);
								glVertex3f(fx, fy, fz);
								glEnd();

								CPoint newVertex((double)fx, (double)fy, (double)fz);
								vertexList.push_back(newVertex);
							}
						}
					}
					else
					{
						const openni::RGB888Pixel &colorValue = rgbCoorArray[idx];

						float fx, fy, fz;
						openni::CoordinateConverter::convertDepthToWorld(m_depthStream, x, y, zValue, &fx, &fy, &fz);

						glPointSize(10);
						glColor3ub(colorValue.r, colorValue.g, colorValue.b);
						glBegin(GL_POINTS);
						glVertex3f(fx, fy, fz);
						glEnd();

						CPoint newVertex((double)fx, (double)fy, (double)fz);
						vertexList.push_back(newVertex);
					}
				}
			}
		}
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
						CPoint newVertex((double)fx, (double)fy, (double)fz);
						vertexList.push_back(newVertex);
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
}