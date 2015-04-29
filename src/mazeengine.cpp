#include "mazeengine.h"
#include <QDebug>

MazeEngine::MazeEngine(QObject *parent, int dimension) :
	QObject(parent) {
}

int MazeEngine::dimension() {
	return _mazesize;
}

// Warning - changing dimension will destroy the maze
void MazeEngine::setDimension(int dimension) {
	if(dimension != _mazesize) {
		_mazesize = dimension;
		dimensionChanged();
		pass p;
		for (int i = 0; i < dimension; i++)
			for (int j = 0; j < dimension; j++) {
				p.s = QPoint(i, j);

				if(j > 0) {
					p.d = Up;
					p.e = QPoint(i, j - 1);
					_passes.append(p);
				}

				if(j < dimension - 1) {
					p.d = Down;
					p.e = QPoint(i, j + 1);
					_passes.append(p);
				}

				if(i > 0) {
					p.d = Left;
					p.e = QPoint(i - 1, j);
					_passes.append(p);
				}

				if(i < dimension - 1) {
					p.d = Right;
					p.e = QPoint(i + 1, j);
					_passes.append(p);
				}
			}
	}
}

bool MazeEngine::canGo(int x, int y, QString dir) {
	if(dir.toLower() == "up") {
		foreach(pass i, _passes) {
			if(i.s.x() == x && i.s.y() == y && i.d == Up)
				return true;
		}
	}
	if(dir.toLower() == "down") {
		foreach(pass i, _passes) {
			if(i.s.x() == x && i.s.y() == y && i.d == Down)
				return true;
		}
	}
	if(dir.toLower() == "left") {
		foreach(pass i, _passes) {
			if(i.s.x() == x && i.s.y() == y && i.d == Left)
				return true;
		}
	}
	if(dir.toLower() == "right") {
		foreach(pass i, _passes) {
			if(i.s.x() == x && i.s.y() == y && i.d == Right)
				return true;
		}
	}
	return false;
}
