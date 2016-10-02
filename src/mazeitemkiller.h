/*
 * Copyright © 2015 Andrew Penkrat
 *
 * This file is part of TwitchTube.
 *
 * TwitchTube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TwitchTube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TwitchTube.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAZEITEMKILLER_H
#define MAZEITEMKILLER_H

#include <QObject>
#include "mazeitem.h"
#include <QList>
#include <QHash>

class MazeItemKiller
{
public:
    MazeItemKiller(MazeItem *thisItem);

    QList<MazeItem*> targets;
    QHash<MazeItem*, QList<QPoint> > paths;

    // slots
    void findPaths();
private:
    MazeItem *mThis;
};

#endif // MAZEITEMKILLER_H
