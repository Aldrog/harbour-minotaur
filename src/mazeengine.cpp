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
	for (int i = 0; i < size; i++) {
		for(int g = 0; g < 5; g++) {
			p.s = QPoint(rand() % size, i);
			switch (rand() % 4) {
			case 0:
				p.d = Up;
				p.e = p.s + QPoint(0, -1);
				break;
			case 1:
				p.d = Down;
				p.e = p.s + QPoint(0, 1);
				break;
			case 2:
				p.d = Left;
				p.e = p.s + QPoint(-1, 0);
				break;
			case 3:
				p.d = Right;
				p.e = p.s + QPoint(1, 0);
				break;
			}
			_passes.removeOne(p);
			std::swap(p.s, p.e);
			p.d = getOppositeDirection(p.d);
			_passes.removeOne(p);
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
	qDebug() << "item added" << item->currentTurn();
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
	qDebug() << "item added 2";
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
