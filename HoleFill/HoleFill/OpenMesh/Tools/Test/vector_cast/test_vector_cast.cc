// ============================================================================
// $Id: test_vector_cast.cc,v 1.1.1.1 2004-09-06 12:37:47 botsch Exp $
// ----------------------------------------------------------------------------

#include <vector>
#include <typeinfo>
#include <iostream>
#include <iterator>
#include <string>
#include <exception>

#include <OpenMesh/Core/Math/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/GenProg.hh>
#include <OpenMesh/Tools/Utils/TestingFramework.hh>

// ----------------------------------------------------------------------------

using OpenMesh::Utils::TestingFramework;


// ----------------------------------------------------------------------------

class MyVector;

namespace OpenMesh
{
  template <>
  struct vector_traits<MyVector>
  {
    typedef MyVector        vector_type;
    typedef float           value_type;

    static const size_t     size_ = 3;
    static size_t size() { return size_; }
  };
}

std::ostream& operator << ( std::ostream& _ostr, const MyVector& _vec );

// ----------------------------------------------------------------------------

template<typename SrcType, typename DstType >
struct test_vector_cast : public TestingFramework::TestFunc
{
  typedef SrcType                    src_t;
  typedef DstType                    dst_t;
  typedef TestingFramework::TestFunc Inherited;
  typedef test_vector_cast           Self;
      
  src_t   src_;
  dst_t   dst_;

   test_vector_cast( TestingFramework& _test, const std::string& _n,
		     src_t& _src, dst_t& _dst)
     : Inherited( _test, _n ),
       src_(_src), dst_(_dst)
  { }
   
  test_vector_cast( const Self& _cpy ) : TestingFramework::TestFunc(_cpy) { }

  ~test_vector_cast() { }

  void body(void)
  {
    using namespace std;

    dst_t casted;

    casted = OpenMesh::vector_cast<dst_t>(src_);

    TH_VERIFY( casted, dst_ );
    
  }

};

// ----------------------------------------------------------------------------


class MyVector
{
public:

  static const size_t _iSize = 3;

  MyVector()
  {
    std::fill(data_, data_+_iSize, 0);
  }
  MyVector(float a, float b, float c)
  {
    data_[0] = a;
    data_[1] = b;
    data_[2] = c;
  }

  MyVector(const MyVector& _v)
  {
    *this = _v;
  }

  MyVector& operator = ( const MyVector& _rhs )
  {
    std::copy(_rhs.data_, _rhs.data_+_iSize, data_);
    return *this;
  }

  bool operator == ( const MyVector& _rhs ) const
  {
    return std::equal(_rhs.data_, _rhs.data_+_iSize, data_);
  }

  float& operator [] (size_t idx) { return data_[idx]; }

  const float& operator [] (size_t idx) const { return data_[idx]; }

  float data_[_iSize];
};

std::ostream& operator << ( std::ostream& _ostr, const MyVector& _vec )
{
  std::copy( _vec.data_, _vec.data_+_vec._iSize,
	     std::ostream_iterator<float>(_ostr, " ") );
  return _ostr;
};

// ----------------------------------------------------------------------------
int main(void)
{
   using namespace std;
   
   TestingFramework testSuite(std::cout);

   {
     OpenMesh::Vec3f src( 1, 2, 3);
     OpenMesh::Vec3f dst( 1, 2, 3);

     new test_vector_cast<
       OpenMesh::Vec3f,
       OpenMesh::Vec3f>( testSuite, 
			 "OpenMesh::Vec3f <- OpenMesh::Vec3f", 
			 src, dst );
   }
  
   {
     OpenMesh::Vec3f src( 1, 2, 3);
     MyVector        dst( 1, 2, 3);

     new test_vector_cast<
       OpenMesh::Vec3f,
       MyVector>( testSuite, 
		  "MyVector <- OpenMesh::Vec3f", 
		  src, dst );
   }
  
   {
     MyVector        src( 1, 2, 3);
     OpenMesh::Vec3f dst( 1, 2, 3);

     new test_vector_cast<
       MyVector,       
       OpenMesh::Vec3f>( testSuite, 
			 "OpenMesh::Vec3f <- MyVector", 
			 src, dst );
   }
  
   return testSuite.run();
}

