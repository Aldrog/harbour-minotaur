import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.minotaur.maze 1.0

Page {
	id: page

	property int size: 10

	MazeEngine {
		id: engine
		dimension: size
	}

	Column {
		anchors.fill: parent

		PageHeader {
			title: qsTr("Minotaur. Level 1")
		}

		//Parent rectangle for labyrinth cells
		Rectangle {
			id: labyrinth
			anchors {
				left: parent.left
				right: parent.right
				margins: Theme.paddingMedium
			}
			height: width
			color: "transparent"
			border.width: 3
			border.color: "white"

			property int youx: 0
			property int youy: 0

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
								width: labyrinth.width / size
								height: labyrinth.height / size
								color: (lx == labyrinth.youx && ly == labyrinth.youy) ? "magenta" : "transparent"
								border.width: 1
								border.color: "white"
							}
						}
					}
				}
			}
		}
		Button {
			text: "Up"
			onClicked: {
				if(engine.canGo(labyrinth.youx, labyrinth.youy, "Up"))
					labyrinth.youy--
			}
		}
		Button {
			text: "Down"
			onClicked: {
				if(engine.canGo(labyrinth.youx, labyrinth.youy, "Down"))
					labyrinth.youy++
			}
		}
		Button {
			text: "Left"
			onClicked: {
				if(engine.canGo(labyrinth.youx, labyrinth.youy, "Left"))
					labyrinth.youx--
			}
		}
		Button {
			text: "Right"
			onClicked: {
				if(engine.canGo(labyrinth.ux, labyrinth.uy, "Right"))
					labyrinth.youx++
			}
		}
	}
}
