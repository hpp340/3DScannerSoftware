// updated: 7/23/2014
// author: Jerome Jiang
// updated: 8/5/2014
// update information: make it compatible with JVertex class. 
#include "header\PlyCloud.h"

PlyCloud::PlyCloud()
{
	existFace = existNormal = existValue = existVertexPos = existTexture =  false;
	vertex_num = 0;
	face_num = 0;
	vertProperty.assign(7, VertexInfo::NONE);
	deleted_vertex_list.assign(vertex_num, false);
	newVertexIdList.assign(vertex_num, 0);
	deleted_face_list.assign(face_num, false);
}

// overload constructor
PlyCloud::PlyCloud(std::vector<CPoint> newVertexList, std::vector<CPoint> newNormalList)
{
	existFace = existValue = existTexture = false;
	existVertexPos = existNormal = true;
	vertProperty.assign(7, VertexInfo::NONE);
	for (size_t i = 0; i < newVertexList.size(); i++)
	{
		JVertex * jVert = new JVertex(newVertexList[i], newNormalList[i]);
		addJVert(jVert);
		newVertexIdList.push_back((int)i);
	}

	vertex_num = (int)newVertexList.size();
	deleted_vertex_list.assign(vertex_num, false);
	cleanMesh();
	normalizeMesh();
}

// overload constructor
PlyCloud::PlyCloud(std::vector<CPoint> newVertexList)
{
	existFace = existValue = existNormal = existTexture = false;
	existVertexPos = true;
	for (size_t i = 0; i < newVertexList.size(); i++)
	{
		JVertex * jVert = new JVertex(newVertexList[i]);
		addJVert(jVert);
		newVertexIdList.push_back((int)i);
	}
	vertProperty.assign(7, VertexInfo::NONE);
	vertProperty[0] = VertexInfo::POSX;
	vertProperty[1] = VertexInfo::POSY;
	vertProperty[2] = VertexInfo::POSZ;
	vertex_num = (int)newVertexList.size();
	normalizeMesh();
	deleted_vertex_list.assign(vertex_num, false);
}

// overload constructor
PlyCloud::PlyCloud(std::vector<CPoint> newVertexList, std::vector<JFace *> newFaceList)
{
	existNormal = existValue = existTexture = false;
	existVertexPos = existFace = true;
	vertProperty.assign(7, VertexInfo::NONE);
	for (size_t i = 0; i < newVertexList.size(); i++)
	{
		JVertex * jVert = new JVertex(newVertexList[i]);
		addJVert(jVert);
		newVertexIdList.push_back((int)i);
	}
	face_list = newFaceList;
	for (size_t i = 0; i < face_list.size(); i++)
	{
		JFace * face = face_list[i];
		JVertexList[face->vert1Id]->addFace(face);
		JVertexList[face->vert2Id]->addFace(face);
		JVertexList[face->vert3Id]->addFace(face);
	}
	vertex_num = (int)newVertexList.size();
	deleted_vertex_list.assign(vertex_num, false);
	face_num = (int)face_list.size();
	deleted_face_list.assign(face_num, false);
	computeFaceNormal();
	computeVertexNormal();
	normalizeMesh();
}

PlyCloud::~PlyCloud()
{
}

