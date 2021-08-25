// Copyright (c) 2006-2021, Universities Space Research Association (USRA).
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Universities Space Research Association nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef LUV_LISTENER_HH
#define LUV_LISTENER_HH

#include "plexil-stdint.h"

#include "ConstantMacros.hh"
#include "ExecListener.hh"

// forward references outside of namespace

class Socket;

namespace PLEXIL 
{

  //! \class LuvListener
  //! \brief An ExecListener for use with the Plexil Viewer (nee' LUV).
  class LuvListener : public ExecListener
  {
  public:
    //
    // Public class constants
    //

    // Defaults
    DECLARE_STATIC_CLASS_CONST(char*, LUV_DEFAULT_HOSTNAME, "localhost");
    DECLARE_STATIC_CLASS_CONST(unsigned int, LUV_DEFAULT_PORT, 49100);

    // Configuration XML
    DECLARE_STATIC_CLASS_CONST(char*, LUV_HOSTNAME_ATTR, "HostName");
    DECLARE_STATIC_CLASS_CONST(char*, LUV_PORT_ATTR, "Port");
    DECLARE_STATIC_CLASS_CONST(char*, LUV_BLOCKING_ATTR, "Blocking");
    DECLARE_STATIC_CLASS_CONST(char*, IGNORE_CONNECT_FAILURE_ATTR, "IgnoreConnectFailure");

    // Literal strings (yes, this is redundant with LuvFormat.hh)
    DECLARE_STATIC_CLASS_CONST(char*, TRUE_STR, "true");
    DECLARE_STATIC_CLASS_CONST(char*, FALSE_STR, "false");

    // End-of-message marker
    DECLARE_STATIC_CLASS_CONST(char, LUV_END_OF_MESSAGE, (char)4);

    //! \brief Constructor from configuration XML.
    //! \param xml Const handle to the XML.
    LuvListener(pugi::xml_node const xml);

	//! \brief Alternative constructor.
    //! \param host Name of the host on which the Viewer is running.
    //! \param port The port number on which the Viewer is listening.
    //! \param block If true, will block Exec upon publishing node transitions
    //!              until the user has consented to step or continue.
    //!              Default is false.
    //! \param ignoreConnectionFailure If true, failure to connect to the
    //!                                given host and port is ignored.
    //!                                Default is true.
    //! \note Only called from the TestExec application.
	LuvListener(const std::string& host, 
				const uint16_t port, 
				const bool block = false,
				const bool ignoreConnectionFailure = true);

	//! Virtual destructor.
	virtual ~LuvListener();

    //! \brief Notify that a node has changed state.
    //! \param prevState The old state.
    //! \param newState The new state.
    //! \param node The node that has transitioned.
    virtual void implementNotifyNodeTransition(NodeState prevState, 
                                               NodeState newState, 
                                               Node *node) const;

    //! \brief Notify that a plan has been received by the Exec.
    //! \param plan The XML representation of the plan.
    virtual void implementNotifyAddPlan(pugi::xml_node const plan) const;

    //! \brief Notify that a library node has been received by the Exec.
    //! \param libNode The XML representation of the plan.
    virtual void implementNotifyAddLibrary(pugi::xml_node const libNode) const;

    //! \brief Notify that a variable assignment has been performed.
    //! \param dest The Expression being assigned to.
    //! \param destName A string naming the destination.
    //! \param value The value (in internal Exec representation) being assigned.
    virtual void implementNotifyAssignment(Expression const *dest,
                                           const std::string& destName,
                                           const Value& value) const;

    //! \brief Perform listener-specific initialization.
    //! \return true if successful, false otherwise.
    virtual bool initialize();

    //! \brief Perform listener-specific startup.
    //! \return true if successful, false otherwise.
    virtual bool start();

    //! \brief Perform listener-specific actions to stop.
    //! \return true if successful, false otherwise.
    virtual bool stop();

    //! \brief Perform listener-specific actions to reset to initialized state.
    //! \return true if successful, false otherwise.
    virtual bool reset();

    //! \brief Perform listener-specific actions to shut down.
    //! \return true if successful, false otherwise.
    bool shutdown();

	//! \brief Report whether the listener is connected to the viewer.
    //! \return True if connected, false if not.
	bool isConnected();

    //! \brief Construct the configuration XML element for the desired settings.
    //! \param block true if the Exec should block until the user steps forward, false otherwise.
    //! \param hostname The host name where the Luv instance is running.
    //! \param port The port number for the Luv instance.
    //! \return Pointer to a freshly created XML document containing the configuration element.
    static pugi::xml_document* constructConfigurationXml(const bool& block = false,
														 const char* hostname = LUV_DEFAULT_HOSTNAME(),
														 const unsigned int port = LUV_DEFAULT_PORT());

  private:

	// deliberately unimplemented
	LuvListener();
	LuvListener(const LuvListener&);
	LuvListener& operator=(const LuvListener&);

	//! \brief Open the socket connection to the viewer.
	//! \param port The IP port to which we are connecting.
	//! \param host The hostname to which we are connecting.
	//! \param ignoreFailure If true, failure is silently ignored.
	//! \return False if the connection fails and ignoreFailure is false, true otherwise.
	bool openSocket(uint16_t port, 
					const char* host, 
					bool ignoreFailure);

	//! \brief Close the socket.
	void closeSocket();

	//! \brief Send a plan info header to the viewer.
	void sendPlanInfo() const;

	//! \brief Send the message to the viewer.
    //! \param msg Const reference to the message.
	void sendMessage(const std::string& msg) const;

	//! \brief Block until acknowledgement from the viewer has been received.
	void waitForAck() const;

	//
	// Member variables
	//

    Socket* m_socket;
	const char* m_host;
	uint16_t m_port;
    bool m_block;
    bool m_ignoreConnectFailure;
  };

}

//! \brief Register the LuvListener factory with the PLEXIL Application Framework.
//! \ingroup interface-library
extern "C"
void initLuvListener();

#endif // LUV_LISTENER_HH
