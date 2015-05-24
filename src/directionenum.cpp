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

#include "directionenum.h"

direction getOppositeDirection(direction d) {
	//					Opposite to Up, Down, Left, Right, Error
	static direction opposite[] = { Down, Up, Right, Left, Error };
	return opposite[d];
}

QPoint directionToPoint(direction dir) {
	switch (dir) {
	case Up:
		return QPoint(0,-1);
		break;
	case Down:
		return QPoint(0,1);
		break;
	case Left:
		return QPoint(-1,0);
		break;
	case Right:
		return QPoint(1,0);
		break;
	}
}

direction directionFromString(QString s) {
	if(s.toLower() == "up")
		return Up;
	if(s.toLower() == "down")
		return Down;
	if(s.toLower() == "left")
		return Left;
	if(s.toLower() == "right")
		return Right;
	return Error;
}
