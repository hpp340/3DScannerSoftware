#pragma once

#include <vector>
#include "JFace.h"
#include "header\Point.h"
#include "header\Point2.h"

using namespace MeshLib;

/*!
* \brief Namespace for self implemented mesh data structure.
* \details Only include JVertex JColor and JFace.
* \details Better for finding adajcent faces of a vertex. But no halfedge data structure. Maybe in the future.
*/
namespace JMesh
{
	/*!
	* \brief Struct for JColor.
	* \details Three components of color
	* - red, green and blue
	*/
	struct JColor{

		/// Constructor. All three components are set to zero at start
		JColor() { red = 0; green = 0; blue = 0; };
		~JColor() {};
		int red;
		int green;
		int blue;
	};

	/*!
	* \brief the Class for vertex in triangle mesh.
	* \details Use this class so that it would be easier to find the corresponded faces of one vertex.
	*/
	class JVertex
	{
	public:

		/// Constructor
		JVertex();

		/// Constructor: with the vertex position
		/// \param pos x y z coordinates of the vertex
		JVertex(CPoint pos);

		/// Constructor: with the vertex position and its normal
		/// \param pos x y z coordinates of the vertex
		/// \param norm normal vector
		JVertex(CPoint pos, CPoint norm);

		/// add adjacent face to current vertex
		/// \param face an adjacent face of the vertex
		void addFace(JFace * face);

		/// add position to current vertex 
		void addPos(CPoint pos);

		/// add normal to current vertex
		void addNormal(CPoint norm);

		/// add texture to current vertex
		void addTexture(CPoint2 textPos);

		/// add color to current vertex with RGB integers
		/// \param red,green,blue: RGB of the vertex
		void addColor(int red, int green, int blue);

		/// add color to current vertex with JColor struct
		void addColor(JColor _color);

		~JVertex();

		/// Return the adjacent faces list of current vertex
		std::vector<JFace*> getFacesList() { return facesList; };

		/// Get the position of current vertex
		/// \return the position of the vertex
		CPoint getPoint() { return position; };

		/// Get the normal of current vertex
		/// \return the normal vector of the vertex
		CPoint getNormal() { return normalPos; };

		/// Get the texture coordinates of current vertex
		/// \return the texture coordinates of the vertex
		CPoint2 getTexture() { return texturePos; };

		/// Get the color of current vertex
		/// \return color in struct JColor of current vertex
		JColor getColor() { return color; };

		bool hasNormal() { return existNormal; };
		bool hasFaces() { return existFaces; };
		bool hasTexture() { return existTexture; };
		bool hasColor() { return existColor; }

	protected:

		/// List of adjacent faces of the vertex
		std::vector<JFace*> facesList;

		/// 3D position coordinates of the vertex
		CPoint position;

		/// Normal of current vertex
		CPoint normalPos;

		/// texture coordinate of the vertex
		CPoint2 texturePos;

		/// RGB information of the vertex
		JColor color;

		/// flag indicating if the vertex has color
		bool existColor;

		/// flag indicating if the vertex has normal
		bool existNormal;

		/// flag indicating if the vertex has adjacent faces
		bool existFaces;

		/// flag indicating if the vertex has texture coordinate
		bool existTexture;
	};
}

