#include "tools.h"

Tools::Tools(QObject *parent) :
	QObject(parent) {}

void Tools::quit() {
	exit(0);
}
