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

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
	id: page
	allowedOrientations: Orientation.All

	PageHeader {
		id: header
		title: qsTr("You died. What a pity!")
	}

	Label {
		id: winningText
		anchors {
			top: header.bottom
			left: parent.left
			right: parent.right
			margins: Theme.paddingLarge
		}
		text: qsTr("You were eaten by evil minotaur.\n\nSee you next game.")
		wrapMode: Text.WordWrap
	}

	Button {
		id: endButton
		anchors {
			bottom: parent.bottom
			horizontalCenter: parent.horizontalCenter
			margins: Theme.paddingLarge
		}
		text: qsTr("Goodbye!")
		onClicked: pageStack.pop()
	}
}
