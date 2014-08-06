// ----------------------------------------------------------------------------
// Test: Read an arbitrary mesh add custom properties
//       with fundamental types and a custom type
//       and store enhanced mesh in result.om and restore it again.
//
// ----------------------------------------------------------------------------

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Utils/getopt.h>

typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;

// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------

void usage_and_exit( int xcode )
{
  std::cout << "Usage: test_om -i <name>.om" << std::endl;
  exit(xcode);
}

// ----------------------------------------------------------------------------

struct my_data
{  
  my_data() 
    : b_(true), l_(1), d_(3) 
  { }

  bool operator == ( const my_data& _rhs )
  {
    return b_ == _rhs.b_ && l_ == _rhs.l_ && d_==_rhs.d_;
  }

  bool   b_;
  long   l_;
  double d_;
};


namespace OpenMesh {
  namespace IO {

    template <> struct binary< my_data >
    {
      typedef my_data value_type;

      static const bool is_streamable = true;

      static size_t size_of(void)
      { return IO::size_of<bool>()+IO::size_of<long>()+IO::size_of<double>(); }

      static size_t size_of( const value_type& _v) { return size_of(); }

      static 
      size_t store( std::ostream& _os, const value_type& _v,
		    bool _swap = false)
      {
	size_t bytes;

	bytes  = IO::store( _os, _v.b_, _swap );
	bytes += IO::store( _os, _v.l_, _swap );
	bytes += IO::store( _os, _v.d_, _swap );

	return bytes;
      }
      
      static 
      size_t restore( std::istream& _is, value_type& _v,
		      bool _swap = false)
      {
	size_t bytes;

	bytes  = IO::restore( _is, _v.b_, _swap );
	bytes += IO::restore( _is, _v.l_, _swap );
	bytes += IO::restore( _is, _v.d_, _swap );

	return bytes;
      }      
    };
  }
}


// ----------------------------------------------------------------------------


std::ostream& operator << ( std::ostream& _os, my_data& _d)
{
  _os << "mydata[" << _d.b_ << ", " << _d.l_ << ", " << _d.d_ << std::endl;
  return _os;
}


// ----------------------------------------------------------------------------