bool PlyCloud::read_ply(const char * filename)
{
	//if (vertex_list.size() > 0) // already read have the point cloud
	//{
	//	return false;
	//}
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
		if ((inS.length() > 12) && (inS.substr(0, 12) == "element face"))
		{
			string faceNumStr = inS.substr(13, inS.length() - 13);
			face_num = atoi(faceNumStr.c_str());
			existFace = true;
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
		existVertexPos = true;
	}

	if ((std::find(vertProperty.begin(), vertProperty.end(), VertexInfo::NORMALX) != vertProperty.end())
		&& (std::find(vertProperty.begin(), vertProperty.end(), VertexInfo::NORMALY) != vertProperty.end())
		&& (std::find(vertProperty.begin(), vertProperty.end(), VertexInfo::NORMALZ) != vertProperty.end()))
	{
		existNormal = true;
	}

	if (std::find(vertProperty.begin(), vertProperty.end(), VertexInfo::VALUE) != vertProperty.end())
	{
		existValue = true;
	}

	// read in the vertex position and normal
	int vertIter = 0;
	while ((!plyfile.eof()) && (vertIter < vertex_num))
	{
		getline(plyfile, inS);
		vertIter++;
		CPoint vertexPosition, normalPosition;
		double vertexValue;
		JVertex * jVert = new JVertex();
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
		if (existVertexPos)
		{
			jVert->addPos(vertexPosition);
			addJVert(jVert);
		}
		if (existNormal)
		{
			add_norm(normalPosition);
		}
		if (existValue)
		{
			add_value(vertexValue);
		}
	}

	std::cout << "vertex list size" << JVertexList.size() << std::endl;

	//normalize mesh
	normalizeMesh();
	deleted_vertex_list.assign(vertex_num, false);

	// add normal to JVertexList
	for (size_t i = 0; i < JVertexList.size(); i++)
	{
		JVertex * jVert = JVertexList[i];
		if (existNormal)
		{
			jVert->addNormal(normal_list[i]);
		}
		newVertexIdList.push_back((int)i);
	}

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

		JFace * face = new JFace(data1, data2, data3);
		JVertex * jVert1 = JVertexList[data1];
		JVertex * jVert2 = JVertexList[data2];
		JVertex * jVert3 = JVertexList[data3];
		jVert1->addFace(face); jVert2->addFace(face); jVert3->addFace(face);
		add_face(face);
		face->setFaceId((int)face_list.size() - 1);
	}
	face_num = (int)face_list.size() - 1;
	deleted_face_list.assign(face_num+1, false);
	std::cout << "face list size" << face_list.size() << std::endl;
	if (face_num > 0)
	{
		std::cout << "computing face normal" << std::endl;
		computeFaceNormal();
	}
	if ((!existNormal) && (existFace))
	{
		std::cout << "computing vertex normal" << std::endl;
		computeVertexNormal();
	}
	if (existNormal)
	{
		cleanMesh();
	}
	plyfile.close();
	return true;
}

