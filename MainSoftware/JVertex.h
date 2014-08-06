#pragma once

#include <vector>
#include "JFace.h"
#include "header\Point.h"
#include "header\Point2.h"

using namespace MeshLib;

namespace JMesh
{
	class JVertex
	{
	public:
		JVertex();
		JVertex(CPoint pos);
		JVertex(CPoint pos, CPoint norm);

		// add face to vertex
		void addFace(JFace * face);
		void addPos(CPoint pos);
		void addNormal(CPoint norm);
		void addTexture(CPoint2 textPos);



		~JVertex();

		std::vector<JFace*> getFacesList() { return facesList; };
		CPoint getPoint() { return position; };
		CPoint getNormal() { return normalPos; };

		bool hasNormal() { return existNormal; };
		bool hasFaces() { return existFaces; };
		bool hasTexture() { return existTexture; };

	protected:
		std::vector<JFace*> facesList;
		CPoint position;
		CPoint normalPos;
		CPoint2 texturePos;
		bool existNormal;
		bool existFaces;
		bool existTexture;
	};
}

