//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP


#include <twec/server_manager.hpp>
#include "ui_main_window.h"

#include <QMainWindow>
#include <QMessageBox>


namespace Ui
{class main_window;}

class main_window : public QMainWindow
{
	Q_OBJECT

	Ui::main_window* m_ui;

	twec::server_manager m_servermgr;

public:
	main_window(QWidget* parent = nullptr) :
		QMainWindow{parent},
		m_ui{new Ui::main_window}
	{
		m_ui->setupUi(this);

		connect(&m_servermgr, SIGNAL(log_added(const std::string&)), this, SLOT(_on_log_added(const std::string&)));
		connect(&m_servermgr, SIGNAL(connection_timeout()), this, SLOT(_on_connection_timeout()));
		connect(&m_servermgr, SIGNAL(connection_lost()), this, SLOT(_on_connection_lost()));
		connect(&m_servermgr, SIGNAL(connection_start(const std::string&)), this, SLOT(_on_connection_start(const std::string&)));
		connect(&m_servermgr, SIGNAL(logged_in()), this, SLOT(_on_login()));
	}

	~main_window()
	{delete m_ui;}

private slots:
	void on_btn_login_clicked()
	{
		m_servermgr.open_econ({m_ui->le_host->text().toStdString(), m_ui->sb_port->text().toStdString()}, m_ui->le_password->text().toStdString());
		this->set_window_state("Trying to connect...");
	}

	// tab: main controlls
	void on_btn_change_map_clicked()
	{m_servermgr.exec_command("change_map " + m_ui->le_mapname->text().toStdString());}

	void on_btn_reload_map_clicked()
	{m_servermgr.exec_command("reload");}

	void on_btn_start_recording_clicked()
	{m_servermgr.exec_command("record " + m_ui->le_record_filename->text().toStdString());}

	void on_btn_stop_recording_clicked()
	{m_servermgr.exec_command("stoprecord");}

	void on_btn_set_broadcast_clicked()
	{m_servermgr.exec_command("broadcast " + m_ui->le_broadcast->text().toStdString());}

	void on_btn_restart_round_clicked()
	{m_servermgr.exec_command("restart " + m_ui->le_restart_time->text().toStdString());}

	void on_btn_vote_yes_clicked()
	{m_servermgr.exec_command("vote yes");}

	void on_btn_vote_no_clicked()
	{m_servermgr.exec_command("vote no");}

	void on_btn_shutdown_clicked()
	{m_servermgr.exec_command("shutdown");}

	void on_btn_text_send_clicked()
	{m_servermgr.exec_command("say " + m_ui->le_echo_text->text().toStdString());}

	void on_btn_custom_cmd_send_clicked()
	{m_servermgr.exec_command(m_ui->le_custom_cmd->text().toStdString());}


	// tab: players
	void on_btn_refresh_playerlist_clicked()
	{m_servermgr.request_player_info();}

	void on_btn_ban_ip_clicked()
	{m_servermgr.exec_command("ban " + m_ui->le_ip_ban->text().toStdString() + " " + m_ui->le_ip_bantime->text().toStdString());}

	void on_btn_unban_ip_clicked()
	{m_servermgr.exec_command("unban " + m_ui->le_ip_ban->text().toStdString());}

	void _on_log_added(const std::string& str)
	{
		m_ui->te_log->moveCursor(QTextCursor::End);
		m_ui->te_log->insertPlainText(str.c_str());
		m_ui->te_log->moveCursor(QTextCursor::End);
	}

	void _on_connection_timeout()
	{
		QMessageBox::critical(this, "Error", "Connection attempt timed out");
		this->set_window_state("Connection failed");
	}

	void _on_connection_lost()
	{
		QMessageBox::critical(this, "Error", "Connection to server lost");
		this->set_window_state("Connection lost");
	}

	void _on_connection_start(const std::string& addr_str)
	{
		m_ui->tw_main->setTabText(0, addr_str.c_str());
		this->set_window_state("Connected");
	}

	void _on_login()
	{
		m_ui->tw_main->setTabText(0, m_ui->tw_main->tabText(0) + "(logged)");
	}

private:
	void set_window_state(const std::string& state)
	{m_ui->lb_state->setText(state.c_str());}
};


#endif // MAIN_WINDOW_HPP