bool PlyCloud::read_obj(const char * filename)
{
	std::cout << "PlyCloud: read_obj..." << std::endl;
	std::ifstream objfile;
	objfile.open(filename);
	if (objfile.fail())
	{
		std::cout << "Error:Can't open obj file." << std::endl;
		return false;
	}
	string inS;
	getline(objfile, inS);

	// reading not finished
	while (! objfile.eof())
	{
		if ((inS[0] == 'v') && (inS[1] == ' ')) // this is a vertex
		{
			JVertex * vert = new JVertex();
			CPoint vertPos;
			size_t firstSpace, secondSpace, thirdSpace;
			firstSpace = secondSpace = thirdSpace = 0;
			firstSpace = inS.find(" ", 0);
			secondSpace = inS.find(" ", firstSpace + 2);
			thirdSpace = inS.find(" ", secondSpace + 1);
			string firstPos = inS.substr(firstSpace + 1, secondSpace - firstSpace);
			string secondPos = inS.substr(secondSpace + 1, thirdSpace - secondSpace);
			string thirdPos = inS.substr(thirdSpace + 1, inS.length() - thirdSpace);
			double xPos = atof(firstPos.c_str());
			double yPos = atof(secondPos.c_str());
			double zPos = atof(thirdPos.c_str());

			vertPos[0] = xPos; vertPos[1] = yPos; vertPos[2] = zPos;
			//std::cout << xPos << " " << yPos << " " << zPos << std::endl;

			vert->addPos(vertPos);
			//add_vert(vertPos);
			addJVert(vert);
		}
		else if ((inS[0] == 'v') && (inS[1] == 'n')) // this is a vertex normal
		{
			existNormal = true;
			CPoint norlPos;
			size_t firstSpace, secondSpace, thirdSpace;
			firstSpace = secondSpace = thirdSpace = 0;
			firstSpace = inS.find(" ", 0);
			secondSpace = inS.find(" ", firstSpace + 1);
			thirdSpace = inS.find(" ", secondSpace + 1);
			string firstPos = inS.substr(firstSpace + 1, secondSpace - firstSpace);
			string secondPos = inS.substr(secondSpace + 1, thirdSpace - secondSpace);
			string thirdPos = inS.substr(thirdSpace + 1, inS.length() - thirdSpace);
			double xPos = atof(firstPos.c_str());
			double yPos = atof(secondPos.c_str());
			double zPos = atof(thirdPos.c_str());
			norlPos[0] = xPos; norlPos[1] = yPos; norlPos[2] = zPos;
			add_norm(norlPos);
		}
		else if ((inS[0] == 'v') && (inS[1] == 't')) // this is a vertex texture coordinate
		{
			existTexture = true;
			CPoint2 texturePos;
			size_t firstSpace, secondSpace;
			firstSpace = secondSpace = 0;
			firstSpace = inS.find(" ", 0);
			secondSpace = inS.find(" ", firstSpace + 1);
			string firstPos = inS.substr(firstSpace + 1, secondSpace - firstSpace);
			string secondPos = inS.substr(secondSpace + 1, inS.length() - secondSpace);
			double xPos = atof(firstPos.c_str());
			double yPos = atof(secondPos.c_str());

			texturePos[0] = xPos; texturePos[1] = yPos;
			texture_list.push_back(texturePos);
		}
		else if (inS[0] == 'f') // this is a face
		{
			existFace = true;
			size_t firstSpace, secondSpace, thirdSpace;
			firstSpace = secondSpace = thirdSpace = 0;
			firstSpace = inS.find(" ", 0);
			secondSpace = inS.find(" ", firstSpace + 1);
			thirdSpace = inS.find(" ", secondSpace + 1);
			string firstPos = inS.substr(firstSpace + 1, secondSpace - firstSpace);
			string secondPos = inS.substr(secondSpace + 1, thirdSpace - secondSpace);
			string thirdPos = inS.substr(thirdSpace + 1, inS.length() - thirdSpace);
			size_t firstSlash1, secondSlash1, firstSlash2, secondSlash2, firstSlash3, secondSlash3;
			firstSlash1 = secondSlash1 = firstSlash2 = secondSlash2 = firstSlash3 = secondSlash3 = 0;
			firstSlash1 = firstPos.find("/", 0);
			secondSlash1 = firstPos.find("/", firstSlash1 + 1);
			firstSlash2 = secondPos.find("/", 0);
			secondSlash2 = secondPos.find("/", firstSlash2 + 1);
			firstSlash3 = thirdPos.find("/", 0);
			secondSlash3 = thirdPos.find("/", firstSlash3 + 1);
			string vert1 = firstPos.substr(0, firstSlash1);
			string vert2 = secondPos.substr(0, firstSlash2);
			string vert3 = thirdPos.substr(0, firstSlash3);
			string texture1 = firstPos.substr(firstSlash1 + 1, secondSlash1 - firstSlash1);
			string texture2 = secondPos.substr(firstSlash2 + 1, secondSlash2 - firstSlash2);
			string texture3 = thirdPos.substr(firstSlash3 + 1, secondSlash3 - firstSlash3);
			int vert1Id = atoi(vert1.c_str()) - 1;
			int vert2Id = atoi(vert2.c_str()) - 1;
			int vert3Id = atoi(vert3.c_str()) - 1;
			JVertex * Jvert1 = JVertexList[vert1Id];
			JVertex * Jvert2 = JVertexList[vert2Id];
			JVertex * Jvert3 = JVertexList[vert3Id];
			if ((texture1 != "") && (texture2 != "") && (texture3 != ""))
			{
				int text1Id = atoi(texture1.c_str());
				int text2Id = atoi(texture2.c_str());
				int text3Id = atoi(texture3.c_str());
				JFace  * currFace = new JFace(vert1Id, vert2Id, vert3Id, text1Id, text2Id, text3Id);
				Jvert1->addFace(currFace); Jvert2->addFace(currFace); Jvert3->addFace(currFace);
				add_face(currFace);
				currFace->setFaceId((int)face_list.size() - 1);
			}
			else
			{
				JFace * currFace = new JFace(vert1Id, vert2Id, vert3Id);
				Jvert1->addFace(currFace); Jvert2->addFace(currFace); Jvert3->addFace(currFace);
				add_face(currFace);
				currFace->setFaceId((int)face_list.size() - 1);
			}
		}

		getline(objfile, inS);
	}

	vertex_num = (int)JVertexList.size();
	face_num = (int)face_list.size();
	// normalize mesh
	normalizeMesh();

	deleted_vertex_list.assign(vertex_num, false);
	deleted_face_list.assign(face_num, false);

	if (face_num > 0)
	{
		std::cout << "PlyCloud:computeFaceNormal" << std::endl;
		computeFaceNormal();
		if (! existNormal)
		{
			std::cout << "PlyCloud:computeVertexNormal" << std::endl;
			computeVertexNormal();
		}
		
	}

	for (size_t i = 0; i < JVertexList.size(); i++)
	{
		JVertex * jVert = JVertexList[i];
		if (existTexture)
		{
			CPoint2 text = texture_list[i];
			jVert->addTexture(text);
		}
		newVertexIdList.push_back((int)i);
	}

	std::cout << "vertex size " << JVertexList.size() << " face size " << face_list.size() << std::endl;
 	objfile.close();
	return true;
}

