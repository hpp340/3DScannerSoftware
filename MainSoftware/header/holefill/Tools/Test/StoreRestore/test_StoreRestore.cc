// ============================================================================
// $Id: test_StoreRestore.cc,v 1.1.1.1 2004-09-06 12:37:33 botsch Exp $
// ----------------------------------------------------------------------------

#include <OpenMesh/Core/System/config.h>
#include <vector>
#include <typeinfo>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <exception>
#include <algorithm>
#if defined(OM_CC_MIPS)
#  include <ctype.h>
#  include <limits.h>
#else
#  include <cctype>
#  include <limits>
#endif
#include <OpenMesh/Core/IO/SR_store.hh>
#include <OpenMesh/Tools/Utils/TestingFramework.hh>

// ----------------------------------------------------------------------------

using OpenMesh::Utils::TestingFramework;


// ----------------------------------------------------------------------------

template<typename V>
struct test_reverse_byte_order : public TestingFramework::TestFunc
{
  typedef V                          value_type;
  typedef TestingFramework::TestFunc Inherited;
  typedef test_reverse_byte_order Self;
   
  value_type val_a_;
  value_type val_b_;

  test_reverse_byte_order( TestingFramework& _test, const std::string& _n,
			   V _val_a, V _val_b)
    : Inherited( _test, _n ),
      val_a_(_val_a), val_b_(_val_b)
  { }
   
  test_reverse_byte_order( Self& _cpy ) : TestingFramework::TestFunc(_cpy) { }
   
  ~test_reverse_byte_order()
  { }
   
  //    bool equal(value_type a, value_type b)
  //    {
  //      return a == b;
  //    }

  void write( std::ostream& ostr, value_type val )
  {
    using namespace std;      
    unsigned char *a = (unsigned char*)&val;      
    ostr << setw(2) << hex << (unsigned int)a[0];      
    for (size_t i=1; i < sizeof(value_type); ++i)
      ostr << '|' << setw(2) << hex << (unsigned int)a[i];      
    ostr << dec << ", " << val;
  }

  void prolog(void)
  {
    begin(name_, "reverse_byte_order");
  }

   
  void body(void)
  {
    using namespace std;
    using namespace OpenMesh;

    // ---------------------------------------- start the game

    {
      ostringstream ostr;

      ostr << "Value of type " << typeid(value_type).name()
	   << " with size " << sizeof(value_type) << " bytes.";
      info( ostr );
    }

    {
      ostringstream ostr;
      write( ostr, val_a_);
      info(ostr);
    }
         
    {
      ostringstream ostr;
      write(ostr, val_b_);
      info(ostr);
    }         
    IO::reverse_byte_order(val_a_);
    {
      ostringstream ostr;
      write(ostr, val_a_);
      info(ostr);
    }         
    TH_VERIFY( val_a_, val_b_ );
      
    info("End");
  }
   
};


// ----------------------------------------------------------------------------

template<typename V>
struct test_binary_store_restore : public TestingFramework::TestFunc
{
  typedef V                          value_type;
  typedef TestingFramework::TestFunc Inherited;
  typedef test_binary_store_restore  Self;
   
  value_type val_a_;
  value_type val_b_;

  test_binary_store_restore( TestingFramework& _test, const std::string& _n,
			     V& _val_a, V& _val_b)
    : Inherited( _test, _n ),
      val_a_(_val_a), val_b_(_val_b)
  { }
   
  test_binary_store_restore( Self& _cpy ) : 
    TestingFramework::TestFunc(_cpy) { }
   
  ~test_binary_store_restore()
  { }
      
  void prolog(void)
  {
    begin(name_, "binary store restore");
  }

