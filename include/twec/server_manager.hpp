//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWEC_SERVER_MANAGER_HPP
#define TWEC_SERVER_MANAGER_HPP


#include "job_manager.hpp"
#include "twecon_client.hpp"

#include <mlk/network/network.h>

#include <QObject>
#include <QTimer>


namespace twec
{
	class server_manager : public QObject
	{
		Q_OBJECT

		twecon_client m_econ_client;
		job_manager m_jobmanager{m_econ_client};

	public:
		server_manager()
		{
			m_econ_client.on_log_added =
			[this](const std::string& str){emit this->log_added(str);};

			m_econ_client.on_connection_timeout =
			[this]{emit this->connection_timeout();};

			m_econ_client.on_connection_lost =
			[this]{emit this->connection_lost();};

			m_econ_client.on_connection =
			[this]{emit this->connection_start(m_econ_client.get_address().to_string());};

			m_econ_client.on_login =
			[this]{emit this->logged_in();};

			m_econ_client.on_playerinfo_received =
			[this](const player_infos& pi){emit this->playerinfo_received(pi);};

			m_jobmanager.on_job_ended =
			[this](int id){emit this->job_ended(id);};
		}

		void update()
		{
			m_econ_client.update();
			m_jobmanager.update();
		}

		bool open_econ(const mlk::ntw::ip_address& addr, const std::string& pass)
		{
			auto is_same_addr(addr == m_econ_client.get_address());
			if(!m_econ_client.is_connected() || !is_same_addr)
				m_econ_client.open_connection(addr);
			if(!m_econ_client.is_logged() || !is_same_addr)
				m_econ_client.login(pass);
			return !m_econ_client.is_connected() || !m_econ_client.is_logged() || !is_same_addr;
		}

		void exec_command(const std::string& cmd)
		{m_econ_client.exec_command(cmd);}

		void request_player_info()
		{m_econ_client.request_player_info();}

		twecon_client& econclient() noexcept
		{return m_econ_client;}

		job_manager& jobmgr() noexcept
		{return m_jobmanager;}

	signals:
		void log_added(const std::string&);
		void connection_timeout();
		void connection_lost();
		void connection_start(const std::string&);
		void logged_in();
		void playerinfo_received(const twec::player_infos&); // twec:: - QT needs that namspace, lel
		void job_ended(int);
	};
}


#endif // TWEC_SERVER_MANAGER_HPP
