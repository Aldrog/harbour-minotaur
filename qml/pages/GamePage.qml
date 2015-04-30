import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.minotaur.maze 1.0

Page {
	id: page

	property int size: 11
	property int wallWidth: 4
	property var walls: []

	MazeEngine {
		id: engine
		dimension: size
	}

	Column {
		anchors.fill: parent

		PageHeader {
			title: qsTr("Level 1")
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

			property int youx: size/2
			property int youy: size/2

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

								Rectangle {
									id: topB
									property bool isWall: walls.indexOf(lx + "\\" + ly + "\\t") >= 0
									property int bWidth: isWall ? wallWidth : 1
									anchors {
										verticalCenter: parent.top
										left: angleB.right
										right: parent.right
										//leftMargin: bWidth / 2
										rightMargin: bWidth / 2
									}
									visible: ly > 0
									height: bWidth
									color: isWall ? Theme.primaryColor : Theme.highlightColor
								}

								Rectangle {
									id: leftB
									property bool isWall: walls.indexOf(lx + "\\" + ly + "\\l") >= 0
									onIsWallChanged: console.log(lx, ly, walls.length)
									property int bWidth: isWall ? wallWidth : 1
									anchors {
										horizontalCenter: parent.left
										top: angleB.bottom
										bottom: parent.bottom
										//topMargin: bWidth / 2
										bottomMargin: bWidth / 2
									}
									visible: lx > 0
									width: bWidth
									color: isWall ? Theme.primaryColor : Theme.highlightColor
								}

								Rectangle {
									id: angleB
									property bool isWall: walls.indexOf(lx + "\\" + ly + "\\l") >= 0 ||
														  walls.indexOf(lx + "\\" + ly + "\\t") >= 0 ||
														  walls.indexOf((lx - 1) + "\\" + ly + "\\t") >= 0 ||
														  walls.indexOf(lx + "\\" + (ly - 1) + "\\l") >= 0
									anchors.verticalCenter: parent.top
									anchors.horizontalCenter: parent.left
									width: isWall ? wallWidth : 1
									height: width
									visible: lx * ly > 0
									color: isWall ? Theme.primaryColor : Theme.highlightColor
								}
							}
						}
					}
				}
			}
		}
		//Now here're some messy machinations with opacity effects
		OpacityRampEffect {
			id: eff1
			sourceItem: labyrinth
			direction: OpacityRamp.LeftToRight
			offset: 0.8
			slope: 5
		}
		OpacityRampEffect {
			id: eff2
			sourceItem: eff1
			direction: OpacityRamp.TopToBottom
			offset: 0.8
			slope: 5
		}
		OpacityRampEffect {
			id: eff3
			sourceItem: eff2
			direction: OpacityRamp.RightToLeft
			offset: 0.8
			slope: 5
		}
		OpacityRampEffect {
			id: eff4
			sourceItem: eff3
			direction: OpacityRamp.BottomToTop
			offset: 0.8
			slope: 5
		}

		Button {
			text: "Up"
			onClicked: {
				if(engine.canGo(labyrinth.youx, labyrinth.youy, "Up"))
					labyrinth.youy--
				else {
					if(walls.indexOf(labyrinth.youx + "\\" + labyrinth.youy + "\\t") < 0)
						walls = walls + [labyrinth.youx + "\\" + labyrinth.youy + "\\t"]
				}
			}
		}
		Button {
			text: "Down"
			onClicked: {
				if(engine.canGo(labyrinth.youx, labyrinth.youy, "Down"))
					labyrinth.youy++
				else {
					if(walls.indexOf(labyrinth.youx + "\\" + (labyrinth.youy + 1) + "\\t") < 0)
						walls = walls + [labyrinth.youx + "\\" + (labyrinth.youy + 1) + "\\t"]
				}
			}
		}
		Button {
			text: "Left"
			onClicked: {
				if(engine.canGo(labyrinth.youx, labyrinth.youy, "Left"))
					labyrinth.youx--
				else {
					if(walls.indexOf(labyrinth.youx + "\\" + labyrinth.youy + "\\l") < 0)
						walls = walls + [labyrinth.youx + "\\" + labyrinth.youy + "\\l"]
				}
			}
		}
		Button {
			text: "Right"
			onClicked: {
				if(engine.canGo(labyrinth.youx, labyrinth.youy, "Right"))
					labyrinth.youx++
				else {
					if(walls.indexOf((labyrinth.youx + 1) + "\\" + labyrinth.youy + "\\l") < 0)
						walls = walls + [(labyrinth.youx + 1) + "\\" + labyrinth.youy + "\\l"]
				}
			}
		}
	}
}
