//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWEC_JOB_MANAGER_HPP
#define TWEC_JOB_MANAGER_HPP


#include "job.hpp"
#include "twecon_client.hpp"

#include <mlk/containers/container_utl.h>


namespace twec
{
	class job_manager
	{
		twecon_client& m_econclient;

		std::vector<job> m_jobs;

	public:
		mlk::slot<int> on_job_ended;

		job_manager(twecon_client& ec) :
			m_econclient{ec}
		{ }

		template<typename... Args>
		void add_job(Args&&... args)
		{
			m_jobs.emplace_back(std::forward<Args>(args)...);
			m_jobs.back().on_exec = [this](const std::string& cmd){this->exec_job(cmd);};
		}

		void update()
		{
			for(auto& a : m_jobs)
				a.update();
			this->remove_ended();
		}

		void remove_job(int id)
		{mlk::cnt::remove_all_if([id](const job& j){return j.get_id() == id;}, m_jobs);}

		auto get_jobs() const noexcept
		-> const decltype(m_jobs)&
		{return m_jobs;}

	private:
		void remove_ended()
		{
			for(auto& a : m_jobs)
				if(a.is_ended())
					on_job_ended(a.get_id());
			mlk::cnt::remove_all_if([](const job& j){return j.is_ended();}, m_jobs);
		}

		void exec_job(const std::string& cmd)
		{m_econclient.exec_command(cmd);}
	};
}



#endif // TWEC_JOB_MANAGER_HPP
