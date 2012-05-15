// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "common/OptionList.hpp"
#include "common/OptionT.hpp"
#include "common/OptionComponent.hpp"

#include "mesh/InterpolationFunction.hpp"
#include "mesh/Dictionary.hpp"

//////////////////////////////////////////////////////////////////////////////

namespace cf3 {
namespace mesh {

  using namespace common;

//////////////////////////////////////////////////////////////////////////////

InterpolationFunction::InterpolationFunction( const std::string& name )
  : Component(name)
{

  options().add_option("dict",m_dict)
      .description("Dictionary used to find interpolation points and weights")
      .link_to(&m_dict);
}

////////////////////////////////////////////////////////////////////////////////

} // mesh
} // cf3