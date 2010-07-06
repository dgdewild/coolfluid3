#include <boost/foreach.hpp>

#include "Common/ObjectProvider.hpp"
#include "Mesh/P1/Hexa3D.hpp"
#include "Mesh/P1/Quad3D.hpp"

//////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace Mesh {
namespace P1 {
  
////////////////////////////////////////////////////////////////////////////////

Common::ObjectProvider < Hexa3D,
                         ElementType,
                         P1Lib >
aP1Hexa3D_Provider ( "P1-"+Hexa3D::type_name() );

////////////////////////////////////////////////////////////////////////////////

Hexa3D::Hexa3D() 
{
  m_shape=GeoShape::HEXA;
  m_shapeName=GeoShape::Convert::to_str(m_shape);
  m_order=1;
  m_nbNodes=8;
  m_dimension=3;
  m_dimensionality=3;
  m_nbEdges=8;
  
  // set faces
  m_faces.reserve(6);

  boost::shared_ptr<ElementType> quad(new Quad3D);
  std::vector<Uint> nodes(4);

  nodes[0]=0;   nodes[1]=3;   nodes[2]=2;   nodes[3]=1;
  m_faces.push_back( Face(quad,nodes));

  nodes[0]=4;   nodes[1]=5;   nodes[2]=6;   nodes[3]=7;
  m_faces.push_back( Face(quad,nodes));

  nodes[0]=0;   nodes[1]=1;   nodes[2]=5;   nodes[3]=4;
  m_faces.push_back( Face(quad,nodes));

  nodes[0]=1;   nodes[1]=2;   nodes[2]=6;   nodes[3]=5;
  m_faces.push_back( Face(quad,nodes));

  nodes[0]=3;   nodes[1]=7;   nodes[2]=6;   nodes[3]=2;
  m_faces.push_back( Face(quad,nodes));

  nodes[0]=0;   nodes[1]=4;   nodes[2]=7;   nodes[3]=3;
  m_faces.push_back( Face(quad,nodes));


}

////////////////////////////////////////////////////////////////////////////////

} // P1
} // Mesh
} // CF
