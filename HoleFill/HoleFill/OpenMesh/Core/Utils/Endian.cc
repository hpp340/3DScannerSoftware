//=============================================================================
//                                                                            
//                               OpenMesh                                     
//      Copyright (C) 2001-2005 by Computer Graphics Group, RWTH Aachen       
//                           www.openmesh.org                                 
//                                                                            
//-----------------------------------------------------------------------------
//                                                                            
//                                License                                     
//                                                                            
//   This library is free software; you can redistribute it and/or modify it 
//   under the terms of the GNU Library General Public License as published  
//   by the Free Software Foundation, version 2.                             
//                                                                             
//   This library is distributed in the hope that it will be useful, but       
//   WITHOUT ANY WARRANTY; without even the implied warranty of                
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
//   Library General Public License for more details.                          
//                                                                            
//   You should have received a copy of the GNU Library General Public         
//   License along with this library; if not, write to the Free Software       
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                 
//                                                                            
//-----------------------------------------------------------------------------
//                                                                            
//   $Revision: 1.2 $
//   $Date: 2005-12-21 13:53:48 $
//                                                                            
//=============================================================================


//=============================================================================
//
//  Helper Functions for binary reading / writing
//
//=============================================================================


//== INCLUDES =================================================================


#include <iostream>
#include <algorithm>
//#include <OpenMesh/Core/Utils/Endian.hh>
#include"../Utils/Endian.hh"

//== NAMESPACES ===============================================================


namespace OpenMesh {


//== IMPLEMENTATION ===========================================================

//-----------------------------------------------------------------------------

int Endian::one_ = 1;

const Endian::Type Endian::local_ = *((unsigned char*)&Endian::one_)
   ? Endian::LSB
   : Endian::MSB;

const char * Endian::as_string(Type _t)
{
   return _t == LSB ? "LSB" : "MSB";
}
   
//=============================================================================
} // namespace OpenMesh
//=============================================================================