  void body(void)
  {
    using namespace std;
    using namespace OpenMesh;
    
    // ---------------------------------------- start the game
    
    value_type   tmpv;
    
    {
      ostringstream ostr;
      
      ostr << "Value of type " << typeid(value_type).name()
	   << " with size " << IO::binary<value_type>::size_of(val_a_) 
	   << " bytes.\n";
      ostr << "    is streamable: " 
	   << IO::is_streamable<value_type>()
	   << " [=" << IO::is_streamable(tmpv) << "]";
      info( ostr );
    }
    
    {
      size_t bytes;
      stringstream str;
      bytes = IO::binary< value_type >::store(   str, val_a_ );
      TH_VERIFY( bytes, IO::binary< value_type >::size_of( val_a_ ) );
      bytes = IO::binary< value_type >::restore( str, tmpv );
      TH_VERIFY( bytes, IO::binary< value_type >::size_of( tmpv ) );
    }
    TH_VERIFY( val_a_, tmpv );
    
    {
      stringstream str;
      IO::binary< value_type >::store(   str, val_a_, true ); // reverse
      IO::binary< value_type >::restore( str, tmpv,   false );
    }

    TH_VERIFY( val_b_, tmpv );
    
    
    info("End");
  }   
};

// ----------------------------------------------------------------------------

template < typename T >
void write_hex( std::ostream& _os, const T& _val, const std::string& _pat)
{
  size_t N = sizeof(T);

  assert( N == _pat.size() );

  const unsigned char *p = reinterpret_cast<const unsigned char*>(&_val);

  _os << std::hex;
  for (size_t i=0; i<N; ++i)
    _os << std::setfill('0') << std::setw(2) << (int)(p[i]) << ' ';
  _os << "  [";
  for (size_t i=0; i<N; ++i)
    _os << std::setfill('0') << std::setw(2) << (_pat[i]) << ' ';
  _os << "]";
  _os << std::dec << std::endl;
}

