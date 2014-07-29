// updated: 7/23/2014
#include "header\PlyCloud.h"

PlyCloud::PlyCloud()
{
	hasFace = hasNormal = hasValue = hasVertexPos = false;
	vertex_num = 0;
	face_num = 0;
	vertProperty.assign(7, VertexInfo::NONE);
}

// overload constructor
PlyCloud::PlyCloud(std::vector<CPoint> newVertexList, std::vector<CPoint> newNormalList)
{
	hasFace = hasValue = false;
	hasVertexPos = hasNormal = true;
	vertProperty.assign(7, VertexInfo::NONE);
	vertex_list = newVertexList;
	normal_list = newNormalList;
	vertex_num = (int)vertex_list.size();
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

		std::cout << "reading..." << std::endl;
		if ((inS.length() > 14) && (inS.substr(0, 14) == "element vertex"))
		{
			std::cout << "element vertex" << std::endl;
			string vertexNumStr = inS.substr(15, inS.length() - 15);
			std::cout << "vertex num string " << vertexNumStr << std::endl;
			vertex_num = atoi(vertexNumStr.c_str());
			std::cout << "vertex number" << vertex_num << std::endl;
			getline(plyfile, inS);
			int propIter = 0;
			while ((inS != "end_header") && (inS.substr(0, 7) != "element"))
			{
				setupVertexProperty(inS, propIter);
				getline(plyfile, inS);
				propIter++;
			}
		}
		else if ((inS.length() > 12) && (inS.substr(0, 12) == "element face"))
		{
			string faceNumStr = inS.substr(13, inS.length() - 13);
			face_num = atoi(faceNumStr.c_str());
			hasFace = true;
		}
		if (inS != "end_header")
		{
			getline(plyfile, inS);
		}
	}

	if ((std::find(vertProperty.begin(), vertProperty.end(), VertexInfo::POSX) != vertProperty.end())
		&& (std::find(vertProperty.begin(), vertProperty.end(), VertexInfo::POSY) != vertProperty.end())
		&& (std::find(vertProperty.begin(), vertProperty.end(), VertexInfo::POSZ) != vertProperty.end()))
	{
		hasVertexPos = true;
	}

	if ((std::find(vertProperty.begin(), vertProperty.end(), VertexInfo::NORMALX) != vertProperty.end())
		&& (std::find(vertProperty.begin(), vertProperty.end(), VertexInfo::NORMALY) != vertProperty.end())
		&& (std::find(vertProperty.begin(), vertProperty.end(), VertexInfo::NORMALZ) != vertProperty.end()))
	{
		hasNormal = true;
	}

	if (std::find(vertProperty.begin(), vertProperty.end(), VertexInfo::VALUE) != vertProperty.end())
	{
		hasValue = true;
	}

	// read in the vertex position and normal
	int vertIter = 0;
	while ((!plyfile.eof()) && (vertIter < vertex_num))
	{
		getline(plyfile, inS);
		vertIter++;
		CPoint vertexPosition, normalPosition;
		double vertexValue;

		size_t firstSpace, secondSpace, thirdSpace, fourthSpace, fifthSpace, sixthSpace;
		firstSpace = secondSpace = thirdSpace = fourthSpace = fifthSpace = 0;
		firstSpace = inS.find(" ", 0);
		secondSpace = inS.find(" ", firstSpace + 1);
		thirdSpace = inS.find(" ", secondSpace + 1);
		fourthSpace = inS.find(" ", thirdSpace + 1);
		fifthSpace = inS.find(" ", fourthSpace + 1);
		sixthSpace = inS.find(" ", fifthSpace + 1);
		string firstPosition = inS.substr(0, firstSpace);
		string secondPosition = inS.substr(firstSpace + 1, secondSpace - firstSpace);
		string thirdPosition = inS.substr(secondSpace + 1, thirdSpace - secondSpace);
		string normalFirstPosition = inS.substr(thirdSpace + 1, fourthSpace - thirdSpace);
		string normalSecondPosition = inS.substr(fourthSpace + 1, fifthSpace - fourthSpace);
		string normalThirdPosition = inS.substr(fifthSpace + 1, sixthSpace - fifthSpace);
		string valuePosition = inS.substr(sixthSpace + 1, inS.length() - sixthSpace);
		double data1 = atof(firstPosition.c_str());
		double data2 = atof(secondPosition.c_str());
		double data3 = atof(thirdPosition.c_str());
		double data4 = atof(normalFirstPosition.c_str());
		double data5 = atof(normalSecondPosition.c_str());
		double data6 = atof(normalThirdPosition.c_str());
		double data7 = atof(valuePosition.c_str());

		std::vector<double> dataVector;
		dataVector.push_back(data1); dataVector.push_back(data2); dataVector.push_back(data3); dataVector.push_back(data4);
		dataVector.push_back(data5); dataVector.push_back(data6); dataVector.push_back(data7);

		for (size_t i = 0; i < vertProperty.size(); i++)
		{
			if (vertProperty[i] != VertexInfo::NONE)
			{
				switch (vertProperty[i])
				{
				case VertexInfo::POSX:
					vertexPosition[0] = dataVector[i];
					break;
				case VertexInfo::POSY:
					vertexPosition[1] = dataVector[i];
					break;
				case VertexInfo::POSZ:
					vertexPosition[2] = dataVector[i];
					break;
				case VertexInfo::NORMALX:
					normalPosition[0] = dataVector[i];
					break;
				case VertexInfo::NORMALY:
					normalPosition[1] = dataVector[i];
					break;
				case VertexInfo::NORMALZ:
					normalPosition[2] = dataVector[i];
					break;
				case VertexInfo::VALUE:
					vertexValue = dataVector[i];
					break;
				default:
					break;
				}
			}
		}
		if (hasVertexPos)
		{
			add_vert(vertexPosition);
		}
		if (hasNormal)
		{
			add_norm(normalPosition);
		}
		if (hasValue)
		{
			add_value(vertexValue);
		}
	}

	std::cout << "vertex list size" << vertex_list.size() << std::endl;
	// read face information

	while (!plyfile.eof())
	{
		getline(plyfile, inS);

		size_t firstSpace, secondSpace, thirdSpace;
		firstSpace = secondSpace = thirdSpace = 0;
		firstSpace = inS.find(" ", 0);
		secondSpace = inS.find(" ", firstSpace + 1);
		thirdSpace = inS.find(" ", secondSpace + 1);
		string numVert = inS.substr(0, firstSpace);
		string firstVert = inS.substr(firstSpace + 1, secondSpace - firstSpace);
		string secondVert = inS.substr(secondSpace + 1, thirdSpace - secondSpace);
		string thirdVert = inS.substr(thirdSpace + 1, inS.length() - thirdSpace);
		int data1 = atoi(firstVert.c_str());
		int data2 = atoi(secondVert.c_str());
		int data3 = atoi(thirdVert.c_str());

		JFace face(data1, data2, data3);
		add_face(face);
	}

	std::cout << "face list size" << face_list.size() << std::endl;

	plyfile.close();
	return true;
}

