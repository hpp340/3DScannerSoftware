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
//#include"stdafx.h"
#define HOLEFILLER_CC
#include "HoleFiller.hh"
//=============================================================================

#pragma warning (disable:4267)
template< class MeshT >
HoleFiller< MeshT >::HoleFiller( Mesh & _mesh )
  : mesh_( _mesh )
{
  mesh_.request_vertex_status();
  mesh_.request_edge_status();
  mesh_.add_property( scale_ );
}


//=============================================================================



template< class MeshT >
HoleFiller< MeshT >::~HoleFiller()
{
  mesh_.release_vertex_status();
  mesh_.release_edge_status();
  mesh_.remove_property( scale_ );
}


//=============================================================================
//
// Identify and fill all holes of the mesh.
//
//=============================================================================


template< class MeshT >
void
HoleFiller< MeshT >::fill_all_holes( int _stages )
{
  // Lock all edges and vertices
  
  for ( EI ei = mesh_.edges_begin(); ei != mesh_.edges_end(); ++ei )
    mesh_.status( ei ).set_locked( true );
  
  for ( VI vi = mesh_.vertices_begin(); vi != mesh_.vertices_end(); ++vi )
    mesh_.status( vi ).set_locked( true );
  
  // Collect all boundary edges
  
  std::vector< EH > bdry_edge;
  
  for ( EI ei = mesh_.edges_begin();
	ei != mesh_.edges_end(); ++ei )
    if ( mesh_.is_boundary( ei ) )
      bdry_edge.push_back( ei );
  
  // Fill holes
  
  int cnt = 0;
  for ( typename std::vector< EH >::iterator i = bdry_edge.begin();
	i != bdry_edge.end(); ++i )
    if ( mesh_.is_boundary( *i ) )
    {
      ++cnt;
      std::cerr << "Filling hole " << cnt << "\n";
      fill_hole( *i, _stages );
    }
  
  
  // Smooth fillings

  if ( _stages <= 2 )
    return;

  std::cerr << "Stage 3 : Smoothing the hole fillings ... ";
  
  OpenMesh::Smoother::JacobiLaplaceSmootherT< Mesh > smoother( mesh_ );
  smoother.initialize( OpenMesh::Smoother::SmootherT< Mesh >::
		       Tangential_and_Normal,
		       OpenMesh::Smoother::SmootherT< Mesh >::C1 );
  
  smoother.smooth( 500 );
  std::cerr << "ok\n";
}


//=============================================================================
//
// Fill a hole which is identified by one of its boundary edges.
//
//=============================================================================


