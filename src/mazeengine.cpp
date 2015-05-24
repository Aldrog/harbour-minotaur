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

#include "mazeengine.h"
#include "mazeitemexit.h"
#include <QDebug>

bool operator ==(pass a, pass b) {
	return ( a.s == b.s && a.e == b.e && a.d == b.d );
}

MazeEngine::MazeEngine(QObject *parent) :
	QObject(parent) {
}
MazeEngine::~MazeEngine() {
	foreach (MazeItem *i, _items) {
		delete i;
	}
}

void MazeEngine::generateRandom(int size) {
	pass p;
	// Fill maze with passes but leave walls on the sides.
	int exitNumber = rand() % (size * 4);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			p.s = QPoint(i, j);
			availableLocations.append(p.s);

			if(j > 0 || i == exitNumber) {
				p.d = Up;
				p.e = QPoint(i, j - 1);
				_passes.append(p);
				if(j == 0) {
					MazeItemExit *ex = new MazeItemExit();
					ex->setEngine(this);
					ex->setLocation(p.e);
				}
			}

			if(j < size - 1 || size + i == exitNumber) {
				p.d = Down;
				p.e = QPoint(i, j + 1);
				_passes.append(p);
				if(j == size - 1) {
					MazeItemExit *ex = new MazeItemExit();
					ex->setEngine(this);
					ex->setLocation(p.e);
				}
			}

			if(i > 0 || 2*size + j == exitNumber) {
				p.d = Left;
				p.e = QPoint(i - 1, j);
				_passes.append(p);
				if(i == 0) {
					MazeItemExit *ex = new MazeItemExit();
					ex->setEngine(this);
					ex->setLocation(p.e);
				}
			}

			if(i < size - 1 || 3*size + j == exitNumber) {
				p.d = Right;
				p.e = QPoint(i + 1, j);
				_passes.append(p);
				if(i == size - 1) {
					MazeItemExit *ex = new MazeItemExit();
					ex->setEngine(this);
					ex->setLocation(p.e);
				}
			}
		}
	}

	// Generate random walls
	foreach (QPoint i, availableLocations) {
		// In each square 2x2 we create one wall
		if (availableLocations.contains(i + QPoint(0,1)) &&
			availableLocations.contains(i + QPoint(1,0)) &&
			availableLocations.contains(i + QPoint(1,1))) {

			switch(rand() % 4) {
			case 0:
				p.s = i;
				p.e = i + QPoint(0,1);
				p.d = Down;
				break;
			case 1:
				p.s = i;
				p.e = i + QPoint(1,0);
				p.d = Right;
				break;
			case 2:
				p.s = i + QPoint(0,1);
				p.e = i + QPoint(1,1);
				p.d = Right;
				break;
			case 3:
				p.s = i + QPoint(1,0);
				p.e = i + QPoint(1,1);
				p.d = Down;
				break;
			}
			_passes.removeAll(p);
			std::swap(p.s, p.e);
			p.d = getOppositeDirection(p.d);
			_passes.removeAll(p);
		}
	}

	// Now check for pass blockers and get rid of them
	QList<pass> *blocker;
	foreach (QPoint i, availableLocations) {
		blocker = new QList<pass>();
		// We need a direction for which a wall exists on the right side
		if(!canGo(i, Up))
			findBlockers(blocker, i, Left, i, Left);
		else if(!canGo(i, Left))
			findBlockers(blocker, i, Down, i, Down);
		else if(!canGo(i, Down))
			findBlockers(blocker, i, Right, i, Right);
		else if(!canGo(i, Right))
			findBlockers(blocker, i, Up, i, Up);
		// Don't check for blockers if there're no walls around

		if(blocker->count() > 0) {
			qDebug() << "found a closed loop consisting of" << blocker->count() << "walls";
			p = blocker->at(rand() % blocker->count());
			_passes.append(p);
			std::swap(p.s, p.e);
			p.d = getOppositeDirection(p.d);
			_passes.append(p);
		}

		delete blocker;
	}
}