void PlyCloud::computeFaceNormal()
{
	assert(face_num > 0);
	for (size_t i = 0; i < face_list.size(); i++)
	{
		JFace *faceIter = face_list[i];
		CPoint v1 = JVertexList[faceIter->vert1Id]->getPoint();
		CPoint v2 = JVertexList[faceIter->vert2Id]->getPoint();
		CPoint v3 = JVertexList[faceIter->vert3Id]->getPoint();
		CPoint edgeVector1 = v2 - v1;
		CPoint edgeVector2 = v3 - v1;
		CPoint faceNormal = crossProduct(edgeVector1, edgeVector2);
		faceNormal = faceNormal / faceNormal.norm();
		faceIter->setFaceNormal(faceNormal);
	}
}

void PlyCloud::computeVertexNormal()
{
	for (size_t i = 0; i < JVertexList.size(); i++)
	{
		JVertex *vert = JVertexList[i];
		CPoint vertNormal(0.0, 0.0, 0.0);
		std::vector<JFace *> faces = vert->getFacesList();
		for (size_t f = 0; f < faces.size(); f++)
		{
			JFace * face = faces[f];
			CPoint faceNormal = face->getFaceNormal();
			vertNormal += faceNormal;
		}
		vertNormal /= (int)faces.size();

		vert->addNormal(vertNormal);
	}
	existNormal = true;
}

CPoint PlyCloud::crossProduct(CPoint p1, CPoint p2)
{
	CPoint result;
	result[0] = p1[1] * p2[2] - p2[1] * p1[2];
	result[1] = p1[2] * p2[0] - p1[0] * p2[2];
	result[2] = p1[0] * p2[1] - p1[1] * p2[0];

	return result;
}

//void PlyCloud::add_vert(CPoint v)
//{
//	vertex_list.push_back(v);
//}

void PlyCloud::addJVert(JVertex * Jvert)
{
	JVertexList.push_back(Jvert);
}

void PlyCloud::add_norm(CPoint n)
{
	normal_list.push_back(n);
}

void PlyCloud::add_value(double value)
{
	value_list.push_back(value);
}