void PlyCloud::add_vert(CPoint v)
{
	vertex_list.push_back(v);
}

void PlyCloud::add_norm(CPoint n)
{
	normal_list.push_back(n);
}

void PlyCloud::add_value(double value)
{
	value_list.push_back(value);
}

void PlyCloud::add_face(JFace f)
{
	face_list.push_back(f);
}

bool PlyCloud::write_ply(const char *filename)
{
	std::ofstream outputPlyFile;
	outputPlyFile.open(filename);
	if (outputPlyFile.fail())
	{
		std::cout << "Error : Can't new output file." << std::endl;
		return false;
	}
	if (vertex_list.size() == 0)
	{
		std::cout << "Error: No Vertex to save." << std::endl;
		return false;
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
	return true;
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

std::vector<JFace> PlyCloud::get_face_list()
{
	return face_list;
}

int PlyCloud::get_vertex_num()
{
	return vertex_num;
}

void PlyCloud::setupVertexProperty(string inS, int propIter)
{
	if (inS.substr(inS.length() - 2, 2) == " x")
	{
		vertProperty[propIter] = VertexInfo::POSX;
	}
	if (inS.substr(inS.length() - 2, 2) == " y")
	{
		vertProperty[propIter] = VertexInfo::POSY;
	}
	if (inS.substr(inS.length() - 2, 2) == " z")
	{
		vertProperty[propIter] = VertexInfo::POSZ;
	}

	if (inS.substr(inS.length() - 2, 2) == "nx")
	{
		vertProperty[propIter] = VertexInfo::NORMALX;
	}
	if (inS.substr(inS.length() - 2, 2) == "ny")
	{
		vertProperty[propIter] = VertexInfo::NORMALY;
	}
	if (inS.substr(inS.length() - 2, 2) == "nz")
	{
		vertProperty[propIter] = VertexInfo::NORMALZ;
	}
	if (inS.substr(inS.length() - 5, 5) == "value")
	{
		vertProperty[propIter] = VertexInfo::VALUE;
	}
}
