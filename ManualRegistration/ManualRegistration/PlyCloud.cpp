#include "stdafx.h"
#include "header\PlyCloud.h"

PlyCloud::PlyCloud()
{
}

// overload constructor
PlyCloud::PlyCloud(std::vector<CPoint> newVertexList, std::vector<CPoint> newNormalList)
{
	vertex_list = newVertexList;
	normal_list = newNormalList;
}

// overload constructor
PlyCloud::PlyCloud(std::vector<CPoint> newVertexList)
{
	vertex_list = newVertexList;
}

PlyCloud::~PlyCloud()
{
}

bool PlyCloud::read_ply(const char * filename)
{
	if (vertex_list.size() > 0) // already read have the point cloud
	{
		return false;
	}
	std::ifstream plyfile;
	plyfile.open(filename);
	if (plyfile.fail())
	{
		std::cout << "Error:Can't open ply file." << std::endl;
		return false;
	}
	string inS;
	getline(plyfile, inS);
	// on this stage, we just ommit the header of ply file
	while (inS != "end_header")
	{
		getline(plyfile, inS);
	}
	// read in the vertex position and normal
	while (! plyfile.eof())
	{
		getline(plyfile, inS);
		int firstSpace = inS.find(" ", 0);
		int secondSpace = inS.find(" ", firstSpace+1);
		int thirdSpace = inS.find(" ", secondSpace+1);
		int fourthSpace = inS.find(" ", thirdSpace + 1);
		int fifthSpace = inS.find(" ", fourthSpace + 1);
		string firstPosition = inS.substr(0, firstSpace);
		string secondPosition = inS.substr(firstSpace + 1, secondSpace - firstSpace);
		string thirdPosition = inS.substr(secondSpace + 1, thirdSpace - secondSpace);
		string normalFirstPosition = inS.substr(thirdSpace + 1, fourthSpace - thirdSpace);
		string normalSecondPosition = inS.substr(fourthSpace + 1, fifthSpace - fourthSpace);
		string normalThirdPosition = inS.substr(fifthSpace + 1, inS.length() - fifthSpace);

		//cout << inS << endl;
		//cout << firstSpace << firstPosition << endl;
		//cout << secondSpace << secondPosition << endl;
		//cout << thirdSpace << thirdPosition << endl;

		double x = atof(firstPosition.c_str());
		double y = atof(secondPosition.c_str());
		double z = atof(thirdPosition.c_str());
		double nx = atof(normalFirstPosition.c_str());
		double ny = atof(normalSecondPosition.c_str());
		double nz = atof(normalThirdPosition.c_str());

		// cout << x << y << z << endl;
		//system("PAUSE");

		CPoint vertexPosition;
		vertexPosition[0] = x; vertexPosition[1] = y; vertexPosition[2] = z;
		CPoint normalPosition;
		normalPosition[0] = nx; normalPosition[1] = ny; normalPosition[2] = nz;
		add_VertNorm(vertexPosition, normalPosition);
	}
	plyfile.close();
	return true;
}

int PlyCloud::add_VertNorm(CPoint v, CPoint n)
{
	vertex_list.push_back(v);
	normal_list.push_back(n);
	return 1;
}

int PlyCloud::write_ply(const char *filename)
{
	std::ofstream outputPlyFile;
	outputPlyFile.open(filename);
	if (outputPlyFile.fail())
	{
		std::cout << "Error : Can't new output file." << std::endl;
		return 0;
	}
	// write header of ply file
	outputPlyFile << "ply" << endl;
	outputPlyFile << "format ascii 1.0" << endl;
	outputPlyFile << "element vertex " << vertex_list.size() << endl;
	outputPlyFile << "property double x" << endl;
	outputPlyFile << "property double y" << endl;
	outputPlyFile << "property double z" << endl;
	if (normal_list.size() > 0)
	{
		outputPlyFile << "property double nx" << endl;
		outputPlyFile << "property double ny" << endl;
		outputPlyFile << "property double nz" << endl;
	}
	outputPlyFile << "end_header" << endl;
	if (normal_list.size() > 0)
	{
		// write the vertex position and normal
		for (size_t i = 0; i < vertex_list.size(); i++)
		{
			CPoint vert = vertex_list[i];
			CPoint norm = normal_list[i];
			outputPlyFile << vert[0] << " " << vert[1] << " " << vert[2] << " " << norm[0] << " " << norm[1] << " " << norm[2] << endl;
		}
	}
	else
	{
		for (size_t i = 0; i < vertex_list.size(); i++)
		{
			CPoint vert = vertex_list[i];
			outputPlyFile << vert[0] << " " << vert[1] << " " << vert[2] << endl;
		}
	}
	outputPlyFile.close();
	return 1;
}

CPoint PlyCloud::get_vertex(size_t v_id)
{
	assert(v_id < vertex_list.size());
	return vertex_list[v_id];
}

CPoint PlyCloud::get_normal(size_t v_id)
{
	assert(v_id < vertex_list.size());
	return normal_list[v_id];
}

std::vector<CPoint> PlyCloud::get_vertex_list()
{
	return vertex_list;
}

std::vector<CPoint> PlyCloud::get_normal_list()
{
	return normal_list;
}
