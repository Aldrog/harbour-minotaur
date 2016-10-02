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

#include "mazeitemkiller.h"

MazeItemKiller::MazeItemKiller(MazeItem *thisItem) {
    mThis = thisItem;
}

void MazeItemKiller::findPaths() {
    paths.clear();
    foreach (MazeItem *target, targets) {
        QList<QPoint> path;
        path.append(mThis->location());
        if(mThis->searchPath(&path, target->location())) {
            paths.insert(target, path);
        }
    }
}
