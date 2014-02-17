#ifndef TWEC_SERVER_MANAGER_HPP
#define TWEC_SERVER_MANAGER_HPP


#include <QObject>
#include <QTimer>


namespace twec
{
	class server_manager : public QObject
	{
		Q_OBJECT

		QTimer m_update_timer;

	public:
		server_manager()
		{
			connect(&m_update_timer, SIGNAL(timeout()), this, SLOT(update()));
			m_update_timer.start(0);
		}

	private slots:
		void update()
		{

		}
	};
}


#endif // TWEC_SERVER_MANAGER_HPP
