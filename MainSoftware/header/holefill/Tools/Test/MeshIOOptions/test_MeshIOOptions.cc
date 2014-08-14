// =============================================================================
// $Id: test_MeshIOOptions.cc,v 1.1.1.1 2004-09-06 12:37:32 botsch Exp $
// -----------------------------------------------------------------------------

#include <vector>
#include <typeinfo>
#include <iostream>
#include <string>
#include <exception>
#include <OpenMesh/Core/IO/Options.hh>
#include <OpenMesh/Tools/Utils/TestingFramework.hh>

// -----------------------------------------------------------------------------

using OpenMesh::Utils::TestingFramework;


// -----------------------------------------------------------------------------


struct test_IOOptions : public TestingFramework::TestFunc
{
   typedef TestingFramework::TestFunc Inherited;
   typedef test_IOOptions Self;
   

   test_IOOptions( TestingFramework& _test, const std::string& _n)
         : Inherited( _test, _n )
   { }
   
   test_IOOptions( Self& _cpy ) : TestingFramework::TestFunc(_cpy) { }
   
   ~test_IOOptions()
   { }
   
      
   void body(void)
   {
      using namespace std;
      using namespace OpenMesh::IO;

      // ---------------------------------------- start the game

      Options opt;
      
      info( "Testing OpenMesh::IO::Options" );

      info( "Check options after default constructor" );
      TH_VERIFY( opt == Options::Default, true );
      TH_VERIFY( opt.check(Options::Binary), false );
      TH_VERIFY( opt.check(Options::MSB),    false );
      TH_VERIFY( opt.check(Options::VertexNormal),   false );
      TH_VERIFY( opt.check(Options::VertexColor),    false );
      TH_VERIFY( opt.check(Options::VertexTexCoord), false );

      info( "Selective set/unset methods" );
      TH_VERIFY( (opt += Options::Binary).check(Options::Binary), true );
      TH_VERIFY( (opt -= Options::Binary).check(Options::Binary), false );
      TH_VERIFY( opt.check(Options::Default), true );
      
      TH_VERIFY( (opt += Options::Binary|Options::MSB).check(Options::Binary|Options::MSB), true );
      TH_VERIFY( (opt -= Options::Binary).check(Options::MSB), true );
      TH_VERIFY( opt == Options::Default, false );
      TH_VERIFY( opt.check(Options::Binary), false );
      
      info("End");
   }
   
};
// ----------------------------------------------------------------------------

int main(void)
{
   TestingFramework testSuite(std::cout);

   new test_IOOptions( testSuite, "OpenMesh::IO::Options");
   
   return testSuite.run();
}


