#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP


#include <QObject>
#include <QTimer>


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




#endif // SERVER_MANAGER_HPP
