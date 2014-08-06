//=============================================================================
//                                                                            
//   Example code for the full-day course
//
//   M. Botsch, M. Pauly, L. Kobbelt, P. Alliez, B. Levy,
//   "Geometric Modeling Based on Polygonal Meshes"
//   held at SIGGRAPH 2007, San Diego, and Eurographics 2008, Crete.
//
//   Copyright (C) 2007 by  Computer Graphics Laboratory, ETH Zurich, 
//                      and Computer Graphics Group,      RWTH Aachen
//
//                                                                            
//-----------------------------------------------------------------------------
//                                                                            
//                                License                                     
//                                                                            
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your option) any later version.
//   
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//   
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin Street, Fifth Floor, 
//   Boston, MA  02110-1301, USA.
//                                                                            
//=============================================================================
//=============================================================================
//
// Often used abbreviations for OpenMesh entitites.
//
//=============================================================================

#ifndef OMAbbreviations_HH
#define OMAbbreviations_HH

#define import_om_abbreviations( Mesh )    \
\
  typedef Mesh::Point Point;   \
  typedef Mesh::Scalar Scalar; \
\
  typedef Mesh::VertexHandle   VH; \
  typedef Mesh::EdgeHandle     EH; \
  typedef Mesh::HalfedgeHandle HH; \
  typedef Mesh::FaceHandle     FH; \
\
  typedef Mesh::VertexIter     VI; \
  typedef Mesh::HalfedgeIter   HI; \
  typedef Mesh::EdgeIter       EI; \
  typedef Mesh::FaceIter       FI; \
\
  typedef Mesh::Vertex    Vertex;   \
  typedef Mesh::Halfedge  Halfedge; \
  typedef Mesh::Edge      Edge;     \
  typedef Mesh::Face      Face;     \
\
  typedef Mesh::ConstVertexIter    CVI; \
  typedef Mesh::ConstHalfedgeIter  CHI; \
  typedef Mesh::ConstEdgeIter      CEI; \
  typedef Mesh::ConstFaceIter      CFI; \
\
  typedef Mesh::VertexVertexIter    VVI;  \
  typedef Mesh::VertexOHalfedgeIter VOHI; \
  typedef Mesh::VertexIHalfedgeIter VIHI; \
  typedef Mesh::VertexEdgeIter      VEI;  \
  typedef Mesh::VertexFaceIter      VFI;  \
  typedef Mesh::FaceVertexIter      FVI;  \
  typedef Mesh::FaceHalfedgeIter    FHI;  \
  typedef Mesh::FaceEdgeIter        FEI;  \
  typedef Mesh::FaceFaceIter        FFI;  \
\
  typedef Mesh::ConstVertexVertexIter    CVVI;  \
  typedef Mesh::ConstVertexOHalfedgeIter CVOHI; \
  typedef Mesh::ConstVertexIHalfedgeIter CVIHI; \
  typedef Mesh::ConstVertexEdgeIter      CVEI;  \
  typedef Mesh::ConstVertexFaceIter      CVFI;  \
  typedef Mesh::ConstFaceVertexIter      CFVI;  \
  typedef Mesh::ConstFaceHalfedgeIter    CFHI;  \
  typedef Mesh::ConstFaceEdgeIter        CFEI;  \
  typedef Mesh::ConstFaceFaceIter        CFFI;  \

#endif
