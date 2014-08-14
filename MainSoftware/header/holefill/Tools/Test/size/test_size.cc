#include <OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh>
#include <iostream>

using namespace OpenMesh;

struct woPHETraits : DefaultTraits
{
  HalfedgeAttributes(0);
};

typedef TriMesh_ArrayKernelT<> Mesh_w_phe;
typedef TriMesh_ArrayKernelT<woPHETraits> Mesh_wo_phe;

#define SIZEOF( entity,b ) \
  std::cout << "size of " << #entity << ": " << sizeof( entity ) << std::endl; \
  b += sizeof( entity )

template <typename Mesh> 
void print_size(void)
{
  size_t total=0;
  SIZEOF(Mesh::Vertex, total);
  SIZEOF(Mesh::Halfedge, total);
  SIZEOF(Mesh::Edge, total);
  SIZEOF(Mesh::Face, total);
  std::cout << "total: " << total << std::endl;
}

int main(int argc, char *argv[])
{
  std::cout << "w/o previous halfedge\n";
  print_size<Mesh_wo_phe>();
  std::cout << std::endl;
  
  std::cout << "w/ previous halfedge\n";
  print_size<Mesh_w_phe>();
  std::cout << std::endl;

  return 0;
}
