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

#ifndef MAZEITEM_H
#define MAZEITEM_H

#include <QObject>
#include <QPoint>
#include "mazeengine.h"

class MazeEngine;

class MazeItem : public QObject
{
	Q_OBJECT
public:
	explicit MazeItem(QObject *parent = 0);
	Q_PROPERTY(MazeEngine *engine READ engine WRITE setEngine NOTIFY engineChanged)
	Q_PROPERTY(QPoint location READ location WRITE setLocation NOTIFY locationChanged)

	Q_INVOKABLE bool move(QString direction);
	MazeEngine *engine();
	void setEngine(MazeEngine *engine);
	QPoint location();
	void setLocation(QPoint location);
	bool killable;
	bool killing;
	bool pickable;
	bool picking;

signals:
	void engineChanged();
	void locationChanged(MazeItem *item);
	void wasKilled();
	void wasPicked();


public slots:
	void intersected(MazeItem *item);

private:
	MazeEngine *_engine;
	QPoint _location;
};

#endif // MAZEITEM_H
