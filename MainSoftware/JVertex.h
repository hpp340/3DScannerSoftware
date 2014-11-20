#pragma once

#include <vector>
#include "JFace.h"
#include "header\Point.h"
#include "header\Point2.h"

using namespace MeshLib;

namespace JMesh
{
	struct JColor{
		JColor() { red = 0; green = 0; blue = 0; };
		~JColor() {};
		int red;
		int green;
		int blue;
	};

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
		void addColor(int red, int green, int blue);
		void addColor(JColor _color);
		~JVertex();

		std::vector<JFace*> getFacesList() { return facesList; };
		CPoint getPoint() { return position; };
		CPoint getNormal() { return normalPos; };
		CPoint2 getTexture() { return texturePos; };
		JColor getColor() { return color; };

		bool hasNormal() { return existNormal; };
		bool hasFaces() { return existFaces; };
		bool hasTexture() { return existTexture; };
		bool hasColor() { return existColor; }

	protected:
		std::vector<JFace*> facesList;
		CPoint position;
		CPoint normalPos;
		CPoint2 texturePos;
		JColor color;
		bool existColor;
		bool existNormal;
		bool existFaces;
		bool existTexture;
	};
}

