#ifndef _CONV_MESH_H_
#define _CONV_MESH_H_ 

#include "stdafx.h"
#include "BaseMesh.h"
#include "Edge.h"
#include "HalfEdge.h"
#include "Face.h"
#include "iterators.h"
#include "Vertex.h"
#include "boundary.h"

using namespace MeshLib;

template<typename V, typename E, typename F, typename H>
class ConvertMesh : public CBaseMesh<CVertex, CEdge, CFace, CHalfEdge>
{
public :
	typedef V CVertex;
	typedef E CEdge;
	typedef F CFace;
	typedef H CHalfEdge;

	typedef ConvertMesh<V,E,F,H> M;

	typedef CBoundary<M> CBoundary;
	typedef CLoop<M> CLoop;

	typedef MeshVertexIterator<M> MeshVertexIterator;
	typedef MeshEdgeIterator<M> MeshEdgeIterator;
	typedef VertexVertexIterator<M> VertexVertexIterator;
	typedef VertexEdgeIterator<M> VertexEdgeIterator;
	typedef MeshFaceIterator<M> MeshFaceIterator;
	typedef FaceVertexIterator<M> FaceVertexIterator;
	typedef VertexFaceIterator<M> VertexFaceIterator;
	typedef FaceHalfedgeIterator<M> FaceHalfedgeIterator;
	typedef VertexInHalfedgeIterator<M> VertexInHalfedgeIterator;
};

typedef ConvertMesh<CVertex, CEdge, CFace, CHalfEdge> ConvMesh;

void MeshNormalize(ConvMesh *);

CPoint crossProduct(CPoint, CPoint);

#endif _CONV_MESH_H_