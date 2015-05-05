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

void MazeEngine::generateRandom(int size) {
	pass p;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			p.s = QPoint(i, j);

			if(j > 0) {
				p.d = Up;
				p.e = QPoint(i, j - 1);
				_passes.append(p);
			}

			if(j < size - 1) {
				p.d = Down;
				p.e = QPoint(i, j + 1);
				_passes.append(p);
			}

			if(i > 0) {
				p.d = Left;
				p.e = QPoint(i - 1, j);
				_passes.append(p);
			}

			if(i < size - 1) {
				p.d = Right;
				p.e = QPoint(i + 1, j);
				_passes.append(p);
			}
		}
	}

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

bool MazeEngine::canGo(int x, int y, QString dir) {
	return canGo(QPoint(x, y), dir);
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
}

void MazeEngine::checkIntersection(MazeItem *item) {
	foreach (MazeItem *i, _items) {
		if(i != item && i->location() == item->location()) {
			emit intersect(item, i);
			item->intersected(i);
			i->intersected(item);
		}
	}
}