void MazeEngine::findBlockers(QList<pass> *blockingWalls, QPoint initialLocation, direction initialDirection, QPoint location, direction dir, bool debug, bool start) {
	if(debug) {
		if(start)
			qDebug() << "started search in" << initialLocation.x() << initialLocation.y() << initialDirection;
		else
			qDebug() << location.x() << location.y() << dir;
	}
	if(location == initialLocation && dir == initialDirection && !start) {
		if(debug)
			qDebug() << "found a loop";
		return;
	}

	// Check for a wall to the right of current direction
	direction checkDir;
	switch (dir) {
	case Up:
		checkDir = Right;
		break;
	case Right:
		checkDir = Down;
		break;
	case Down:
		checkDir = Left;
		break;
	case Left:
		checkDir = Up;
		break;
	case Error:
		qDebug() << "wtf?";
		break;
	}
	if(canGo(location, checkDir)) {
		if(!availableLocations.contains(move(location, checkDir))) {
			// If there's an exit, stop checking and clear all found walls
			blockingWalls->clear();
			return;
		}
		// If there's a pass, go into it
		findBlockers(blockingWalls, initialLocation, initialDirection, move(location, checkDir), checkDir, debug, false);
		return;
	}
	// If there's a wall we either add it to the list or remove it from there depending on did we see this wall previously
	if(availableLocations.contains(location + directionToPoint(checkDir))) {
		pass p;
		p.d = checkDir;
		p.s = location;
		p.e = location + directionToPoint(p.d);
		if(debug)
			qDebug() << "wall" << p.s.x() << p.s.y() << p.d;
		if(blockingWalls->contains(p)) {
			blockingWalls->removeAll(p);
		}
		else {\
			blockingWalls->append(p);
		}
		std::swap(p.s, p.e);
		p.d = getOppositeDirection(p.d);
		if(blockingWalls->contains(p)) {
			blockingWalls->removeAll(p);
		}
		else {
			blockingWalls->append(p);
		}
	}

	// If there's a wall to the right and we can go further, do it
	if(canGo(location, dir)) {
		if(!availableLocations.contains(location + directionToPoint(dir))) {
			// If there's an exit, stop checking and clear all found walls
			blockingWalls->clear();
			return;
		}
		findBlockers(blockingWalls, initialLocation, initialDirection, move(location, dir), dir, debug, false);
		return;
	} else {
		// If there's a wall in front, turn left on the place
		direction newDir;
		switch (dir) {
		case Up:
			newDir = Left;
			break;
		case Left:
			newDir = Down;
			break;
		case Down:
			newDir = Right;
			break;
		case Right:
			newDir = Up;
			break;
		case Error:
			qDebug() << "wtf?";
			break;
		}

		if(!availableLocations.contains(location + directionToPoint(newDir))) {
			// If there's an exit, stop checking and clear all found walls
			blockingWalls->clear();
			return;
		}
		findBlockers(blockingWalls, initialLocation, initialDirection, location, newDir, debug, false);
		return;
	}
}

bool MazeEngine::canGo(QPoint location, direction dir) {
	foreach(pass i, _passes) {
		if(i.s == location && i.d == dir)
			return true;
	}
	return false;
}

bool MazeEngine::canGo(QPoint location, QString dir) {
	if(directionFromString(dir) != Error)
		return canGo(location, directionFromString(dir));
	else return false;
}

// Should only be called after check with canGo()
QPoint MazeEngine::move(QPoint location, direction dir) {
	foreach(pass i, _passes) {
		if(i.s == location && i.d == dir)
			return i.e;
	}
}

void MazeEngine::registerItem(MazeItem *item) {
	qDebug() << "item added";
	_items.append(item);
	connect(item, SIGNAL(locationChanged(MazeItem*)), this, SLOT(checkIntersection(MazeItem*)));
	// Movable items have turns
	if(item->movable) {
		// If added item is marked as having current turn, remove this mark from all other items. Just for the case.
		if(item->currentTurn()) {
			foreach (MazeItem *i, _turns) {
				i->setCurrentTurn(false);
			}
		}
		_turns.append(item);
		connect(item, SIGNAL(turnEnded()), this, SLOT(switchTurn()));
	}
}

void MazeEngine::removeItem(MazeItem *item) {
	qDebug() << "item removed";
	_items.removeAll(item);
}

void MazeEngine::checkIntersection(MazeItem *item) {
	foreach (MazeItem *i, _items) {
		if(i != item && i->location() == item->location()) {
			qDebug() << "Intersection detected";
			emit intersection(item, i);
			item->intersected(i);
			i->intersected(item);
		}
	}
}

void MazeEngine::switchTurn() {
	bool switchFlag = false;
	foreach (MazeItem *i, _turns) {
		if(switchFlag) {
			i->setCurrentTurn(true);
			switchFlag = false;
		}
		else if(i->currentTurn()) {
			i->setCurrentTurn(false);
			switchFlag = true;
		}
	}
	if(switchFlag)
		_turns.first()->setCurrentTurn(true);
}
