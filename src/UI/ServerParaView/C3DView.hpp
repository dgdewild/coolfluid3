#ifndef CF_UI_ServerParaView_C3DVIEW_HPP
#define CF_UI_ServerParaView_C3DVIEW_HPP

#include <QString>
#include <QProcess>

#include "Mesh/CTable.hpp"

//////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace UI {
namespace ServerParaView {

//////////////////////////////////////////////////////////////////////////////

  class /*ServerParaView_API*/ C3DView :
    public Common::Component
{
public: // typedefs

  typedef boost::shared_ptr<C3DView> Ptr;
  typedef boost::shared_ptr<C3DView const> ConstPtr;

public:
    C3DView(const std::string& name);

    void setPort(QString port);

    virtual ~C3DView();

    /// Get the class name
    static std::string type_name () { return "C3DView"; }

    //void launch_pvserver(QString port,QString machinesFilePath,QString numberOfProcess);

    void launch_pvserver( Common::SignalArgs & args );

    void dump_file( Common::SignalArgs & args );

    void send_server_info_to_client( Common::SignalArgs & args );

    //void file_dumped( Common::SignalArgs & args );

  private : //data
        QProcess * pvserver;
        QString m_port;
};

//////////////////////////////////////////////////////////////////////////////

} // ServerParaView
} // UI
} // CF

//////////////////////////////////////////////////////////////////////////////

#endif // CF_UI_ServerParaView_C3DVIEW_HPP
