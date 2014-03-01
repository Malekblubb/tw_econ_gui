//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWEC_JOB_HPP
#define TWEC_JOB_HPP


#include <mlk/signals_slots/slot.h>
#include <mlk/time/simple_timer.h>

#include <string>


namespace twec
{
	class job
	{
		std::string m_cmd;
		mlk::tm::simple_timer m_exec_timer;

		mlk::ullong m_interval;
		mlk::ullong m_end_in{0};

		int m_id{-1};
		bool m_repeat, m_ended{false};

	public:
		mlk::slot<const std::string&> on_exec;

		job(int id, const std::string& cmd, mlk::ullong exec_in, bool repeat) :
			m_cmd{cmd},
			m_exec_timer{exec_in},
			m_interval{exec_in},
			m_id{id},
			m_repeat{repeat}
		{
			m_exec_timer.run();
			this->calc_end();
		}

		void update()
		{
			if(m_ended)
				return;

			if(m_exec_timer.timed_out())
			{
				on_exec(m_cmd);
				if(m_repeat)
				{
					m_exec_timer.restart();
					this->calc_end();
				}
				else m_ended = true;
			}
		}

		bool is_ended() const noexcept
		{return m_ended;}

		int get_id() const noexcept
		{return m_id;}

		mlk::ullong get_countdown() const noexcept
		{
			auto cd(m_end_in - mlk::tm::time_stmp<mlk::milli_s>());
			return cd > m_interval ? 0 : cd;
		}

	private:
		void calc_end() noexcept
		{m_end_in = mlk::tm::time_stmp<mlk::milli_s>() + m_interval;}
	};
}


#endif // TWEC_JOB_HPP
