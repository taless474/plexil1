// Copyright (c) 2006-2021, Universities Space Research Association (USRA).
//  All rights reserved.
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

#include <string>

#include "plexil-stddef.h" // size_t; also includes plexil-config.h
#include "plexil-stdint.h" // int32_t; also includes plexil-config.h

namespace PLEXIL
{

  //! \brief Structure to contain parameters for wait_for_input(). 
  //! \ingroup interface-library
  struct udp_thread_params
  {
    unsigned char* buffer; //!< Pointer to a receive buffer.
    size_t size;           //!< Maximum number of bytes to receive.
    int local_port;        //!< Local port number to bind the socket to.
    int sock;              //!< The file number of an open socket.
    bool debug;            //!< true to print debugging messages.
  };

  //! \brief Encode a 32 bit integer (in network byte order) into a buffer.
  //! \param num The integer to encode.
  //! \param buffer Pointer to the buffer.
  //! \param start_index Starting offset into the buffer.
  //! \ingroup interface-library
  void encode_int32_t(int32_t num, unsigned char* buffer, size_t start_index);

  //! \brief Encode a 16 bit integer (in network byte order) into a buffer.
  //! \param num The integer to encode.
  //! \param buffer Pointer to the buffer.
  //! \param start_index Starting offset into the buffer.
  //! \ingroup interface-library
  void encode_short_int(short int num, unsigned char* buffer, size_t start_index);

  //! \brief Decode a 32 bit integer from the network bytes in host byte order.
  //! \param buffer Pointer to the buffer.
  //! \param start_index Starting offset into the buffer.
  //! \return The integer.
  //! \ingroup interface-library
  int32_t decode_int32_t(const unsigned char* buffer, size_t start_index);

  //! \brief Decode a 16 bit integer from the network bytes in host byte order.
  //! \param buffer Pointer to the buffer.
  //! \param start_index Starting offset into the buffer.
  //! \return The integer.
  //! \ingroup interface-library
  short int decode_short_int(const unsigned char* buffer, size_t start_index);

  //! \brief Encode a 32 bit float in network byte order into a buffer.
  //! \param num The float to encode.
  //! \param buffer Pointer to the buffer.
  //! \param start_index Starting offset into the buffer.
  //! \ingroup interface-library
  void encode_float(float num, unsigned char* buffer, size_t start_index);

  //! \brief Decode a 32 bit float from network byte order.
  //! \param buffer Pointer to the buffer.
  //! \param start_index Starting offset into the buffer.
  //! \return The float.
  //! \ingroup interface-library
  float decode_float(const unsigned char* buffer, size_t start_index);

  //! \brief Copy the text of a std::string into a buffer.
  //! \param str The string to encode.
  //! \param buffer Pointer to the buffer.
  //! \param start_index Starting offset into the buffer.
  //! \note This DOES NOT encode a c string.  You can do that on your own.
  //! \ingroup interface-library
  void encode_string(const std::string& str, unsigned char* buffer, size_t start_index);

  //! \brief Create a std::string from contents of a buffer.
  //! \param buffer Pointer to the buffer.
  //! \param start_index Starting offset into the buffer.
  //! \param length The maximum number of bytes to copy.
  //! \note This decoder stops at \0 or length, which ever comes first.  The \0 is never included.
  //! \ingroup interface-library
  std::string decode_string(const unsigned char* buffer, size_t start_index, int length);

  //! \brief Print the contents of a buffer for debugging.
  //! \param buffer Pointer to the buffer.
  //! \param bytes Number of bytes to display
  //! \param fancy If true, attempts to print bytes which could be characters as characters;
  //!              if false, prints all as numbers.  Default is false.
  //! \ingroup interface-library
  void print_buffer(const unsigned char* buffer, size_t bytes, bool fancy=false);

  //! \brief Use connect() to establish a UDP socket, and send the given message on it.
  //! \param peer_host Name or IP address of the host to contact,
  //! \param peer_port The UDP port number to contact on the host.
  //! \param buffer The message buffer.
  //! \param size The number of bytes to send from the buffer.
  //! \param debug true to print a message upon successful transmission.  Default is false. 
  //! \ingroup interface-library
  int send_message_connect(const char* peer_host, int peer_port, const char* buffer, size_t size, bool debug=false);

  //! \brief Open a UDP socket and bind it to the given local port.
  //!        Send a message to the given host and port over the socket.
  //!        Close the socket when finished.
  //! \param local_port The local port number.
  //! \param peer_host Name or IP address of the host to contact,
  //! \param peer_port The UDP port number to contact on the host.
  //! \param buffer The message buffer.
  //! \param size The number of bytes to send from the buffer.
  //! \param debug true to print debug messages. Default is false. 
  //! \ingroup interface-library
  int send_message_bind(int local_port, const char* peer_host, int peer_port, const char* buffer, size_t size, bool debug=false);

  //! \brief Call wait_for_input() with the parameters in the given struct.
  //! \param params Pointer to a udp_thread_params struct.
  //! \ingroup interface-library
  int wait_for_input_on_thread(udp_thread_params* params);

  //! \brief Bind a socket to the given local port. Wait indefinitely
  //!        for a datagram. Receive at most size bytes into the given
  //!        buffer.  Close the socket if successful.
  //! \param local_port The local port number.
  //! \param buffer The message buffer.
  //! \param size The masimum number of bytes to receive into the buffer.
  //! \param sock The file number of an open UDP socket.
  //! \param debug true to print debug messages. Default is false.
  //! \return 0 if successful, -1 on error.
  //! \ingroup interface-library
  int wait_for_input(int local_port, unsigned char* buffer, size_t size, int sock, bool debug=false);
}

// EOF