void PlyCloud::add_face(JFace * f)
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
	if (JVertexList.size() == 0)
	{
		std::cout << "Error: No Vertex to save." << std::endl;
		return false;
	}
	// write header of ply file
	outputPlyFile << "ply" << endl;
	outputPlyFile << "format ascii 1.0" << endl;
	outputPlyFile << "element vertex " << vertex_num << endl;
	outputPlyFile << "property double x" << endl;
	outputPlyFile << "property double y" << endl;
	outputPlyFile << "property double z" << endl;
	if (existNormal)
	{
		outputPlyFile << "property double nx" << endl;
		outputPlyFile << "property double ny" << endl;
		outputPlyFile << "property double nz" << endl;
	}
	if (existFace)
	{
		outputPlyFile << "element face " << face_num << endl;
		outputPlyFile << "property list uchar int vertex_indices" << endl;
	}
	outputPlyFile << "end_header" << endl;

	int delected_num = 0;
	for (size_t i = 0; i < deleted_vertex_list.size(); i++)
	{
		if (deleted_vertex_list[i])
		{
			delected_num++;
		}
	}

	if (existNormal)
	{
		// write the vertex position and normal
		for (size_t i = 0; i < JVertexList.size(); i++)
		{
			if (! deleted_vertex_list[i])
			{
				CPoint vert = JVertexList[i]->getPoint();
				CPoint norm = JVertexList[i]->getNormal();
				outputPlyFile << vert[0] << " " << vert[1] << " " << vert[2] << " " << norm[0] << " " << norm[1] << " " << norm[2] << endl;
			}
		}
	}
	else
	{
		for (size_t i = 0; i < JVertexList.size(); i++)
		{
			if (! deleted_vertex_list[i])
			{
				CPoint vert = JVertexList[i]->getPoint();
				outputPlyFile << vert[0] << " " << vert[1] << " " << vert[2] << endl;
			}
		}
	}
	if (existFace)
	{
		for (size_t i = 0; i < face_list.size(); i++)
		{
			JFace * currFace = face_list[i];
			int newVert1Id = newVertexIdList[currFace->vert1Id];
			int newVert2Id = newVertexIdList[currFace->vert2Id];
			int newVert3Id = newVertexIdList[currFace->vert3Id];
			if (! deleted_face_list[currFace->faceId])
			{
				outputPlyFile << "3 " << newVert1Id << " " << newVert2Id << " " << newVert3Id << endl;
			}
		}
	}
	outputPlyFile.close();
	return true;
}

bool PlyCloud::write_obj(const char *filename)
{

	std::ofstream outputObjFile;
	outputObjFile.open(filename);
	if (outputObjFile.fail())
	{
		std::cout << "Error : Can't new output file." << std::endl;
		return false;
	}
	if (JVertexList.size() == 0)
	{
		std::cout << "Error: No Vertex to save." << std::endl;
		return false;
	}

	// output comments: num of vertices and num of faces
	outputObjFile << "# " << JVertexList.size() << " vertices " << face_list.size() << " faces" << endl;
	for (size_t i = 0; i < JVertexList.size(); i++)
	{
		JVertex * currVert = JVertexList[i];
		CPoint currPoint = currVert->getPoint();
		if (! deleted_vertex_list[i])
		{
			outputObjFile << "v " << currPoint[0] << " " << currPoint[1] << " " << currPoint[2] << endl;
		}
	}
	if (existNormal)
	{
		for (size_t i = 0; i < JVertexList.size(); i++)
		{
			JVertex * currVert = JVertexList[i];
			CPoint currNorl = currVert->getNormal();
			if (!deleted_vertex_list[i])
			{
				outputObjFile << "vn " << currNorl[0] << " " << currNorl[1] << " " << currNorl[2] << endl;
			}
		}
	}
	if (existTexture)
	{
		for (size_t i = 0; i < JVertexList.size(); i++)
		{
			JVertex * currVert = JVertexList[i];
			CPoint2 currText = currVert->getTexture();
			if (!deleted_vertex_list[i])
			{
				outputObjFile << "vt " << currText[0] << " " << currText[1] << endl;
			}
		}
	}
	
	if (existFace)
	{
		for (size_t i = 0; i < face_list.size(); i++)
		{
			JFace * currFace = face_list[i];
			int newVert1Id = newVertexIdList[currFace->vert1Id];
			int newVert2Id = newVertexIdList[currFace->vert2Id];
			int newVert3Id = newVertexIdList[currFace->vert3Id];
			if (! deleted_face_list[currFace->faceId])
			{
				if ((!existNormal) && (!existTexture))
				{
					outputObjFile << "f " << newVert1Id + 1 << " " << newVert2Id + 1 << " " << newVert3Id + 1 << endl;
				}
				else if ((existNormal) && (!existTexture))
				{
					outputObjFile << "f " << newVert1Id + 1 << "//" << newVert1Id + 1 << " "
						<< newVert2Id + 1 << "//" << newVert2Id + 1 << " "
						<< newVert3Id + 1 << "//" << newVert3Id + 1 << endl;
				}
				else if ((!existNormal) && (existTexture))
				{
					outputObjFile << "f " << newVert1Id + 1 << "/" << newVert1Id + 1 << " "
						<< newVert2Id + 1 << "/" << newVert2Id + 1 << " "
						<< newVert3Id + 1 << "/" << newVert3Id + 1 << endl;
				}
				else if ((existNormal) && (existTexture))
				{
					outputObjFile << "f " << newVert1Id + 1 << "/" << newVert1Id + 1 << "/" << newVert1Id + 1 << " "
						<< newVert2Id + 1 << "/" << newVert2Id + 1 << "/" << newVert2Id + 1 << " "
						<< newVert3Id + 1 << "/" << newVert3Id + 1 << "/" << newVert3Id + 1 << endl;
				}
			}
		}
	}
	outputObjFile.close();
	return true;
}

