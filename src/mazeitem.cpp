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
#include <QDebug>

MazeItem::MazeItem(QObject *parent) :
	QObject(parent),
	_engine(NULL){}
MazeItem::~MazeItem() {
	if(_engine)
		_engine->removeItem(this);
}

MazeEngine *MazeItem::engine() {
	return _engine;
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

QPoint MazeItem::location() {
	return _location;
}

void MazeItem::setLocation(QPoint location) {
	if(location != _location) {
		_location = location;
		emit locationChanged(this);
	}
}

bool MazeItem::move(QString direction) {
	if(_engine) {
		if(_engine->canGo(_location, direction)) {
			if(direction.toLower() == "up") {
				setLocation(_location + QPoint(0, -1));
			}
			if(direction.toLower() == "down") {
				setLocation(_location + QPoint(0, 1));
			}
			if(direction.toLower() == "left") {
				setLocation(_location + QPoint(-1, 0));
			}
			if(direction.toLower() == "right") {
				setLocation(_location + QPoint(1, 0));
			}
			return true;
		}
	}
	return false;
}

void MazeItem::intersected(MazeItem *item) {
	if(this->killable && item->killing)
		emit wasKilled();
	if(this->pickable && item->picking)
		emit wasPicked();
}
