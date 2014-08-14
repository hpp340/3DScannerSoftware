#define TA 1
#define PA 0

#if TA
#  include <OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh>
#  define MESH_TYPE TriMesh_ArrayKernelT

#elif PA
#  include <OpenMesh/Core/Mesh/Types/PolyMesh_ArrayKernelT.hh>
#  define MESH_TYPE PolyMesh_ArrayKernelT

#else
#  error You should define a mesh type
#endif


using namespace OpenMesh;
using namespace OpenMesh::Attributes;


struct MyTraits1 : public virtual DefaultTraits
{
  VertexAttributes   (Attributes::Normal);
  HalfedgeAttributes (Attributes::PrevHalfedge);
  FaceAttributes     (Attributes::Normal);

  VertexTraits
  {
    typename Refs::Point aa;
  };

  FaceTraits
  {
    typename Refs::VertexHandle traits_vertex_handle;
  };
};


struct MyTraits2 : public virtual DefaultTraits
{
  VertexAttributes  (Attributes::Status);
  EdgeAttributes    (Attributes::Status);
  FaceAttributes    (Attributes::Status);

  VertexTraits
  {
    typename Refs::Point bb;
  };
};

OM_Merge_Traits(MyTraits1, MyTraits2, MyTraits);


typedef MESH_TYPE<MyTraits>  MyMesh;



int main()
{
  MyMesh                   m;
  MyMesh::VertexHandle     vh0, vh1, vh2, vh3;
  MyMesh::FaceHandle       fh;
  MyMesh::EdgeHandle       eh;
  MyMesh::HalfedgeHandle   heh0, heh1;

  //MyMesh::VertexIter       vi;
  //MyMesh::ConstVertexIter  cvi(vi);
  //MyMesh::ConstVertexFaceIter cvfit;


  // use face's vertex handle
  MyMesh::Face f;
  f.traits_vertex_handle = MyMesh::VertexHandle(0);


  MyMesh::Vertex v;
  v.aa = v.bb = MyMesh::Point(1,2,3);

  
  // sizes
  std::cout 
    << "\nsizeof(MyMesh::Point)    = " 
    << sizeof(MyMesh::Point) 
    << "\nsizeof(MyMesh::Vertex)   = " 
    << sizeof(MyMesh::Vertex)
    << "\nsizeof(MyMesh::Halfedge) = " 
    << sizeof(MyMesh::Halfedge)
    << "\nsizeof(MyMesh::Edge)     = " 
    << sizeof(MyMesh::Edge)
    << "\nsizeof(MyMesh::Face)     = " 
    << sizeof(MyMesh::Face) 
    << "\n\n";


  // support prev halfedge ?
  std::cout << "stores prev halfedge: " 
 	    << MyMesh::has_prev_halfedge()
	    << "\n\n";





  // add vertices
  vh0 = m.add_vertex(MyMesh::Point(1,0,0));
  vh1 = m.add_vertex(MyMesh::Point(0,2,0));
  vh2 = m.add_vertex(MyMesh::Point(0,0,3));
  vh3 = m.add_vertex(MyMesh::Point(4,0,0));



  // add face
  std::vector<MyMesh::VertexHandle> vhandles(3);
  vhandles[0] = vh0;  vhandles[1] = vh1;  vhandles[2] = vh2;
  fh = m.add_face(vhandles);
  vhandles[0] = vh2;  vhandles[1] = vh1;  vhandles[2] = vh3;
  fh = m.add_face(vhandles);

  std::cout << m.n_vertices() << " vertices, "
	    << m.n_edges()    << " edges, "
	    << m.n_faces()    << " faces\n\n";


  // edge-halfedge test
  heh0 = m.halfedge_handle(fh);
  heh1 = m.opposite_halfedge_handle(heh0);
  eh   = m.edge_handle(heh1);
  std::cout << "edge <-> halfedge test:\n";
  std::cout << "{" << heh0 << ", " << heh1 << "} == {"
	    << m.halfedge_handle(eh, 0) << ", "
	    << m.halfedge_handle(eh, 1) << "}\n\n";


  // opposite test
  std::cout << "opposite test:\n";
  std::cout << heh0 
	    << " == " 
	    << m.opposite_halfedge_handle(heh1) 
	    << " && "
	    << heh1 
	    << " == " 
	    << m.opposite_halfedge_handle(heh0) 
	    << "\n\n";


  // next-prev test
  std::cout << "next <-> prev test:\n";
  heh1 = m.prev_halfedge_handle(heh0);
  std::cout << heh0 
	    << " == " 
	    << m.next_halfedge_handle(heh1) 
	    << "\n\n";


  

  // additional attributes
  m.update_face_normals();
  m.update_vertex_normals();
  


  // halfedge iter
  MyMesh::HalfedgeIter  hIt(m.halfedges_begin()), 
                        hEnd(m.halfedges_end());
  int count=0;
  std::cout << "HalfedgeIter test:\n";
  for (; hIt != hEnd; ++hIt, ++count);
  std::cout << count << " halfedges\n\n";


  // deletion test
  m.delete_vertex(MyMesh::VertexHandle(0));
  m.garbage_collection();


  std::cout << m.n_vertices() << " vertices, "
	    << m.n_edges()    << " edges, "
	    << m.n_faces()    << " faces\n\n";
}
