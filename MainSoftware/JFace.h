#pragma once

#include "header\Point.h"

using namespace MeshLib;

namespace JMesh
{

	/*!
	* \brief Class for face in the data structure.
	* \details It has the ID of three vertices, face normal and texture ID.
	* \details If the face normal is not given at start, the class can compute it.
	* \author Jerome Jiang
	*/
	class JFace
	{
	public:
		/// Constructor
		JFace();

		/// overload constructor
		/// Construct with face ID
		JFace(int);

		/// overload constructor
		/// Construct with IDs of three vertices
		JFace(int, int, int);

		/// overload constructor
		/// Construct with three vertex id and three texture id
		JFace(int, int, int, int, int, int);

		~JFace();

		/// Compute face normal
		void setFaceNormal(CPoint);

		/// Get face normal
		CPoint getFaceNormal();

		/// Set face id
		void setFaceId(int);

		/// flag indicating if face normal is set
		bool isNormalSet() { return normalSet; };

		/// flag indicating if texture id is set
		bool isTextureSet() { return normalSet; };

	private:
		bool normalSet;
		bool textureSet;
	protected:

		/// Normal of the face
		CPoint faceNormal;

	public:

		/// Face ID
		int faceId;

		/// Three vertex IDs
		int vert1Id, vert2Id, vert3Id;

		/// Three texture IDs
		int texture1Id, texture2Id, texture3Id;
	};

}

