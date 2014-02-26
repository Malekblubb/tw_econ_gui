//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWEC_TWECON_CLIENT_HPP
#define TWEC_TWECON_CLIENT_HPP


#include "data_parser.hpp"
#include "telnet_client.hpp"

#include <mlk/time/simple_timer.h>


namespace twec
{
	class twecon_client
	{
		static constexpr std::size_t m_maxrecv{2048};
		static constexpr mlk::ullong m_timeout{4000};
		telnet_client m_telnet;

		std::stringstream m_log;
		mlk::data_packet m_recvdata;

		mlk::ntw::ip_address m_next_addr;
		std::string m_next_passwd;

		mlk::tm::simple_timer m_conntimeout_timer{m_timeout};

		bool m_valid{false}, m_connected{false}, m_logged{false};
		bool m_need_connect{false}, m_need_send_pass{false}, m_need_send_connection_lost{true};
		bool m_playerinfo_requested{false};

	public:
		mlk::slot<const std::string&> on_log_added;
		mlk::slot<> on_connection_timeout;
		mlk::slot<> on_connection_lost;
		mlk::slot<> on_connection;
		mlk::slot<> on_login;
		mlk::slot<const player_infos&> on_playerinfo_received;

		twecon_client() :
			m_recvdata(m_maxrecv)
		{ }

		void update()
		{
			// connect when needed
			if(m_need_connect)
			{				
				if(!m_connected)
				{
					if(m_conntimeout_timer.timed_out())
					{
						m_need_connect = false;
						on_connection_timeout();
					}
					else this->try_connect();
				}

				else this->on_internal_connection();
			}

			// send passwd when needed
			if(m_need_send_pass && !m_need_connect)
			{
				this->exec_command(m_next_passwd);
				m_need_send_pass = false;
			}

			m_recvdata.clear();
			auto len(m_telnet.recv_line(m_recvdata, m_maxrecv));
			if(m_connected)
			{
				if(len == 0)
				{
					m_valid = false;
					if(m_need_send_connection_lost)
					{
						on_connection_lost();
						m_need_send_connection_lost = false;
						this->on_internal_connection_lost();
					}
					return;
				}
				else
				{
					m_valid = true;
					m_need_send_connection_lost = true;
				}
			}
			if(len > 0)
			{
				auto str(this->make_string(m_recvdata, len));
				data_parser dp{str};

				if(!m_logged && dp.is_login_reply())
					this->on_internal_login();
				if(m_playerinfo_requested && dp.is_player_reply())
				{
					on_playerinfo_received(dp.get_player_info());
					m_playerinfo_requested = false;
				}

				this->add_to_log(str);
			}
		}

		void open_connection(const mlk::ntw::ip_address& addr)
		{
			if(m_connected)
				m_telnet.disconnect();
			m_next_addr = addr;
			m_need_connect = true;
			m_connected = false;
			m_logged = false;
			m_valid = false;
			m_conntimeout_timer.run();
		}

		void login(const std::string& pass)
		{
			m_need_send_pass = true;
			m_next_passwd = pass;
		}

		void request_player_info()
		{
			m_playerinfo_requested = true;
			this->exec_command("status");
		}

		ssize_t exec_command(const std::string& command) const
		{
			if(!this->is_valid())
				return 0;

			mlk::data_packet send_data{command.begin(), command.end()};
			return m_telnet.send_line(send_data);
		}

		bool is_connected() const noexcept
		{return m_connected;}

		bool is_valid() const noexcept
		{return m_valid;}

		bool is_logged() const noexcept
		{return m_logged;}

		const mlk::ntw::ip_address& get_address() const noexcept
		{return m_next_addr;}

		std::string get_log() const noexcept
		{return m_log.str();}

	private:
		void on_internal_connection()
		{
			m_connected = true;
			m_need_connect = false;
			on_connection();
		}

		void on_internal_login()
		{
			m_logged = true;
			on_login();
		}

		void on_internal_connection_lost() noexcept
		{
			m_connected = false;
			m_logged = false;
			m_valid = false;
			m_need_connect = false;
			m_need_send_pass = false;
			m_need_send_connection_lost = false;
			m_playerinfo_requested = false;
			m_telnet.disconnect();
		}

		void try_connect()
		{m_connected = m_telnet.connect(m_next_addr);}

		std::string make_string(const mlk::data_packet& data, std::size_t len)
		{
			std::string result{reinterpret_cast<const char*>(data.data()), len};
			mlk::stl_string::erase_all(0x00, result);
			return result;
		}

		void add_to_log(const std::string& str)
		{
			on_log_added(str);
			m_log << str;
		}
	};
}


#endif // TWEC_TWECON_CLIENT_HPP
