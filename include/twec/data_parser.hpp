//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWEC_DATA_PARSER_HPP
#define TWEC_DATA_PARSER_HPP


#include "player_info.hpp"

#include <mlk/tools/stl_string_utl.h>

#include <string>


namespace twec
{
	class data_parser
	{
		const std::string& m_inputstr;

	public:
		data_parser(const std::string& inputstr) :
			m_inputstr{inputstr}
		{ }

		bool is_login_reply() const
		{return mlk::stl_string::contains("Authentication successful. External console access granted.", m_inputstr);}

		// TODO: use std::regex when its impled right
		bool is_player_reply() const
		{
			return mlk::stl_string::contains("id=", m_inputstr) && mlk::stl_string::contains("addr=", m_inputstr) &&
				   mlk::stl_string::contains("name=", m_inputstr) && mlk::stl_string::contains("id=", m_inputstr) && !mlk::stl_string::contains("cid", m_inputstr);
		}

		// hackish parser
		player_infos get_player_info() const
		{
			auto cpy(m_inputstr);
			auto num_players(mlk::stl_string::count_of('\n', cpy));
			player_infos result;

			for(std::size_t i{0}; i < num_players; ++i)
			{
				auto id_pos(cpy.find("id="));
				auto addr_pos(cpy.find("addr="));
				auto name_pos(cpy.find("name="));
				auto score_pos(cpy.find("score="));
				auto linebreak(cpy.find('\n'));

				result.emplace_back(
				player_info
				{
					cpy.substr(id_pos + 3, addr_pos - id_pos - 4),
					cpy.substr(addr_pos + 5, name_pos - addr_pos - 6),
					cpy.substr(name_pos + 6, score_pos - name_pos - 8),
					cpy.substr(score_pos + 6, linebreak - score_pos - 8)
				});

				cpy.erase(0, linebreak + 1);
			}
			return result;
		}
	};
}


#endif // TWEC_DATA_PARSER_HPP
