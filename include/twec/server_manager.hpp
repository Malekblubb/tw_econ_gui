//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWEC_SERVER_MANAGER_HPP
#define TWEC_SERVER_MANAGER_HPP


#include "twecon_client.hpp"

#include <QObject>
#include <QTimer>


namespace twec
{
	class server_manager : public QObject
	{
		Q_OBJECT

		QTimer m_update_timer;
		twecon_client m_econ_client;

	public:
		server_manager()
		{
			connect(&m_update_timer, SIGNAL(timeout()), this, SLOT(update()));

			m_econ_client.on_log_added =
			[this](const std::string& str){emit this->log_added(str);};

			m_econ_client.on_connection_timeout =
			[this]{this->connection_timeout();};

			m_econ_client.on_connection_lost =
			[this]{emit this->connection_lost();};

			m_econ_client.on_connection =
			[this]{emit this->connection_start(m_econ_client.get_address().to_string());};

			m_econ_client.on_login =
			[this]{emit this->logged_in();};

			m_econ_client.on_playerinfo_received =
			[this](const player_infos& pi){emit this->playerinfo_received(pi);};

			m_update_timer.start(0);
		}

		void open_econ(const mlk::ntw::ip_address& addr, const std::string& pass)
		{
			if(!m_econ_client.is_connected())
				m_econ_client.open_connection(addr);
			if(!m_econ_client.is_logged())
				m_econ_client.login(pass);
		}

		void exec_command(const std::string& cmd)
		{m_econ_client.exec_command(cmd);}

		void request_player_info()
		{m_econ_client.request_player_info();}

	signals:
		void log_added(const std::string&);
		void connection_timeout();
		void connection_lost();
		void connection_start(const std::string&);
		void logged_in();
		void playerinfo_received(const twec::player_infos&); // twec:: - QT needs that namspace, lel

	private slots:		
		void update()
		{
			m_econ_client.update();
		}
	};
}


#endif // TWEC_SERVER_MANAGER_HPP
