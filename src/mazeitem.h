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
#include <QList>
#include <QHash>
#include "mazeengine.h"
#include "directionenum.h"

class MazeEngine;

class MazeItem : public QObject
{
	Q_OBJECT
protected:
	MazeEngine *_engine;
	QPoint _location;
	bool _turn;
	int _danger;

public:
	explicit MazeItem(QObject *parent = 0);
	~MazeItem();

	// Basic item properties
	bool killable;
	bool killer;
	bool pickable;
	bool picker;
	bool movable;

	Q_PROPERTY(MazeEngine *engine READ engine WRITE setEngine NOTIFY engineChanged)
	Q_PROPERTY(QPoint location READ location WRITE setLocation NOTIFY locationChanged)
	// Used by movable items. One and only one movable item must have currentTurn set to true
	Q_PROPERTY(bool currentTurn READ currentTurn WRITE setCurrentTurn NOTIFY currentTurnChanged)
	// Used by killable items
	Q_PROPERTY(int dangerLevel READ dangerLevel NOTIFY dangerChanged)

	inline MazeEngine *engine()	{ return _engine; }
	void setEngine(MazeEngine *engine);
	inline QPoint location()	{ return _location; }
	void setLocation(QPoint location);
	inline bool currentTurn()	{ return _turn; }
	void setCurrentTurn(bool currentTurn);
	inline int dangerLevel()	{ return _danger; }
	void setDangerLevel(int dangerLevel);

	// Used by killers
	QList<MazeItem*> targets;
	QHash<MazeItem*, QList<QPoint> > paths;

	bool move(direction dir);
	Q_INVOKABLE inline bool move(QString dir) { return move(directionFromString(dir)); }
	Q_INVOKABLE void randLocation();

signals:
	void engineChanged();
	void locationChanged(MazeItem *item);
	void currentTurnChanged();
	void dangerChanged();
	void wasKilled();
	void wasPicked();
	void outOfMaze();
	void turnEnded();
	void turnStarted();

public slots:
	virtual void intersected(MazeItem *item);
	void findPaths();

private:
	bool searchPath(QList<QPoint> *path, QPoint target);
};

#endif // MAZEITEM_H
