#include "SurfaceTrim.h"
#include "qmessagebox.h"

#ifndef FOR_RELEASE
#define FOR_RELEASE 1
#endif

SurfaceTrim::SurfaceTrim()
{
	smoothValue = 5;
	islandAreaRatio = 0.001f;
	trimValue = 6.0;
}


SurfaceTrim::~SurfaceTrim()
{
}

long long SurfaceTrim::EdgeKey(int key1, int key2)
{
	if (key1 < key2) return (((long long)key1) << 32) | ((long long)key2);
	else            return (((long long)key2) << 32) | ((long long)key1);
}

template< class Real >
inline Point3D< Real > CrossProduct(Point3D< Real > p1, Point3D< Real > p2){ return Point3D< Real >(p1[1] * p2[2] - p1[2] * p2[1], p1[2] * p2[0] - p1[0] * p2[2], p1[0] * p1[1] - p1[1] * p2[0]); }

bool SurfaceTrim::startSurfaceTrim(char * filename)
{
	float min, max;
	std::vector< PlyValueVertex< float > > vertices;
	std::vector< std::vector< int > > polygons;
	int ft;// , commentNum;
	//char** comments;
	bool readFlags[PlyValueVertex< float >::Components];
	std::cout << "Trimmer:PlyReadPolygon.." << std::endl;
	int plyReadSuccess = PlyReadPolygons(filename, vertices, polygons, PlyValueVertex< float >::Properties, PlyValueVertex< float >::Components, ft, readFlags); // , &comments, &commentNum, readFlags);
	if (plyReadSuccess == 0)
	{
		QMessageBox readFail;
		readFail.setText("Can't Load File to be Trimmed");
		readFail.exec();
		return false;
	}
	std::cout << "Trimmer:SmoothValues" << std::endl;
	for (int i = 0; i < smoothValue; i++)
	{
		SmoothValues(vertices, polygons);
	}

	min = max = vertices[0].value;
	for (size_t i = 0; i < vertices.size(); i++)
	{
		min = std::min< float >(min, vertices[i].value);
		max = std::max< float >(max, vertices[i].value);
	}
	printf("Value Range: [%f,%f]\n", min, max);

	hash_map< long long, int > vertexTable;
	std::vector< std::vector< int > > ltPolygons, gtPolygons;
	std::vector< bool > ltFlags, gtFlags;

	//double t = Time();
	std::cout << "Trimmer:SplitPolygon" << std::endl;
	for (size_t i = 0; i < polygons.size(); i++)
	{
		//std::cout << i << std::endl;
		SplitPolygon(polygons[i], vertices, &ltPolygons, &gtPolygons, &ltFlags, &gtFlags, vertexTable, trimValue);
	}

	if (islandAreaRatio > 0)
	{
		std::vector< std::vector< int > > _ltPolygons, _gtPolygons;
		std::vector< std::vector< int > > ltComponents, gtComponents;
		SetConnectedComponents(ltPolygons, ltComponents);
		SetConnectedComponents(gtPolygons, gtComponents);
		std::vector< double > ltAreas(ltComponents.size(), 0.0), gtAreas(gtComponents.size(), 0.0);
		std::vector< bool > ltComponentFlags(ltComponents.size(), false), gtComponentFlags(gtComponents.size(), false);
		double area = 0.0;
		for (size_t i = 0; i < ltComponents.size(); i++)
		{
			for (size_t j = 0; j < ltComponents[i].size(); j++)
			{
				ltAreas[i] += PolygonArea(vertices, ltPolygons[ltComponents[i][j]]);
				ltComponentFlags[i] = (ltComponentFlags[i] || ltFlags[ltComponents[i][j]]);
			}
			area += ltAreas[i];
		}

		for (size_t i = 0; i < gtComponents.size(); i++)
		{
			for (size_t j = 0; j < gtComponents[i].size(); j++)
			{
				gtAreas[i] += PolygonArea(vertices, gtPolygons[gtComponents[i][j]]);
				gtComponentFlags[i] = (gtComponentFlags[i] || gtFlags[gtComponents[i][j]]);
			}
			area += gtAreas[i];
		}

		for (size_t i = 0; i < ltComponents.size(); i++)
		{
			if (ltAreas[i] < area*islandAreaRatio && ltComponentFlags[i])
			{
				for (size_t j = 0; j < ltComponents[i].size(); j++)
				{
					_gtPolygons.push_back(ltPolygons[ltComponents[i][j]]);
				}
			}
			else
			{
				for (size_t j = 0; j < ltComponents[i].size(); j++)
				{
					_ltPolygons.push_back(ltPolygons[ltComponents[i][j]]);
				}
			}	
		}

		for (size_t i = 0; i < gtComponents.size(); i++)
		{
			if (gtAreas[i] < area*islandAreaRatio && gtComponentFlags[i])
			{
				for (size_t j = 0; j < gtComponents[i].size(); j++)
				{
					_ltPolygons.push_back(gtPolygons[gtComponents[i][j]]);
				}
			}
			else
			{
				for (size_t j = 0; j < gtComponents[i].size(); j++)
				{
					_gtPolygons.push_back(gtPolygons[gtComponents[i][j]]);
				}
			}
		}
		ltPolygons = _ltPolygons, gtPolygons = _gtPolygons;
	}
	std::cout << "Trimmer:Triangulate" << std::endl;
	std::vector< std::vector< int > > polys = ltPolygons;
	Triangulate(vertices, ltPolygons, polys), ltPolygons = polys;

	std::cout << "Trimmer:Triangulate 2" << std::endl;
	//std::vector< std::vector< int > > polys = gtPolygons;
	polys = gtPolygons;
	Triangulate(vertices, gtPolygons, polys), gtPolygons = polys;

	std::cout << "Trimmer:RemovingHangingVertices" << std::endl;
	RemoveHangingVertices(vertices, gtPolygons);
	std::wcout << "Trimmer:Writing..." << std::endl;
	PlyWritePolygons("reconOutputTrimmed.ply", vertices, gtPolygons, PlyValueVertex< float >::Properties, PlyValueVertex< float >::Components, ft); // , comments, commentNum);
	std::cout << "Finished Surface Trimming..." << std::endl;
	return true;
}