template< class MeshT >
void
HoleFiller< MeshT >::fill_hole( EH _eh, int _stages )
{
  std::cerr << "  Stage 1 : Computing a minimal triangulation ... ";
  
  // No boundary edge, no hole
  
  if ( ! mesh_.is_boundary( _eh ) )
    return;
  
  
  // Get boundary halfedge
  
  HH hh = mesh_.halfedge_handle( _eh, 0 );
  if ( ! mesh_.is_boundary( hh ) )
    hh = mesh_.opposite_halfedge_handle( hh );
  
  
  // Collect boundary vertices
  
  boundary_vertex_.clear();
  opposite_vertex_.clear();
  
  HH ch = hh;
  do {
    boundary_vertex_.push_back( mesh_.from_vertex_handle( ch ) );
    opposite_vertex_.push_back
      ( mesh_.to_vertex_handle
	( mesh_.next_halfedge_handle
	  ( mesh_.opposite_halfedge_handle( ch ) ) ) );
    ch = mesh_.next_halfedge_handle( ch );
  } while ( ch != hh );
  
  
  // Compute a scale for each boundary vertex which is an average of
  // the lengths of its incident edges
  
  int nv = boundary_vertex_.size();

  for ( int i = 0; i < nv; ++i )
  {
    float cnt   = 0;
    float scale = 0;
    
    for ( VVI vvi = mesh_.vv_iter( boundary_vertex_[i] ); vvi; ++vvi )
    {
      cnt += 1.0f;
      Point p0 = mesh_.point( boundary_vertex_[i] );
      Point p1 = mesh_.point( vvi );
      scale += ( p1 - p0 ).norm();
    }
    
    scale /= cnt;
    
    mesh_.property( scale_, boundary_vertex_[i] ) = scale;
    
  }
  
  
  // Compute an initial triangulation
  
  w_.clear();
  w_.resize( nv, std::vector<Weight>( nv, Weight() ) );
  
  l_.clear();
  l_.resize( nv, std::vector<int>( nv, 0 ) );
  
  for ( int i = 0; i < nv - 1; ++i )
    w_[i][i+1] = Weight( 0, 0 );
  
  for ( int j = 2; j < nv; ++j )
    for ( int i = 0; i + j < nv; ++i )
    {
      
      Weight valmin;
      int   argmin = -1;
      for ( int m = i + 1; m < i + j; ++m )
      {
	Weight newval = w_[i][m] + w_[m][i+j] + weight( i, m, i+j );
	if ( newval < valmin )
	{
	  valmin = newval;
	  argmin = m;
	}
      }
      
      w_[i][i+j] = valmin;
      l_[i][i+j] = argmin;
    }

  
  // Actually fill the hole. We collect all triangles and edges of
  // this filling for further processing.
  
  hole_edge_.clear();
  hole_triangle_.clear();
  fill( 0, nv - 1 );
  
  std::cerr << "ok\n";

  if ( _stages <= 1 )
    return;
  
  // Do the patch fairing
  
  std::cerr << "  Stage 2 : Fairing the filling ... ";
  for ( int k = 0; k < 10; ++k )
  {
    bool go_on = true;
    for ( int i = 0; i < 10 && go_on; ++i )
    {
      go_on = false;
      unsigned int size = hole_triangle_.size();
      for ( unsigned int i = 0; i < size; ++i )
	go_on |= refine( hole_triangle_[i] );
    }
    
    go_on = true;
    for ( int i = 0; i < 10 && go_on; ++i )
    {
      go_on = false;
      for ( unsigned int i = 0; i < hole_edge_.size(); ++i )
	go_on |= relax_edge( hole_edge_[i] );
    }
  }
  std::cerr << "ok\n";

}


//=============================================================================
//
// Refine a face: Apply a 1-to-3 split if the edge lengths of the
// face do not match the interpolated edge lengths of the hole
// boundary.
//
//=============================================================================


template< class MeshT >
bool
HoleFiller< MeshT >::refine( FH _fh )
{
 
  // Collect the three edges of the face into e0, e1, e2

  FEI fei = mesh_.fe_iter( _fh );
  EH  e0  = fei.handle(); ++fei;
  EH  e1  = fei.handle(); ++fei;
  EH  e2  = fei.handle(); ++fei;


  // Collect the vertices, vertex positions and scale factors of the face.

  FVI   fvi = mesh_.fv_iter( _fh );

  VH    v0 = fvi.handle(); ++fvi;
  VH    v1 = fvi.handle(); ++fvi;
  VH    v2 = fvi.handle(); ++fvi;

  Point p0 = mesh_.point( v0 );
  Point p1 = mesh_.point( v1 );
  Point p2 = mesh_.point( v2 );

  float scale0 = mesh_.property( scale_, v0 );
  float scale1 = mesh_.property( scale_, v1 );
  float scale2 = mesh_.property( scale_, v2 );

  // Interpolate the scale factor.

  float scale = ( scale0 + scale1 + scale2 ) / 3.0f;
  Point center = float(1.0/3.0) * ( p0 + p1 + p2 );

  float d0 = 1.0f * ( p0 - center ).norm();
  float d1 = 1.0f * ( p1 - center ).norm();
  float d2 = 1.0f * ( p2 - center ).norm();

  // If the edge lengths differ too much from the scale, perform a
  // triangle split.

  if ( ( d0 > scale && d0 > scale0 ) ||
       ( d1 > scale && d1 > scale1 ) ||
       ( d2 > scale && d2 > scale2 ) )
  {
    // Split the face ...

    VH ch = mesh_.add_vertex( center );
    mesh_.split( _fh, ch );

    // ... put the new triangles into the global triangle list ...

    for ( VFI vfi = mesh_.vf_iter( ch ); vfi; ++vfi )
      if ( vfi.handle() != _fh )
	hole_triangle_.push_back( vfi.handle() );

    // ... put the new edges into the global edge list ...

    for ( VEI vei = mesh_.ve_iter( ch ); vei; ++vei )
      hole_edge_.push_back( vei.handle() );

    // ... and set the appropriate scale factor for the new vertex.

    mesh_.property( scale_, ch ) = scale;

    // By splitting the face, its old edges might become "relaxable"

    relax_edge( e0 );
    relax_edge( e1 );
    relax_edge( e2 );

    return true;
  }

  return false;
}


