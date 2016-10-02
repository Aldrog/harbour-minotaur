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

#include "mazeitem.h"
#include "mazeitemkiller.h"
#include <QDebug>

MazeItem::MazeItem(QObject *parent) :
	QObject(parent),
	_engine(NULL),
	_turn(false) {}

MazeItem::~MazeItem() {
	if(_engine)
		_engine->removeItem(this);
}

void MazeItem::setEngine(MazeEngine *engine) {
	if(engine != _engine) {
		if(_engine)
			_engine->removeItem(this);
		_engine = engine;
		engine->registerItem(this);
		emit engineChanged();
	}
}

void MazeItem::setLocation(QPoint location) {
	if(location != _location) {
		_location = location;
		emit locationChanged(this);
		if(engine() && !engine()->availableLocations.contains(location))
			emit outOfMaze();
	}
}

void MazeItem::setCurrentTurn(bool currentTurn) {
	if(currentTurn != _turn) {
		_turn = currentTurn;
		emit currentTurnChanged();
		if(_turn)
			emit turnStarted();
	}
}

void MazeItem::setDangerLevel(int dangerLevel) {
	if(dangerLevel != _danger) {
		_danger = dangerLevel;
		emit dangerChanged();
	}
}

bool MazeItem::move(direction dir) {
	if(_engine && movable) {
		if(currentTurn() && _engine->canGo(_location, dir)) {
			setLocation(_engine->move(_location, dir));
			emit turnEnded();
			return true;
		}
		emit turnEnded();
	}
	return false;
}

void MazeItem::randLocation() {
	qDebug() << "choosing random location out of" << _engine->availableLocations.count() << "variants";
	setLocation(_engine->availableLocations.at(rand() % _engine->availableLocations.count()));
}

void MazeItem::intersected(MazeItem *item) {
    if(this->killable && dynamic_cast<MazeItemKiller*>(item))
		emit wasKilled();
	if(this->pickable && item->picker)
		emit wasPicked();
}

bool MazeItem::searchPath(QList<QPoint> *path, QPoint target) {
    if(path->last() == target) {
        return true;
    }
    bool found = false;
    QList<QPoint> testPath, resultPath;
    direction d;
    testPath = *path;
    d = Up;
    if(_engine->canGo(path->last(), d) && !path->contains(_engine->move(path->last(), d))) {
        testPath.append(_engine->move(path->last(), d));
        if(searchPath(&testPath, target) && (!found || testPath.count() < path->count())) {
            resultPath = testPath;
            found = true;
        }
    }
    testPath = *path;
    d = Down;
    if(_engine->canGo(path->last(), d) && !path->contains(_engine->move(path->last(), d))) {
        testPath.append(_engine->move(path->last(), d));
        if(searchPath(&testPath, target) && (!found || testPath.count() < resultPath.count())) {
            resultPath = testPath;
            found = true;
        }
    }
    testPath = *path;
    d = Left;
    if(_engine->canGo(path->last(), d) && !path->contains(_engine->move(path->last(), d))) {
        testPath.append(_engine->move(path->last(), d));
        if(searchPath(&testPath, target) && (!found || testPath.count() < resultPath.count())) {
            resultPath = testPath;
            found = true;
        }
    }
    testPath = *path;
    d = Right;
    if(_engine->canGo(path->last(), d) && !path->contains(_engine->move(path->last(), d))) {
        testPath.append(_engine->move(path->last(), d));
        if(searchPath(&testPath, target) && (!found || testPath.count() < resultPath.count())) {
            resultPath = testPath;
            found = true;
        }
    }
    *path = resultPath;
    return found;
}
