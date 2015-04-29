#ifndef MAZEENGINE_H
#define MAZEENGINE_H

#include <QObject>
#include <QVector>
#include <QPoint>

enum direction { Up=-1, Down=1, Left=-2, Right=2 };
struct pass {
	QPoint s;
	QPoint e;
	direction d;
};

class MazeEngine : public QObject
{
	Q_OBJECT
public:
	explicit MazeEngine(QObject *parent = 0, int dimension = 5);
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
