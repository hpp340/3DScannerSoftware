#include "SensorScanWriterThread.h"


SensorScanWriterThread::SensorScanWriterThread(openni::VideoStream &depth, openni::VideoStream &color, bool rgbToDepthRegConverter, int _maxDepthRange) :
m_depthStream(depth), m_rgbStream(color), m_streams(NULL)
{
	std::cout << "SensorScanWriterThread: construct ..." << std::endl;
	timerecord.open("timerecord.txt");
	maxDepthRange = _maxDepthRange;
	m_rgbToDepthRegConverter = rgbToDepthRegConverter;
	numFile = 0;
	stopped = false;
}

SensorScanWriterThread::~SensorScanWriterThread()
{
}

void SensorScanWriterThread::run()
{
	std::cout << "SensorScanWriterThread: run..." << std::endl;
	timer = new CPPTimer();
	connect(timer, SIGNAL(triggered()), this, SLOT(dataCollectionOneFrame()), Qt::DirectConnection);
	timer->start();
	//while (!stopped)
	//{
		//int current = GetTickCount();

		//dataCollectionOneFrame();
		//int now = GetTickCount();
		//std::cout << "Thread time inter " << now - current << std::endl;
		//Sleep(250);
	//}
	//exec();
	//scanTimer = new QTimer(this);
	//connect(scanTimer, SIGNAL(timeout()), this, SLOT(dataCollectionOneFrame()), Qt::DirectConnection);
	//scanTimer->start(250);

}

void SensorScanWriterThread::stop()
{
	//scanTimer->stop();
	timerecord.close();
	timer->stop();
	stopped = true;
	this->quit();
}

void SensorScanWriterThread::dataCollectionOneFrame()
{
	std::cout << "SensorViewer:dataCollectionOneFrame" << std::endl;
	int currentTime;
	//currentTime = GetTickCount();
	std::cout << GetTickCount() << std::endl;
	const openni::DepthPixel * depthCoorArray = NULL;
	if (m_depthStream.readFrame(&m_depthFrame) == openni::STATUS_OK)
	{
		depthCoorArray = static_cast<const openni::DepthPixel*>(m_depthFrame.getData());
	}
	else
	{
		std::cout << "SensorViewer: Can't read depth frame. Return." << std::endl;
		getchar();
		return;
	}

	// const openni::RGB888Pixel * rgbCoorArray = NULL;
	PlyCloud * pCloudToBeScanned;

	const openni::RGB888Pixel * rgbCoorArray = NULL;
	if (m_rgbStream.readFrame(&m_rgbFrame) == openni::STATUS_OK)
	{
		std::vector<CPoint> vertexList;
		std::vector<JColor> colorList;
		rgbCoorArray = static_cast<const openni::RGB888Pixel*>(m_rgbFrame.getData());

		if (depthCoorArray != NULL)
		{
			// get current time in millisecond
			currentTime = GetTickCount();
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
								if (colorX >= 0 && colorX < m_depthFrame.getWidth() && colorY >= 0 && colorY < m_depthFrame.getHeight())
								{
									const openni::RGB888Pixel & colorValue = rgbCoorArray[colorX + colorY * m_depthFrame.getWidth()];
									JColor j_color;
									j_color.red = colorValue.r; j_color.green = colorValue.g; j_color.blue = colorValue.b;
									float fx, fy, fz;
									openni::CoordinateConverter::convertDepthToWorld(m_depthStream, x, y, zValue, &fx, &fy, &fz);
									if (fz <= maxDepthRange)
									{
										CPoint newVertex((double)fx, (double)fy, (double)fz);
										vertexList.push_back(newVertex);
										colorList.push_back(j_color);
									}
								}
							}
						}
						else
						{
							const openni::RGB888Pixel &colorValue = rgbCoorArray[idx];
							JColor j_color;
							j_color.red = colorValue.r; j_color.green = colorValue.g; j_color.blue = colorValue.b;
							float fx, fy, fz;
							openni::CoordinateConverter::convertDepthToWorld(m_depthStream, x, y, zValue, &fx, &fy, &fz);
							// limit the depth range
							if (fz <= maxDepthRange)
							{
								CPoint newVertex((double)fx, (double)fy, (double)fz);
								//depthOutput << fz << std::endl;
								vertexList.push_back(newVertex);
								colorList.push_back(j_color);
							}
						}
					}
				}
			}
		}

		pCloudToBeScanned = new PlyCloud(vertexList, colorList);
		//pCloudToBeScanned->normalize();
	}
	else
	{
		std::vector<CPoint> vertexList;

		if (depthCoorArray != NULL)
		{
			currentTime = GetTickCount();
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
		//pCloudToBeScanned->normalize();
	}

	string scanName = "scanned_point_cloud";
	std::cout << scanName + std::to_string(numFile) << std::endl;
	pCloudToBeScanned->write_ply((scanName + std::to_string(numFile) + ".ply").c_str());
	timerecord << numFile << " " << currentTime << std::endl;
	std::cout << "Saved one frame to " << scanName + std::to_string(numFile) + ".ply" << std::endl;
	numFile++;
	//getchar();
}