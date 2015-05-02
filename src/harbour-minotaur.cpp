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

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>

#include <QtQml>
#include <QGuiApplication>
#include <QQuickView>

#include "tools.h"
#include "mazeengine.h"

int main(int argc, char *argv[])
{
	// SailfishApp::main() will display "qml/template.qml", if you need more
	// control over initialization, you can use:
	//
	//   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
	//   - SailfishApp::createView() to get a new QQuickView * instance
	//   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
	//
	// To display the view, call "show()" (will show fullscreen on device).

	QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
	qmlRegisterType<MazeEngine>("harbour.minotaur.maze", 1, 0, "MazeEngine");
	QScopedPointer<QQuickView> view(SailfishApp::createView());
	Tools* cpptools = new Tools();
	view->rootContext()->setContextProperty("cpptools", cpptools);
	view->setSource(SailfishApp::pathTo("qml/harbour-minotaur.qml"));
	view->show();
	return app->exec();
}