int main(int argc, char *argv[])
{
  int         c;
  std::string ifname;

  // ---------------------------------------- command line parsing

  while ( (c=getopt(argc,argv,"i:h")) != -1 )
  {
    switch(c)
    {
      case 'i': ifname = optarg; break;
      case 'h': usage_and_exit(0);
      default:  usage_and_exit(1);
    }
  }

  // ---------------------------------------- prerequisites

  if (ifname.empty())
    usage_and_exit(1);

  // ----------------------------------------  

  try
  {
    Mesh mesh;
  
    std::clog << "+ read file " << ifname << std::endl;

    OpenMesh::IO::read_mesh( mesh, ifname );

    mesh.request_vertex_normals();
    mesh.request_vertex_colors();
    mesh.request_vertex_texcoords();
    mesh.request_vertex_status();
    mesh.request_halfedge_status();
    mesh.request_edge_status();
    mesh.request_face_normals();
    mesh.request_face_colors();
    mesh.request_face_status();

    std::clog << "+ property info\n";
    mesh.property_stats();


    std::clog << "+ adding custom properties\n";

    OpenMesh::VPropHandleT<float>   vprop_float;
    OpenMesh::VPropHandleT<my_data> vprop_data;
    OpenMesh::HPropHandleT<float>   hprop_float;
    OpenMesh::EPropHandleT<float>   eprop_float;
    OpenMesh::EPropHandleT<OpenMesh::Vec3f> eprop_vec3f;
    OpenMesh::FPropHandleT<float>   fprop_float;
    OpenMesh::FPropHandleT<bool>    fprop_bool;
    OpenMesh::FPropHandleT<std::string>  fprop_string;
    OpenMesh::MPropHandleT<float>   mprop_float;
    
    mesh.add_property( vprop_float,  "vprop-float" );
    mesh.add_property( vprop_data,   "vprop-data" );
    mesh.add_property( hprop_float,  "hprop-float" );
    mesh.add_property( eprop_float,  "eprop-float" );
    mesh.add_property( eprop_vec3f,  "eprop-vec3f" );
    mesh.add_property( fprop_float,  "fprop-float" );
    mesh.add_property( fprop_bool,   "fprop-bool" );
    mesh.add_property( fprop_string, "fprop-string" );
    mesh.add_property( mprop_float,  "mprop-float" );

    if ( vprop_data.is_valid() )
    {
      std::cout << "data is streamable: " 
                << OpenMesh::IO::binary<my_data>::is_streamable
                << std::endl;

      Mesh::VertexIter vi;
      my_data          d;

      d.l_ = mesh.n_vertices();

      for (vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
      {
        mesh.property( vprop_data, vi ) = d; --d.l_;
      }
    }

    if ( vprop_float.is_valid())
    {
      Mesh::VertexIter vi;
      float            f = 0.0;
      for (vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
        mesh.property( vprop_float, vi ) = (f+=1.1f);
    }

    if (fprop_float.is_valid()  ||
        fprop_bool.is_valid()   || 
        fprop_string.is_valid() )
    {
      Mesh::FaceIter   fi;
      size_t           i = 0;

      for (fi = mesh.faces_begin(); fi != mesh.faces_end(); ++fi)
      {
        if ( fprop_string.is_valid() )
        {
          char C[6]; 
          C[0] = size_t('a')+ (i++ % 10); 
          C[1] = 'A'; 
          C[2] = 'B';
          C[3] = 'C';
          C[4] = 'D';
          C[5] = '\0';
          mesh.property( fprop_string, fi ) = C;
        }
      }
    }


    std::clog << "+ make them persistent\n";
  
    
    mesh.property(vprop_float).set_persistent(true);
    mesh.property(vprop_data).set_persistent(true);
    mesh.property(hprop_float).set_persistent(true);
    mesh.property(eprop_float).set_persistent(true);
    mesh.property(eprop_vec3f).set_persistent(true);
    mesh.property(fprop_float).set_persistent(true);
    mesh.property(fprop_bool).set_persistent(true);
    mesh.property(fprop_string).set_persistent(true);
    mesh.mproperty(mprop_float).set_persistent(true);

    std::clog << "+ property info\n";
    mesh.property_stats();

    std::clog << "+ write file test.om\n";

    OpenMesh::IO::Options opt;

    opt += OpenMesh::IO::Options::VertexNormal;
    opt += OpenMesh::IO::Options::FaceNormal;
    
    mesh.update_face_normals();
    mesh.update_vertex_normals();

    if ( !OpenMesh::IO::write_mesh( mesh, "test.om", opt ) )
      throw std::runtime_error("Could write file 'test.om'!");
  }
  catch( std::exception& x )
  {
    std::cerr << "Error: " << x.what() << std::endl;
    return 1;
  }

  try
  {
    Mesh mesh;
  
    std::clog << "--------------------------------------\n";
    std::clog << "+ read file test.om\n";

    OpenMesh::VPropHandleT<float>    vprop_float;
    OpenMesh::VPropHandleT<my_data>  vprop_data;
    OpenMesh::HPropHandleT<float>    hprop_float;
    OpenMesh::EPropHandleT<float>    eprop_float;
    OpenMesh::EPropHandleT<OpenMesh::Vec3f> eprop_vec3f;
    OpenMesh::FPropHandleT<float>    fprop_float;
    OpenMesh::FPropHandleT<bool>     fprop_bool;
    OpenMesh::FPropHandleT<std::string>  fprop_string;
    OpenMesh::MPropHandleT<float>    mprop_float;
    
    mesh.add_property( vprop_float,  "vprop-float" );
    mesh.add_property( vprop_data,   "vprop-data" );
    mesh.add_property( hprop_float,  "hprop-float" );
    mesh.add_property( eprop_float,  "eprop-float" );
    mesh.add_property( eprop_vec3f,  "eprop-vec3f" );
    mesh.add_property( fprop_float,  "fprop-float" );
    mesh.add_property( fprop_bool,   "fprop-bool" );
    mesh.add_property( fprop_string, "fprop-string" );
    mesh.add_property( mprop_float,  "mprop-float" );

    if ( !OpenMesh::IO::read_mesh( mesh, "test.om" ) )
    {
      std::clog << "Cannot load mesh!\n";
      return 1;
    }

    if ( vprop_float.is_valid() )
    {
      Mesh::VertexIter vi;
      for (vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi)
      {
	std::clog << "f = " << mesh.property( vprop_float, vi ) << std::endl;
	break;
      }
    }

    if ( vprop_data.is_valid() )
    {
      Mesh::VertexIter vi = mesh.vertices_begin();
      std::clog << " data = " << mesh.property( vprop_data, vi ) << std::endl;
    }

  }
  catch( std::exception& x )
  {
    std::cerr << "Error: " << x.what() << std::endl;
    return 1;
  }

  return 0;
}
