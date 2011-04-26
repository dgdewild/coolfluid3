// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "Common/CBuilder.hpp"

#include "LibSF.hpp"
#include "ETLine1DLagrangeP1.hpp"
#include "Point1DLagrangeP1.hpp"

namespace CF {
namespace Mesh {
namespace SF {

////////////////////////////////////////////////////////////////////////////////

Common::ComponentBuilder < ETLine1DLagrangeP1,ElementType2, LibSF > ETLine1DLagrangeP1_Builder;

////////////////////////////////////////////////////////////////////////////////

ETLine1DLagrangeP1::ETLine1DLagrangeP1(const std::string& name) : Line(name)
{
  m_nb_nodes = nb_nodes;
  m_order = order;
}

////////////////////////////////////////////////////////////////////////////////

Real ETLine1DLagrangeP1::compute_volume(const NodesT& coord) const
{
  return volume(coord);
}

////////////////////////////////////////////////////////////////////////////////

void ETLine1DLagrangeP1::compute_centroid(const NodesT& coord , RealVector& centroid) const
{
  centroid[0] = 0.5*(coord(0,XX)+coord(1,XX));
}

////////////////////////////////////////////////////////////////////////////////

bool ETLine1DLagrangeP1::is_coord_in_element( const RealVector& coord, const NodesT& nodes) const
{
  MappedCoordsT mapped_coord;
  mapped_coordinates(CoordsT(coord), nodes, mapped_coord);
  if( (mapped_coord[KSI] >= -0.5) &&
      (mapped_coord[KSI] <= 0.5) )
  {
    return true;
  }
  else
  {
    return false;
  }
}

////////////////////////////////////////////////////////////////////////////////

const CF::Mesh::ElementType2::FaceConnectivity& ETLine1DLagrangeP1::face_connectivity() const
{
  return faces();
}

////////////////////////////////////////////////////////////////////////////////

const CF::Mesh::ElementType2& ETLine1DLagrangeP1::face_type(const CF::Uint face) const
{
  throw Common::NotImplemented(FromHere(),"");
  // const static Point1DLagrangeP1 facetype;
  const static ETLine1DLagrangeP1 facetype;
  return facetype;
}

////////////////////////////////////////////////////////////////////////////////

const CF::Mesh::ElementType2::FaceConnectivity& ETLine1DLagrangeP1::faces()
{
  static FaceConnectivity connectivity;
  if(connectivity.face_first_nodes.empty())
  {
    connectivity.face_first_nodes = boost::assign::list_of(0)(1);
    connectivity.face_node_counts.assign(2, 1);
    connectivity.face_nodes = boost::assign::list_of(0)
                                                    (1);
  }
  return connectivity;
}

////////////////////////////////////////////////////////////////////////////////

void ETLine1DLagrangeP1::mapped_coordinates(const CoordsT& coord, const NodesT& nodes, MappedCoordsT& mappedCoord)
{
  const Real x0 = nodes(0, XX);
  const Real x1 = nodes(1, XX);
  mappedCoord[KSI] = (2*coord[0] - (x1 + x0)) / (x1 - x0);
}

////////////////////////////////////////////////////////////////////////////////

Real ETLine1DLagrangeP1::jacobian_determinant(const MappedCoordsT& mappedCoord, const NodeMatrixT& nodes)
{
  return 0.5*volume(nodes);
}

////////////////////////////////////////////////////////////////////////////////

void ETLine1DLagrangeP1::jacobian(const MappedCoordsT& mappedCoord, const NodeMatrixT& nodes, JacobianT& result)
{
  result(KSI,XX) = jacobian_determinant(mappedCoord, nodes);
}

////////////////////////////////////////////////////////////////////////////////

void ETLine1DLagrangeP1::jacobian_adjoint(const MappedCoordsT& mappedCoord, const NodeMatrixT& nodes, JacobianT& result)
{
  result(KSI,XX) = 1.;
}

////////////////////////////////////////////////////////////////////////////////

} // SF
} // Mesh
} // CF