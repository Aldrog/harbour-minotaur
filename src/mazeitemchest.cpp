#include "mazeitemchest.h"

MazeItemChest::MazeItemChest(QObject *parent) :
	MazeItem(parent) {
	killable = false;
	killing = false;
	pickable = true;
	picking = false;
}
