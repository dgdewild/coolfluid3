// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "Common/CBuilder.hpp"
#include "Common/OptionURI.hpp"
#include "Common/OptionArray.hpp"
#include "Common/FindComponents.hpp"


#include "Mesh/CRegion.hpp"
#include "Mesh/CField.hpp"
#include "Mesh/CMesh.hpp"
#include "Mesh/CElements.hpp"

#include "RDM/NavierStokes/SubsonicOutFlowWeakBc.hpp"

#include "Physics/NavierStokes/Cons2D.hpp"

/////////////////////////////////////////////////////////////////////////////////////

using namespace CF::Common;
using namespace CF::Mesh;
using namespace CF::Solver;

namespace CF {
namespace RDM {

///////////////////////////////////////////////////////////////////////////////////////

Common::ComponentBuilder < SubsonicOutFlowWeakBc, RDM::BoundaryTerm, LibRDM > SubsonicOutFlowWeakBc_Builder;

Common::ComponentBuilder < FaceLoopT< SubsonicOutFlowWeakBc, Physics::NavierStokes::Cons2D>, RDM::FaceLoop, LibRDM > SubsonicOutFlowWeakBc_Euler2D_Builder;

///////////////////////////////////////////////////////////////////////////////////////

SubsonicOutFlowWeakBc::SubsonicOutFlowWeakBc ( const std::string& name ) :
  RDM::BoundaryTerm(name)
{
  regist_typeinfo(this);

  // options

  m_options.add_option< OptionURI > ("solution", URI("cpath:"))
      ->set_description("Solution field where to apply the boundary condition")
      ->set_pretty_name("Solution")
      ->attach_trigger ( boost::bind ( &SubsonicOutFlowWeakBc::config_mesh,   this ) )
      ->mark_basic();

  m_options["mesh"].attach_trigger ( boost::bind ( &SubsonicOutFlowWeakBc::config_mesh, this ) );

  m_options.add_option< OptionT<std::string> > ("p_out", std::string() )
      ->set_description("Outlet pressure (vars x,y)")
      ->attach_trigger ( boost::bind ( &SubsonicOutFlowWeakBc::config_pressure_function, this ) )
      ->mark_basic();

  pressure_function.variables("x,y,z");

}

////////////////////////////////////////////////////////////////////////////////

void SubsonicOutFlowWeakBc::config_pressure_function()
{
  pressure_function.functions( m_options["p_out"].value< std::string >() );
  pressure_function.parse();
}


////////////////////////////////////////////////////////////////////////////////

void SubsonicOutFlowWeakBc::config_mesh()
{
  cf_assert( is_not_null( m_mesh.lock() ) );

  URI sol_uri  = option("solution").value<URI>();
  solution = access_component_ptr(sol_uri)->as_ptr<CField>();
  if( is_null(solution.lock()) )
    solution = find_component_ptr_with_tag<CField>( *(m_mesh.lock()) , "solution" );

  if( is_null(solution.lock()) )
    throw CastingFailed (FromHere(),
                         "Could not find a solution field on mesh "
                         + m_mesh.lock()->uri().string() );

}

/////////////////////////////////////////////////////////////////////////////////////

void SubsonicOutFlowWeakBc::execute()
{
  ElementLoop& loop = access_element_loop( type_name() );

  // loop on all regions configured by the user

  boost_foreach(Mesh::CRegion::Ptr& region, m_loop_regions)
  {

//    std::cout << "REGION [" << region->uri().string() << "]" << std::endl;

    loop.select_region( region );

    // loop all elements of this region

    loop.execute();
  }
}

////////////////////////////////////////////////////////////////////////////////////

} // RDM
} // CF

////////////////////////////////////////////////////////////////////////////////////
