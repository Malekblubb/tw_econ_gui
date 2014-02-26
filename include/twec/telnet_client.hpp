//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWEC_TELNET_CLIENT_HPP
#define TWEC_TELNET_CLIENT_HPP


#include <mlk/network/network.h>


namespace twec
{
	class telnet_client
	{
		mlk::ntw::sock<mlk::ntw::sock_type::tcp, false> m_socket;

	public:
		telnet_client() = default;

		bool connect(const mlk::ntw::ip_address& addr)
		{return m_socket.connect(addr);}

		void disconnect()
		{m_socket.reset_socket();}

		ssize_t send_line(const mlk::data_packet& data) const
		{
			auto cpy(data);
			mlk::cnt::append(mlk::data_packet{0x0D, 0x0A}, cpy);
			return m_socket.send(cpy);
		}

		ssize_t recv_line(mlk::data_packet& result, std::size_t max_len) const
		{return m_socket.recv(result, max_len);}
	};
}


#endif // TWEC_TELNET_CLIENT_HPP