//=============================================================================
//
// Relax an edge: Flip it if one of its opposing vertices lies in
// the circumsphere of the other three vertices.
//
//=============================================================================


template< class MeshT >
bool
HoleFiller< MeshT >::relax_edge( EH _eh )
{
  if ( mesh_.status( _eh ).locked() )
    return false;

  // Abbreviations for the two halfedges of _eh

  HH h0 = mesh_.halfedge_handle( _eh, 0 );
  HH h1 = mesh_.halfedge_handle( _eh, 1 );

  // Get the two end-vertices u and v of the edge
	 
  Point u( mesh_.point( mesh_.to_vertex_handle( h0 ) ) );
  Point v( mesh_.point( mesh_.to_vertex_handle( h1 ) ) );

  // Get the two opposing vertices a and b
    
  Point a( mesh_.point
	   ( mesh_.to_vertex_handle
	     ( mesh_.next_halfedge_handle( h0 ) ) ) );

  Point b( mesh_.point
	   ( mesh_.to_vertex_handle
	     ( mesh_.next_halfedge_handle( h1 ) ) ) );

  // If the circumsphere criterion is fullfilled AND if the flip is
  // topologically admissible, we do it.

  if ( in_circumsphere( a, u, v, b ) ||
       in_circumsphere( b, u, v, a ) )
    if ( mesh_.is_flip_ok( _eh ) )
    {
      mesh_.flip( _eh );
      return true;
    }

  return false;
}


//=============================================================================
//
// Test whether a point _x lies in the circumsphere of _a,_b,_c.
//
//=============================================================================


template< class MeshT >
bool
HoleFiller< MeshT >::in_circumsphere( const Point & _x,
				    const Point & _a,
				    const Point & _b,
				    const Point & _c ) const
{
  Point ab = _b - _a;
  Point ac = _c - _a;

  float a00 = -2.0f * ( ab | _a );
  float a01 = -2.0f * ( ab | _b );
  float a02 = -2.0f * ( ab | _c );
  float b0 = _a.sqrnorm() - _b.sqrnorm();

  float a10 = -2.0f * ( ac | _a );
  float a11 = -2.0f * ( ac | _b );
  float a12 = -2.0f * ( ac | _c );
  float b1 = _a.sqrnorm() - _c.sqrnorm();

  float alpha = -(-a11*a02+a01*a12-a12*b0+b1*a02+a11*b0-a01*b1)
	      / (-a11*a00+a11*a02-a10*a02+a00*a12+a01*a10-a01*a12);
  float beta  =  (a10*b0-a10*a02-a12*b0+a00*a12+b1*a02-a00*b1)
	       / (-a11*a00+a11*a02-a10*a02+a00*a12+a01*a10-a01*a12);
  float gamma = (-a11*a00-a10*b0+a00*b1+a11*b0+a01*a10-a01*b1)
	      / (-a11*a00+a11*a02-a10*a02+a00*a12+a01*a10-a01*a12);

  Point center = alpha * _a + beta * _b + gamma * _c;

  return ( _x - center ).sqrnorm() < ( _a - center ).sqrnorm();
}


//=============================================================================
//
// Create the triangulation
//
// Recursively creates the triangulation for polygon (_i,...,_j).
//
//=============================================================================


template< class MeshT >			
void
HoleFiller< MeshT >::fill( int _i, int _j )
{
  // If the two vertices _i and _j are adjacent, there is nothing to do.

  if ( _i + 1 == _j )
    return;
    

  // Create and store the middle triangle, store its edges.
    
  FH fh = mesh_.add_face( boundary_vertex_[_i],
			  boundary_vertex_[ l_[_i][_j] ],
			  boundary_vertex_[_j] );
  hole_triangle_.push_back( fh );

  hole_edge_.push_back( mesh_.edge_handle
			( mesh_.find_halfedge( boundary_vertex_[_i],
					       boundary_vertex_[ l_[_i][_j] ] ) ) );
  hole_edge_.push_back( mesh_.edge_handle
			( mesh_.find_halfedge( boundary_vertex_[ l_[_i][_j] ],
					       boundary_vertex_[_j] ) ) );
  
  
  // Recursively create the left and right side of the
  // triangulation.
    
  fill( _i, l_[_i][_j] );
  fill( l_[_i][_j], _j );
}



