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

#include"stdafx.h"
#include<iostream>
#include "OpenMesh\Core\IO\MeshIO.hh"
#include "OpenMesh\Core\Mesh\Types\TriMesh_ArrayKernelT.hh"

#include "HoleFiller.hh"

//=============================================================================

typedef OpenMesh::TriMesh_ArrayKernelT<OpenMesh::DefaultTraits> MyMesh;

//=============================================================================


int main( int argc, char ** argv )
{
  if ( argc != 4 )
  {
    std::cerr << "\n"
	      << "Usage :\n"
	      << "\n"
	      << "  holefill <stages> <infile> <outfile>\n"
	      << "\n"
	      << "Read a mesh <infile>, fill all holes and write the result\n"
	      << "to <outfile>.\n"
	      << "Stages are:\n"
	      << "  1 = Produce the minimal triangulation of the holes only.\n"
	      << "  2 = Stage 1 + Remesh the fillings\n"
	      << "  3 = Stage 2 + Smooth the fillings\n\n";
	getchar();
    exit( EXIT_FAILURE );
  }

  int stages = atoi( argv[1] );

  if ( stages < 1 || stages > 3 )
  {
    std::cerr << "Error: The stages parameter may only range from 1 to 3.\n";
	getchar();
    exit( EXIT_FAILURE );
  }

  MyMesh mesh;


  // read a mesh

  if ( ! OpenMesh::IO::read_mesh( mesh, argv[2] ) )
  {
    std::cerr << "Error: Could not read mesh\n";
	getchar();
    exit( EXIT_FAILURE );
  }

  // fill the holes

  HoleFiller< MyMesh > hf( mesh );
  hf.fill_all_holes( stages );


  // write result

  std::cerr << "Saving result ... ";

  if ( ! OpenMesh::IO::write_mesh( mesh, argv[3] ) )
  {
    std::cerr << "Error: Could not write mesh\n";
	getchar();
    exit( EXIT_FAILURE );
  }

  std::cerr << "ok\n";
  getchar();

  exit( EXIT_SUCCESS );
}

//=============================================================================
