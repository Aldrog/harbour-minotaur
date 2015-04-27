#ifndef TOOLS_H
#define TOOLS_H

#include <QObject>

class Tools : public QObject
{
	Q_OBJECT
public:
	explicit Tools(QObject *parent = 0);

public slots:
	void quit();
};

#endif // TOOLS_H
