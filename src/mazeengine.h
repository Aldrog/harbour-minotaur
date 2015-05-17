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
#include <QPoint>
#include "mazeitem.h"

class MazeItem;

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
	~MazeEngine();
	Q_INVOKABLE void generateRandom(int size);
	Q_INVOKABLE bool canGo(QPoint location, QString dir);
	Q_INVOKABLE inline bool canGo(int x, int y, QString dir) { return canGo(QPoint(x, y), dir); }
	void registerItem(MazeItem *item);
	void removeItem(MazeItem *item);

signals:
	void intersection(MazeItem *item1, MazeItem *item2);

public slots:
	void checkIntersection(MazeItem *item);

private:
	QList<pass> _passes;
	QList<MazeItem*> _items;
};

#endif // MAZEENGINE_H
