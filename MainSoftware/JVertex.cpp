#include "JVertex.h"

namespace JMesh
{
	JVertex::JVertex()
	{
		existFaces = false;
		existNormal = false;
		existTexture = false;
		existColor = false;
	}

	JVertex::JVertex(CPoint pos)
	{
		addPos(pos);
		existFaces = false;
		existNormal = false;
		existTexture = false;
		existColor = false;
	}

	JVertex::JVertex(CPoint pos, CPoint norm)
	{
		addPos(pos);
		addNormal(norm);
		existFaces = existTexture = false;
		existNormal = true;
		existColor = false;
	}

	void JVertex::addFace(JFace * face)
	{
		facesList.push_back(face);
		existFaces = true;
	}

	void JVertex::addNormal(CPoint norm)
	{
		for (int i = 0; i < 3; i++)
		{
			normalPos[i] = norm[i];
		}
		existNormal = true;
	}

	void JVertex::addPos(CPoint pos)
	{
		for (int i = 0; i < 3; i++)
		{
			position[i] = pos[i];
		}
	}

	void JVertex::addTexture(CPoint2 texture)
	{
		texturePos[0] = texture[0];
		texturePos[1] = texture[1];
		existTexture = true;
	}

	void JVertex::addColor(int red, int green, int blue)
	{
		color.red = red;
		color.green = green;
		color.blue = blue;
		existColor = true;
	}

	void JVertex::addColor(JColor _color)
	{
		color.red = _color.red;
		color.green = _color.green;
		color.blue = _color.blue;
		existColor = true;
	}

	JVertex::~JVertex()
	{
	}
}
