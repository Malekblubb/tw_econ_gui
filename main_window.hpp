//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP


#include <twec/id_holder.hpp>
#include <twec/player_info.hpp>
#include <twec/server_manager.hpp>
#include <twec/utils.hpp>
#include "ui_main_window.h"

#include <QMainWindow>
#include <QMessageBox>


namespace Ui
{class main_window;}

class main_window : public QMainWindow
{
	Q_OBJECT

	Ui::main_window* m_ui;

	QTimer m_update_timer;

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
		connect(&m_servermgr, SIGNAL(playerinfo_received(const twec::player_infos&)), this, SLOT(_on_playerinfo_received(const twec::player_infos&)));
		connect(&m_servermgr, SIGNAL(job_ended(int)), this, SLOT(_on_job_ended(int)));

		connect(&m_update_timer, SIGNAL(timeout()), this, SLOT(update()));

		m_update_timer.start(0);
	}

	~main_window()
	{delete m_ui;}

private slots:
	void update()
	{
		// update server, connection, etc...
		m_servermgr.update();

		// update some ui stuff
		for(auto& a : m_servermgr.jobmgr().get_jobs())
		{
			// this code rly sucks, qt does not provide itering over cells
			// TODO: maybe impl celliterating one day
			for(auto row(0); row < m_ui->tw_jobs->rowCount(); ++row)
				for(auto col(0); col < m_ui->tw_jobs->rowCount(); ++col)
				{
					auto ptr(twec::to_id_holder(m_ui->tw_jobs->cellWidget(row, col)));
					if(ptr)
						if(ptr->get_id() == a.get_id())
							m_ui->tw_jobs->item(row, 1)->setText(twec::timestr_from_ms(a.get_countdown()).c_str());
				}
		}
	}

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
	{
		m_servermgr.request_player_info();
		m_ui->tw_players->clearContents();
		m_ui->tw_players->setRowCount(0);
	}

	void on_btn_ban_ip_clicked()
	{
		std::string banminutes{m_ui->le_ip_bantime->text().isEmpty() ? "5" : m_ui->le_ip_bantime->text().toStdString()};
		m_servermgr.exec_command("ban " + m_ui->le_ip_ban->text().toStdString() + " " + banminutes);
	}

	void on_btn_unban_ip_clicked()
	{m_servermgr.exec_command("unban " + m_ui->le_ip_ban->text().toStdString());}

	void on_btn_kick_selected_clicked()
	{
		for(auto& a : m_ui->tw_players->selectedItems())
		{
			// only id is interesting
			if(a->column() != 0)
				continue;
			m_servermgr.exec_command("kick " + a->text().toStdString());
		}
	}

	void on_btn_ban_selected_clicked()
	{
		std::string banminutes{m_ui->le_bantime->text().isEmpty() ? "5" : m_ui->le_bantime->text().toStdString()};
		for(auto& a : m_ui->tw_players->selectedItems())
		{
			// only id is interesting
			if(a->column() != 0)
				continue;
			m_servermgr.exec_command("ban " + a->text().toStdString() + " " + banminutes);
		}
	}

	// tab: jobs
	void on_pb_job_add_clicked()
	{
		auto row(m_ui->tw_jobs->rowCount());
		auto time(m_ui->te_job_time->time());
		auto time_ms(twec::ms_from_time(time.hour(), time.minute(), time.second()));
		m_servermgr.jobmgr().add_job(row, m_ui->le_job_command->text().toStdString(), time_ms, m_ui->cb_job_repeat->isChecked());

		auto* cmd_item = new QTableWidgetItem{m_ui->le_job_command->text()};
		auto* tm_item = new QTableWidgetItem;
		auto* repeat_item = new QTableWidgetItem{m_ui->cb_job_repeat->isChecked() ? "Yes" : "No"};
		m_ui->tw_jobs->insertRow(row);
		m_ui->tw_jobs->setItem(row, 0, cmd_item);
		m_ui->tw_jobs->setItem(row, 1, tm_item);
		m_ui->tw_jobs->setItem(row, 2, repeat_item);

		auto* h = new twec::id_holder{row};
		m_ui->tw_jobs->setCellWidget(row, 0, h);

		// reset the ui
		m_ui->le_job_command->clear();
		m_ui->te_job_time->setTime({0, 0});
		m_ui->cb_job_repeat->setChecked(false);
	}

	void on_pb_remove_job_clicked()
	{
		auto selected(m_ui->tw_jobs->selectedItems());
		for(auto iter(std::begin(selected)); iter != std::end(selected); ++iter)
		{
			auto obj(*iter);
			auto ptr(twec::to_id_holder(m_ui->tw_jobs->cellWidget(obj->row(), obj->column())));
			if(ptr == nullptr)
				continue;

			m_servermgr.jobmgr().remove_job(ptr->get_id());
			m_ui->tw_jobs->removeRow(obj->row());
			break; // should be single selection
		}

	}

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
		m_ui->tw_main->setTabText(0, "(no server)");
	}

	void _on_connection_start(const std::string& addr_str)
	{
		m_ui->tw_main->setTabText(0, addr_str.c_str());
		this->set_window_state("Connected");
	}

	void _on_login()
	{
		m_ui->tw_main->setTabText(0, m_ui->tw_main->tabText(0) + "(logged)");
		this->set_window_state("Logged in");
	}

	void _on_playerinfo_received(const twec::player_infos& infos)
	{
		auto table(m_ui->tw_players);
		table->clearContents();
		table->setRowCount(0);
		auto current_row(0);
		for(auto& a : infos)
		{
			table->insertRow(current_row);

			// some ugly code...
			auto* id_item = new QTableWidgetItem{a.id.c_str()};
			auto* addr_item = new QTableWidgetItem{a.addr.c_str()};
			auto* name_item = new QTableWidgetItem{a.name.c_str()};
			auto* score_item = new QTableWidgetItem{a.score.c_str()};
			table->setItem(current_row, 0, id_item);
			table->setItem(current_row, 1, name_item);
			table->setItem(current_row, 2, score_item);
			table->setItem(current_row, 3, addr_item);

			++current_row;
		}
	}

	void _on_job_ended(int id)
	{
		for(auto row(0); row < m_ui->tw_jobs->rowCount(); ++row)
			for(auto col(0); col < m_ui->tw_jobs->rowCount(); ++col)
			{
				auto ptr(twec::to_id_holder(m_ui->tw_jobs->cellWidget(row, col)));
				if(ptr)
					if(ptr->get_id() == id)
						m_ui->tw_jobs->removeRow(row);
			}
	}

private:
	void set_window_state(const std::string& state)
	{m_ui->lb_state->setText(state.c_str());}
};


#endif // MAIN_WINDOW_HPP
