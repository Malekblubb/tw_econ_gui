//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWEC_PLAYER_INFO_HPP
#define TWEC_PLAYER_INFO_HPP


#include <string>
#include <vector>


namespace twec
{
	struct player_info
	{
		std::string id;
		std::string addr;
		std::string name;
		std::string score;
	};

	using player_infos = std::vector<player_info>;
}


#endif // TWEC_PLAYER_INFO_HPP
