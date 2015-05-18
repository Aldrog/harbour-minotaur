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

direction getOppositeDirection(direction d) {
	static direction opposite[] = { Down, Up, Right, Left };
	return opposite[d];
}

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
	foreach (QPoint i, availableLocations) {
		int passCount = (canGo(i, "Up") ? 1 : 0) + (canGo(i, "Down") ? 1 : 0) + (canGo(i, "Left") ? 1 : 0) + (canGo(i, "Right") ? 1 : 0);
		// One-cell blocker
		if(passCount == 0) {
			p.s = i;
			bool blockerRemoved = false;
			while(!blockerRemoved) {
				qDebug() << "trying to remove blocker";
				switch(rand() % 4) {
				case 0:
					p.e = i + QPoint(0,-1);
					p.d = Up;
					if(availableLocations.contains(p.e)) {
						_passes.append(p);
						std::swap(p.s, p.e);
						p.d = getOppositeDirection(p.d);
						_passes.append(p);
						blockerRemoved = true;
					}
					break;
				case 1:
					p.e = i + QPoint(0,1);
					p.d = Down;
					if(availableLocations.contains(p.e)) {
						_passes.append(p);
						std::swap(p.s, p.e);
						p.d = getOppositeDirection(p.d);
						_passes.append(p);
						blockerRemoved = true;
					}
					break;
				case 2:
					p.e = i + QPoint(-1,0);
					p.d = Left;
					if(availableLocations.contains(p.e)) {
						_passes.append(p);
						std::swap(p.s, p.e);
						p.d = getOppositeDirection(p.d);
						_passes.append(p);
						blockerRemoved = true;
					}
					break;
				case 3:
					p.e = i + QPoint(1,0);
					p.d = Right;
					if(availableLocations.contains(p.e)) {
						_passes.append(p);
						std::swap(p.s, p.e);
						p.d = getOppositeDirection(p.d);
						_passes.append(p);
						blockerRemoved = true;
					}
					break;
				}
			}
			qDebug() << "One-cell blocker removed";
		}
		// Two-cells blocker
		if(passCount == 1) {
			// Find which cell is the only pass leading to
			QPoint neighbour;
			if(canGo(i, "Up"))
				neighbour = i + QPoint(0,-1);
			if(canGo(i, "Down"))
				neighbour = i + QPoint(0,1);
			if(canGo(i, "Left"))
				neighbour = i + QPoint(-1,0);
			if(canGo(i, "Right"))
				neighbour = i + QPoint(1,0);
			// Check if it also has only one pass. If so, we have a blocker
			int nPassCount = (canGo(neighbour, "Up") ? 1 : 0) + (canGo(neighbour, "Down") ? 1 : 0) + (canGo(neighbour, "Left") ? 1 : 0) + (canGo(neighbour, "Right") ? 1 : 0);
			if(nPassCount == 1) {
				bool blockerRemoved = false;
				while(!blockerRemoved) {
					qDebug() << "trying to remove blocker";
					switch(rand() % 2) {
					case 0:
						p.s = i;
						break;
					case 1:
						p.s = neighbour;
						break;
					}
					switch(rand() % 4) {
					case 0:
						p.e = p.s + QPoint(0,-1);
						p.d = Up;
						// If we are not lucky, we may get really a lot of iterations before we finally fix the blocker
						// Maybe it would be better to somehow save direction in which we can go and rand()%3
						if(!canGo(p.s, "Up") && availableLocations.contains(p.e)) {
							_passes.append(p);
							std::swap(p.s, p.e);
							p.d = getOppositeDirection(p.d);
							_passes.append(p);
							blockerRemoved = true;
						}
						break;
					case 1:
						p.e = p.s + QPoint(0,1);
						p.d = Down;
						if(!canGo(p.s, "Down") && availableLocations.contains(p.e)) {
							_passes.append(p);
							std::swap(p.s, p.e);
							p.d = getOppositeDirection(p.d);
							_passes.append(p);
							blockerRemoved = true;
						}
						break;
					case 2:
						p.e = p.s + QPoint(-1,0);
						p.d = Left;
						if(!canGo(p.s, "Left") && availableLocations.contains(p.e)) {
							_passes.append(p);
							std::swap(p.s, p.e);
							p.d = getOppositeDirection(p.d);
							_passes.append(p);
							blockerRemoved = true;
						}
						break;
					case 3:
						p.e = p.s + QPoint(1,0);
						p.d = Right;
						if(!canGo(p.s, "Right") && availableLocations.contains(p.e)) {
							_passes.append(p);
							std::swap(p.s, p.e);
							p.d = getOppositeDirection(p.d);
							_passes.append(p);
							blockerRemoved = true;
						}
						break;
					}
				}
				qDebug() << "Two-cells blocker removed";
			}
		}
	}
}

bool MazeEngine::canGo(QPoint location, QString dir) {
	if(dir.toLower() == "up") {
		foreach(pass i, _passes) {
			if(i.s == location && i.d == Up)
				return true;
		}
	}
	if(dir.toLower() == "down") {
		foreach(pass i, _passes) {
			if(i.s == location && i.d == Down)
				return true;
		}
	}
	if(dir.toLower() == "left") {
		foreach(pass i, _passes) {
			if(i.s == location && i.d == Left)
				return true;
		}
	}
	if(dir.toLower() == "right") {
		foreach(pass i, _passes) {
			if(i.s == location && i.d == Right)
				return true;
		}
	}
	return false;
}

void MazeEngine::registerItem(MazeItem *item) {
	qDebug() << "item added";
	_items.append(item);
	connect(item, SIGNAL(locationChanged(MazeItem*)), this, SLOT(checkIntersection(MazeItem*)));
	// Movable items have turns
	if(item->movable) {
		// If added item is marked as having current turn, remove this mark from all other items
		if(item->currentTurn()) {
			foreach (MazeItem *i, _turns) {
				i->setCurrentTurn(false);
			}
		} else {
			// If no item has current turn mark, add it to this item
			item->setCurrentTurn(true);
			foreach (MazeItem *i, _turns) {
				if(i->currentTurn())
					item->setCurrentTurn(false);
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
