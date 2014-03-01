//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWEC_ID_HOLDER_HPP
#define TWEC_ID_HOLDER_HPP


#include <QWidget>


namespace twec
{
	class id_holder : public QWidget
	{
		int m_id;

	public:
		id_holder(int id) :
			QWidget{nullptr},
			m_id{id}
		{ }

		int get_id() const noexcept
		{return m_id;}
	};

	template<typename Ptr_Type>
	id_holder* to_id_holder(Ptr_Type* ptr)
	{return static_cast<id_holder*>(ptr);}
}


#endif // TWEC_ID_HOLDER_HPP