//=============================================================================
//
// Compute the weight of the triangle (_i,_j,_k).
//
//=============================================================================


template< class MeshT >			
typename HoleFiller< MeshT >::Weight
HoleFiller< MeshT >::weight( int _i, int _j, int _k )
{
  // Return an infinite weight if the insertion of this triangle
  // would create complex edges.

  if ( exists_edge( boundary_vertex_[_i], boundary_vertex_[_j] ) ||
       exists_edge( boundary_vertex_[_j], boundary_vertex_[_k] ) ||
       exists_edge( boundary_vertex_[_k], boundary_vertex_[_i] ) )
    return Weight();


  // Return an infinite weight, if one of the neighboring patches
  // could not be created.

    
  if ( l_[_i][_j] == -1 ) return Weight();
  if ( l_[_j][_k] == -1 ) return Weight();

   
  // Compute the maxmimum dihedral angles to the adjacent triangles
  // (if they exist)

  float angle = 0.0f;

  if ( _i + 1 == _j )
    angle = std::max( angle, dihedral_angle( boundary_vertex_[_i],
					     boundary_vertex_[_j],
					     boundary_vertex_[_k],
					     opposite_vertex_[_i] ) );
  else
    angle = std::max( angle, dihedral_angle( boundary_vertex_[_i],
					     boundary_vertex_[_j],
					     boundary_vertex_[_k],
					     boundary_vertex_[l_[_i][_j]] ) );
    
  if ( _j + 1 == _k )
    angle = std::max( angle, dihedral_angle( boundary_vertex_[_j],
					     boundary_vertex_[_k],
					     boundary_vertex_[_i],
					     opposite_vertex_[_j] ) );
  else
    angle = std::max( angle, dihedral_angle( boundary_vertex_[_j],
					     boundary_vertex_[_k],
					     boundary_vertex_[_i],
					     boundary_vertex_[l_[_j][_k]] ) );
    
  if ( _i == 0 && _k == (int) boundary_vertex_.size() - 1 )
    angle = std::max( angle, dihedral_angle( boundary_vertex_[_k],
					     boundary_vertex_[_i],
					     boundary_vertex_[_j],
					     opposite_vertex_[_k] ) );


  return Weight( angle, area( boundary_vertex_[_i],
			      boundary_vertex_[_j],
			      boundary_vertex_[_k] ) );
}



//=============================================================================
//
// Does an edge from vertex _u to _v exist?
//
//=============================================================================


template< class MeshT >			
bool
HoleFiller< MeshT >::exists_edge( VH _u, VH _w )
{
  for ( VOHI vohi = mesh_.voh_iter( _u ); vohi; ++vohi )
    if ( ! mesh_.is_boundary( mesh_.edge_handle( vohi.handle() ) ) )
      if ( mesh_.to_vertex_handle( vohi ) == _w )
	return true;

  return false;
}



//=============================================================================
//
// Compute the area of the triangle (_a,_b,_c).
//
//=============================================================================


template< class MeshT >			
float
HoleFiller< MeshT >::area( VH _a, VH _b, VH _c )
{
  Point a( mesh_.point( _a ) );
  Point b( mesh_.point( _b ) );
  Point c( mesh_.point( _c ) );

  Point n( ( b - a ) % ( c - b ) );

  return 0.5f * n.norm();
}


//=============================================================================
//
// Compute a dihedral angle
//
// Computes the dihedral angle (in degrees) between triangle
// (_u,_v,_a) and triangle (_v,_u,_b), no matter whether these
// triangles actually exist in the current mesh or not).
//
//=============================================================================


template< class MeshT >			
float
HoleFiller< MeshT >::dihedral_angle( VH _u, VH _v, VH _a, VH _b )
{
  Point u( mesh_.point( _u ) );
  Point v( mesh_.point( _v ) );
  Point a( mesh_.point( _a ) );
  Point b( mesh_.point( _b ) );
    
  Point n0( ( v - u ) % ( a - v ) );
  Point n1( ( u - v ) % ( b - u ) );
    
  n0.normalize();
  n1.normalize();
    
  return (float) (acos( n0 | n1 ) * 180.0f / M_PI);
}


//=============================================================================

