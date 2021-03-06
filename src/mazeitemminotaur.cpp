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

#include "mazeitemminotaur.h"
#include <QtConcurrent/QtConcurrent>
#include <QDebug>

MazeItemMinotaur::MazeItemMinotaur(QObject *parent) :
	MazeItem(parent)
{
	killable = false;
	killer = true;
	pickable = false;
	picker = false;
	movable = true;
	connect(this, SIGNAL(turnStarted()), this, SLOT(turn()));
}

void MazeItemMinotaur::setDifficulty(Difficulty newDifficulty) {
	if(_dif != newDifficulty) {
		_dif = newDifficulty;
		emit difficultyChanged();
	}
}

void MazeItemMinotaur::turn() {
	qDebug() << "minotaur turn";
	switch (_dif) {
	case LowDifficulty:
		QtConcurrent::run(this, &MazeItemMinotaur::turnLowDifficulty);
		break;
	case MediumDifficulty:
		QtConcurrent::run(this, &MazeItemMinotaur::turnMediumDifficulty);
		break;
	case HighDifficulty:
		QtConcurrent::run(this, &MazeItemMinotaur::turnHighDifficulty);
		break;
	default:
		qDebug() << "This should never happen";
		break;
	}
}

void MazeItemMinotaur::turnLowDifficulty() {
	QList<direction> allowedList = QList<direction>();
	direction d;
	d = Up;
	if(_engine->canGo(this->location(), d) && _engine->availableLocations.contains(_engine->move(this->location(), d)))
		allowedList += d;
	d = Down;
	if(_engine->canGo(this->location(), d) && _engine->availableLocations.contains(_engine->move(this->location(), d)))
		allowedList += d;
	d = Left;
	if(_engine->canGo(this->location(), d) && _engine->availableLocations.contains(_engine->move(this->location(), d)))
		allowedList += d;
	d = Right;
	if(_engine->canGo(this->location(), d) && _engine->availableLocations.contains(_engine->move(this->location(), d)))
		allowedList += d;

	this->move(allowedList.at(rand() % allowedList.count()));
}

void MazeItemMinotaur::turnMediumDifficulty() {
	// Not implemented
	usleep(1000000);
	emit turnEnded();
}

void MazeItemMinotaur::turnHighDifficulty() {
	// Not implemented
	usleep(10000000);
	emit turnEnded();
}
