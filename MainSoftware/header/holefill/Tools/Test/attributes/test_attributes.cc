// ============================================================================
// $Id: test_attributes.cc,v 1.1.1.1 2004-09-06 12:37:38 botsch Exp $
// ----------------------------------------------------------------------------

#include <iostream>
// -------------------- OpenMesh
#include <OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/Types/PolyMesh_ArrayKernelT.hh>
#if defined(OM_USE_OSG)
#  include <OpenMesh/Tools/Kernel_OSG/TriMesh_OSGArrayKernelT.hh>
#endif
#include <OpenMesh/Tools/Utils/TestingFramework.hh>

// ----------------------------------------------------------------------------

using OpenMesh::Utils::TestingFramework;


// ----------------------------------------------------------------------------

template< typename M >
struct test_attributes : public TestingFramework::TestFunc
{
  typedef M                          mesh_t;
  typedef TestingFramework::TestFunc Inherited;   
  typedef test_attributes< mesh_t >  Self;

  test_attributes( TestingFramework& _test, const std::string& _name ) 
    : Inherited( _test, _name )
  { }
   
  test_attributes( Self& _cpy ) : Inherited(_cpy) { }
   
  ~test_attributes()
  { }
   

  void prolog(void)
  {
    begin(name_, "test_attributes");
  }

   
  void body(void)
  {
    using namespace std;
    using namespace OpenMesh;

    mesh_t mesh;

    // ---------------------------------------- start the game

    info("mesh with default traits after default constructor");

    TH_VERIFY( mesh.has_vertex_normals(), false );
    TH_VERIFY( mesh.has_vertex_colors(), false );
    TH_VERIFY( mesh.has_vertex_status(), false );
    TH_VERIFY( mesh.has_vertex_texcoords(), false );

    TH_VERIFY( mesh.has_face_normals(), false );
    TH_VERIFY( mesh.has_face_colors(), false );
    TH_VERIFY( mesh.has_face_status(), false );

    TH_VERIFY( mesh.has_edge_status(), false );

#if defined(OM_VERSION)
    TH_VERIFY( mesh.has_halfedge_status(), false );
#endif

    // ---------------------------------------- request attributes

    info("request attributes...");

    mesh.request_vertex_normals();
    mesh.request_vertex_colors();
    mesh.request_vertex_status();
    mesh.request_vertex_texcoords();
    TH_VERIFY( mesh.has_vertex_normals(),   true );
    TH_VERIFY( mesh.has_vertex_colors(),    true );
    TH_VERIFY( mesh.has_vertex_status(),    true );
    TH_VERIFY( mesh.has_vertex_texcoords(), true );

    mesh.request_face_normals();
    mesh.request_face_colors();
    mesh.request_face_status();
    TH_VERIFY( mesh.has_face_normals(),   true );
    TH_VERIFY( mesh.has_face_colors(),    true );
    TH_VERIFY( mesh.has_face_status(),    true );

    mesh.request_edge_status();
    TH_VERIFY( mesh.has_edge_status(),    true );

#if defined(OM_VERSION)
    mesh.request_halfedge_status();
    TH_VERIFY( mesh.has_halfedge_status(),    true );
#endif

    // ---------------------------------------- release attributes

    info("release attributes...");
      
    mesh.release_vertex_normals();
    mesh.release_vertex_colors();
    mesh.release_vertex_status();
    mesh.release_vertex_texcoords();
    TH_VERIFY( mesh.has_vertex_normals(),   false );
    TH_VERIFY( mesh.has_vertex_colors(),    false );
    TH_VERIFY( mesh.has_vertex_status(),    false );
    TH_VERIFY( mesh.has_vertex_texcoords(), false );

    mesh.release_face_normals();
    mesh.release_face_colors();
    mesh.release_face_status();
    TH_VERIFY( mesh.has_face_normals(),     false );
    TH_VERIFY( mesh.has_face_colors(),      false );
    TH_VERIFY( mesh.has_face_status(),      false );

    mesh.release_edge_status();
    TH_VERIFY( mesh.has_edge_status(),      false );

#if defined(OM_VERSION)
    mesh.release_halfedge_status();
    TH_VERIFY( mesh.has_halfedge_status(),  false );
#endif

    info("End");
  }
   
};

// ----------------------------------------------------------------------------

typedef OpenMesh::TriMesh_ArrayKernelT<>    TriMesh;
typedef OpenMesh::PolyMesh_ArrayKernelT<>   PolyMesh;
#if defined(OM_USE_OSG)
typedef OpenMesh::Kernel_OSG::TriMesh_OSGArrayKernelT<> OSGTriMesh;
#endif

// ----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
#if defined(OM_USE_OSG)
  osg::osgInit(argc, argv);
#endif

  TestingFramework testSuite(std::cout);

  new test_attributes< TriMesh >( testSuite, "TriMesh" );
  new test_attributes< PolyMesh >( testSuite, "PolyMesh" );
#if defined(OM_USE_OSG)
  new test_attributes< OSGTriMesh >( testSuite, "OSGTriMesh" );
#endif
  
  return testSuite.run();
}
