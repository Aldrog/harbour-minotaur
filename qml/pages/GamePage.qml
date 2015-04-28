import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
	id: page

	property int size: 10

	Column {
		anchors.fill: parent

		PageHeader {
			title: qsTr("Minotaur. Level 1")
		}

		//Parent rectangle for labyrinth cells
		Rectangle {
			id: labyrinth

			border.width: 3
			border.color: "white"
			color: "transparent"
			anchors {
				left: parent.left
				right: parent.right
				margins: Theme.paddingMedium
			}
			height: width

			//Vertical column
			Column {
				Repeater {
					model: size

					//With 10 horizontal rows in it
					Row {
						property int ly: index
						Repeater {
							model: size

							//With 10 rectangles in each one
							Rectangle {
								property int lx: index
								border.width: 1
								border.color: "white"
								color: "transparent"
								Label {
									text: lx + " " + ly
								}

								width: labyrinth.width / size
								height: labyrinth.height / size
							}
						}
					}
				}
			}
		}
	}
} //Quite a lot of closing brackets