JVertex * PlyCloud::get_vertex(size_t v_id)
{
	assert(v_id < JVertexList.size());
	return JVertexList[v_id];
}

std::vector<JFace *> PlyCloud::get_face_list()
{
	return face_list;
}

std::vector<bool> PlyCloud::get_deleted_vertex_list()
{
	return deleted_vertex_list;
}

std::vector<bool> PlyCloud::get_deleted_face_list()
{
	return deleted_face_list;
}

int PlyCloud::get_vertex_num()
{
	return vertex_num;
}

int PlyCloud::get_face_num()
{
	return face_num;
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

void PlyCloud::normalizeMesh()
{
	std::cout << "PlyCloud:normalizeMesh" << std::endl;
	CPoint center(0.0, 0.0, 0.0);
	for (size_t i = 0; i < JVertexList.size(); i++)
	{
		CPoint vertIter = JVertexList[i]->getPoint();
		center += vertIter;
	}
	center /= (double)JVertexList.size();
	for (size_t i = 0; i < JVertexList.size(); i++)
	{
		JVertex * jVert = JVertexList[i];
		CPoint newVertPos = jVert->getPoint() - center;
		jVert->addPos(newVertPos);
	}

	double scale = 0.0;
	for (size_t i = 0; i < JVertexList.size(); i++)
	{
		CPoint vertIter = JVertexList[i]->getPoint();
		for (size_t j = 0; j < 3; j++)
		{
			scale = (scale > fabs(vertIter[(int)j])) ? scale : fabs(vertIter[(int)j]);
		}
	}
	for (size_t i = 0; i < JVertexList.size(); i++)
	{
		JVertex * jVert = JVertexList[i];
		CPoint newVertPos = jVert->getPoint() / scale;
		jVert->addPos(newVertPos);
	}
}

void PlyCloud::add_deleted_vertex(int v)
{
	if (! deleted_vertex_list[v])
	{
		deleted_vertex_list[v] = true;
		newVertexIdList[v] = -1;
		for (size_t i = v+1; i < newVertexIdList.size(); i++)
		{
			newVertexIdList[i] -= 1;
		}
		vertex_num--;
	}
}

void PlyCloud::add_deleted_face(int f)
{
	if (! deleted_face_list[f])
	{
		deleted_face_list[f] = true;
		face_num--;
	}
}

void PlyCloud::cleanMesh()
{
	std::cout << "PlyCloud:cleanMesh..." << std::endl;
	for (size_t i = 0; i < JVertexList.size(); i++)
	{
		CPoint currNormal = JVertexList[i]->getNormal();
		if (currNormal.norm() == 0.0)
		{
			deleted_vertex_list[i] = true;
		}
	}
}