int main(void)
{
  TestingFramework testSuite(std::cout);

  std::string pattern = "abcdefghijklmnop";
  std::string rpattern;
   
  union
  {
    char     pattern[32];
      
    char              c;
    unsigned char     uc;
    short             s;
    unsigned short    us;
    int               i;
    unsigned int      ui;      
    long              l;
    unsigned long     ul;
#if defined(OM_CC_MSVC)
    __int64           ll;
    unsigned __int64  ull;
#else
    long long         ll;
    unsigned long long ull;
#endif
    float             f;
    double            d;
    long double       ld;
  } a, b;

  a.c  = '1';
  b.c  = '1';
  new test_reverse_byte_order<char>( testSuite, "char", a.c, b.c );
  new test_reverse_byte_order<unsigned char>( testSuite, "unsigned char", a.uc, b.uc );

  a.s  = 0x0102;
  b.s  = 0x0201;
  new test_reverse_byte_order<short>(  testSuite, "short", a.s, b.s );

  a.s  = 0x0102;
  b.s  = 0x0201;
  new test_reverse_byte_order<unsigned short>( testSuite, 
					       "unsigned short", 
					       a.us  , b.us );

  a.i  = 0x01020304;
  b.i  = 0x04030201;
  new test_reverse_byte_order<int>(  testSuite, "int", a.i, b.i );
  a.i = 0x01020304;
  new test_reverse_byte_order<unsigned int>( testSuite, 
					     "unsigned int", a.ui  , b.ui );
  a.l = 0x01020304;
  new test_reverse_byte_order<long>(  testSuite, "long", a.l, b.l );
  a.ul = 0x01020304;
  new test_reverse_byte_order<unsigned long>( testSuite, 
					      "unsigned long", a.ul  , b.ul );

#if defined(OM_CC_MSVC)
  a.ll = 0x0102030405060708;
  b.ll = 0x0807060504030201;
  new test_reverse_byte_order<__int64>( testSuite, "int64", a.ll, b.ll );
  new test_reverse_byte_order<unsigned __int64>( testSuite, 
						 "int64", a.ull, b.ull );
#else
  a.ll = 0x0102030405060708ll;
  b.ll = 0x0807060504030201ll;
  new test_reverse_byte_order<long long>(  testSuite, 
					   "long long", a.ll, b.ll );
  new test_reverse_byte_order<unsigned long long>( testSuite, 
						   "unsigned long long", 
						   a.ull  , b.ull );
#endif
  a.l = 0x01020304;
  b.l = 0x04030201;
  new test_reverse_byte_order<float>(  testSuite, "float", a.f, b.f );

#if defined(OM_CC_MSVC)
  a.ll = 0x0102030405060708;
  b.ll = 0x0807060504030201;
#else
  a.ll = 0x0102030405060708ll;
  b.ll = 0x0807060504030201ll;
#endif
  new test_reverse_byte_order<double>( testSuite, "double", a.d  , b.d );


  {
    long double d1, d2;

    d1 = 12345678.87654321;

    for (size_t i=0; i< sizeof(d1); ++i)
      ((char*)&d2)[sizeof(d1)-i-1] = ((char*)&d1)[i];

    new test_reverse_byte_order<long double>( testSuite, "long double", 
					      d1  , d2 );
  }

  // ------------------------------------------------------------ store/restore

#if 1

#define tbsr test_binary_store_restore

//   new test_binary_store_restore<bool>( testSuite, "bool", true, true );

  a.c = 'a';
  new tbsr<char>( testSuite, "char", a.c, a.c );
  new tbsr<unsigned char>( testSuite, "unsigned char", a.uc, a.uc );

  a.s = -1234; b.s = a.s; OpenMesh::IO::reverse_byte_order( b.s );
  new tbsr<short>( testSuite, "short", a.s, b.s );
  a.us = 1234; b.us = a.us; OpenMesh::IO::reverse_byte_order( b.us );
  new tbsr<unsigned short>( testSuite, "unsigned short", a.us, b.us );

  a.i = -12345678; b.i = a.i; OpenMesh::IO::reverse_byte_order( b.i );
  new tbsr<int>( testSuite, "int", a.i, b.i );
  a.ui = 12345678; b.ui = a.ui; OpenMesh::IO::reverse_byte_order( b.ui );
  new tbsr<unsigned int>( testSuite, "unsigned int", a.ui, b.ui );

  a.l = -87654321; b.l = a.l; OpenMesh::IO::reverse_byte_order( b.l );
  new tbsr<long>( testSuite, "long", a.l, b.l );
  a.ul = 87654321; b.ul = a.ul; OpenMesh::IO::reverse_byte_order( b.ul );
  new tbsr<unsigned long>( testSuite, "unsigned long", a.ul, b.ul );

  a.ll = -987654321; b.ll = a.ll; OpenMesh::IO::reverse_byte_order( b.ll );
  new tbsr<OpenMesh::IO::int64_t>( testSuite, "long long", a.ll, b.ll );
  a.ull = 987654321; b.ull = a.ull; OpenMesh::IO::reverse_byte_order( b.ull );
  new tbsr<OpenMesh::IO::uint64_t>( testSuite, "unsigned long long", a.ull, b.ull );

  a.f = 1.123456f; b.f = a.f; OpenMesh::IO::reverse_byte_order( b.f );
  new tbsr<float>( testSuite, "float", a.f, b.f );

  a.d = 2.123456789; b.d = a.d; OpenMesh::IO::reverse_byte_order( b.d );
  new tbsr<double>( testSuite, "double", a.d, b.d );

#define REVERSE_VECTOR( T ) \
  { OpenMesh:: T v1; for(size_t i=0; i<v1.size(); ++i) v1[i]=OpenMesh::T::value_type(i); \
    OpenMesh:: T v2(v1); \
    for (size_t j=0; j < v2.size(); ++j) \
      OpenMesh::IO::reverse_byte_order( v2[j] ); \
    new tbsr<OpenMesh::T>( testSuite, #T, v1, v2 ); \
  }

#define REVERSE_VECTORS( N ) \
  REVERSE_VECTOR( Vec##N##c ) \
  REVERSE_VECTOR( Vec##N##uc ) \
  REVERSE_VECTOR( Vec##N##s )\
  REVERSE_VECTOR( Vec##N##us )\
  REVERSE_VECTOR( Vec##N##i )\
  REVERSE_VECTOR( Vec##N##ui )\
  REVERSE_VECTOR( Vec##N##f )\
  REVERSE_VECTOR( Vec##N##d )

  REVERSE_VECTORS( 1 )
  REVERSE_VECTORS( 2 )
  REVERSE_VECTORS( 3 )
  REVERSE_VECTORS( 4 )
  REVERSE_VECTORS( 6 )

#undef REVERSE_VECTORS
#undef REVERSE_VECTOR

#endif 

  return testSuite.run();
}

