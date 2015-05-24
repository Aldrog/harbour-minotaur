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
	_engine(NULL),
	_turn(false){}
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

bool MazeItem::move(direction dir) {
	if(_engine) {
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
	if(this->killable && item->killer)
		emit wasKilled();
	if(this->pickable && item->picker)
		emit wasPicked();
}
