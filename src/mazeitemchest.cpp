#include "mazeitemchest.h"

MazeItemChest::MazeItemChest(QObject *parent) :
	MazeItem(parent) {
	killable = false;
	killer = false;
	pickable = true;
	picker = false;
	movable = false;
}
