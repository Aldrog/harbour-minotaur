#ifndef MAZEENGINE_H
#define MAZEENGINE_H

#include <QObject>
#include <QVector>
#include <QPoint>

enum direction { Up, Down, Left, Right };
direction getOppositeDirection(direction d);

struct pass {
	QPoint s;
	QPoint e;
	direction d;
};
bool operator ==(pass a, pass b);

class MazeEngine : public QObject
{
	Q_OBJECT
public:
	explicit MazeEngine(QObject *parent = 0);
	Q_PROPERTY(int dimension READ dimension WRITE setDimension NOTIFY dimensionChanged)
	Q_INVOKABLE bool canGo(int x, int y, QString dir);

	int dimension();
	void setDimension(int dimension);
signals:
	void dimensionChanged();
public slots:

private:
	QList<pass> _passes;
	int _mazesize;
};

#endif // MAZEENGINE_H
