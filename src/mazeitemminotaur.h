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

#ifndef MAZEITEMMINOTAUR_H
#define MAZEITEMMINOTAUR_H

#include "mazeitem.h"
#include "mazeitemkiller.h"

class MazeItemMinotaur :
        public MazeItem,
        public MazeItemKiller
{
	Q_OBJECT
public:
	explicit MazeItemMinotaur(QObject *parent = 0);

	enum Difficulty {
		LowDifficulty,
		MediumDifficulty,
		HighDifficulty
	};
	Q_ENUMS(Difficulty)
	Q_PROPERTY(Difficulty difficulty READ difficulty WRITE setDifficulty NOTIFY difficultyChanged)

	inline Difficulty difficulty() { return _dif; }
	void setDifficulty(Difficulty newDifficulty);

signals:
	void difficultyChanged();

public slots:
	void turn();
    void findPaths() { MazeItemKiller::findPaths(); }

private:
	void turnLowDifficulty();
	void turnMediumDifficulty();
	void turnHighDifficulty();

	Difficulty _dif;
};

#endif // MAZEITEMMINOTAUR_H
