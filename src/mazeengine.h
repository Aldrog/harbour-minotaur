/*
 * Copyright © 2015 Andrew Penkrat
 *
 * This file is part of Minotaur.
 *
 * Minotaur is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Minotaur is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Minotaur.  If not, see <http://www.gnu.org/licenses/>.
 */

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
