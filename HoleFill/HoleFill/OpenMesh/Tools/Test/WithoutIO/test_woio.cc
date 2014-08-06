// Test: Use mesh without IO facilities.
// 
// Result: This code must compile without any warning or error!
//
#include <OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Utils/Property.hh>
#include <OpenMesh/Core/Math/VectorT.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;

int main(void)
{
  Mesh mesh;

  OpenMesh::VPropHandleT<float> vprop_float;
  OpenMesh::FPropHandleT<OpenMesh::Vec6f> fprop_vec6f;
  
  mesh.add_property( vprop_float,  "vprop-float" );
  mesh.add_property( fprop_vec6f,  "fprop-vec6f" );

  OpenMesh::VertexHandle vh1 = mesh.add_vertex( Mesh::Point(0,0,0) );
  OpenMesh::VertexHandle vh2 = mesh.add_vertex( Mesh::Point(1,1,0) );
  OpenMesh::VertexHandle vh3 = mesh.add_vertex( Mesh::Point(0,1,1) );
  OpenMesh::FaceHandle   fh  = mesh.add_face( vh1, vh2, vh3 );

  mesh.property( vprop_float, vh1 ) = 1.0;
  mesh.property( fprop_vec6f, fh  ) = OpenMesh::Vec6f(1,2,3,4,5,6);
  return 0;
}
