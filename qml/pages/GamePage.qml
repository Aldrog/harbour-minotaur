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

import QtQuick 2.1
import Sailfish.Silica 1.0
import QtGraphicalEffects 1.0
import QtFeedback 5.0
import harbour.minotaur.maze 1.0

Page {
	id: page
	allowedOrientations: Orientation.Portrait | Orientation.PortraitInverted

	property int size: 9
	property int wallWidth: 4
	// We store known walls as a string consisting of lines like \x\y\l or \x\y\t. l and t mean left and top sides of the cell.
	property string walls: ""

    HapticsEffect {
        id: dangerEffect
        attackIntensity: 1.0
        attackTime: 2500
        intensity: 2.0
        duration: 1000
        fadeIntensity: 0.5
        fadeTime: 1500
    }

	MazeEngine {
		id: engine
		Component.onCompleted: {
			generateRandom(10)
			player.randLocation()
			minotaur.randLocation()
		}
	}
	MazeItemPlayer {
		id: player
		engine: engine
		currentTurn: true

		onOutOfMaze: pageStack.replace(Qt.resolvedUrl("WinPage.qml"))
		onWasKilled: pageStack.replace(Qt.resolvedUrl("LosePage.qml"))
        onDangerChanged: {
            if(dangerLevel > 0)
                dangerEffect.start()
        }
	}
	MazeItemMinotaur {
		id: minotaur
		engine: engine
		difficulty: MazeItemMinotaur.LowDifficulty
	}

	PageHeader {
		id: title
		title: qsTr("Level 1")
	}

	// Parent rectangle for labyrinth cells
	Rectangle {
		id: labyrinth
		anchors {
			top: title.bottom
			left: parent.left
			right: parent.right
			margins: Theme.paddingMedium
		}
		height: width
		color: "transparent"

		// Vertical column
		Column {
			id: rootContainer
			state: "default"

			Repeater {
				model: size + 2

				// With 13 horizontal rows in it
				Row {
					property int ly: index - 1
					Repeater {
						model: size + 2

						// With 13 rectangles in each one
						Rectangle {
							property int lx: index - 1
							width: labyrinth.width / size
							height: labyrinth.height / size
							color: "transparent"

							Rectangle {
								id: topB
								property bool isWall: walls.split("\n").indexOf(lx + "\\" + ly + "\\t") >= 0
								property int bWidth: isWall ? wallWidth : 1
								anchors {
									verticalCenter: parent.top
									left: angleB.right
									right: parent.right
									rightMargin: bWidth / 2
								}
								height: bWidth
								color: isWall ? Theme.primaryColor : Theme.highlightColor
								opacity: isWall ? 1 : 0.75
							}

							Rectangle {
								id: leftB
								property bool isWall: walls.split("\n").indexOf(lx + "\\" + ly + "\\l") >= 0
								property int bWidth: isWall ? wallWidth : 1
								anchors {
									horizontalCenter: parent.left
									top: angleB.bottom
									bottom: parent.bottom
									bottomMargin: bWidth / 2
								}
								width: bWidth
								color: isWall ? Theme.primaryColor : Theme.highlightColor
								opacity: isWall ? 1 : 0.75
							}

							Rectangle {
								id: angleB
								property bool isWall: walls.split("\n").indexOf(lx + "\\" + ly + "\\l") >= 0 ||
													  walls.split("\n").indexOf(lx + "\\" + ly + "\\t") >= 0 ||
													  walls.split("\n").indexOf((lx - 1) + "\\" + ly + "\\t") >= 0 ||
													  walls.split("\n").indexOf(lx + "\\" + (ly - 1) + "\\l") >= 0
								anchors.verticalCenter: parent.top
								anchors.horizontalCenter: parent.left
								width: isWall ? wallWidth : 1
								height: width
								color: isWall ? Theme.primaryColor : Theme.highlightColor
								opacity: isWall ? 1 : 0.75
							}
						}
					}
				}
			}

			states: [
				State {
					name: "default"
                    changes: PropertyChanges {
                        target: rootContainer
                        x: - labyrinth.width / size
                        y: - labyrinth.height / size
                    }
				},
				State {
					name: "movingUp"
                    changes: PropertyChanges {
                        target: rootContainer
                        x: - labyrinth.width / size
                        y: 0
                    }
				},
				State {
					name: "movingDown"
                    changes: PropertyChanges {
                        target: rootContainer
                        x: - labyrinth.width / size
                        y: -2 * labyrinth.height / size
                    }
				},
				State {
					name: "movingLeft"
                    changes: PropertyChanges {
                        target: rootContainer
                        x: 0
                        y: - labyrinth.height / size
                    }
				},
				State {
					name: "movingRight"
                    changes: PropertyChanges {
                        target: rootContainer
                        x: -2 * labyrinth.width / size
                        y: - labyrinth.height / size
                    }
				}]

			transitions: [
				Transition {
					to: "movingUp,movingDown,movingLeft,movingRight"
					PropertyAnimation {
						easing.type: Easing.InOutQuad
						properties: "x,y"
						duration: 300
					}

					onRunningChanged: {
						if(!running) {
							var newWalls = ""
							var wsplit = walls.split("\n")
							var i, wcoords
							for(i in wsplit) {
								if(wsplit[i]) {
									wcoords = wsplit[i].split("\\")
									switch (rootContainer.state) {
									case "movingUp":
										wsplit[i] = wcoords[0] + "\\" + (parseInt(wcoords[1]) + 1) + "\\" + wcoords[2] + "\n"
										break
									case "movingDown":
										wsplit[i] = wcoords[0] + "\\" + (parseInt(wcoords[1]) - 1) + "\\" + wcoords[2] + "\n"
										break
									case "movingLeft":
										wsplit[i] = (parseInt(wcoords[0]) + 1) + "\\" + wcoords[1] + "\\" + wcoords[2] + "\n"
										break
									case "movingRight":
										wsplit[i] = (parseInt(wcoords[0]) - 1) + "\\" + wcoords[1] + "\\" + wcoords[2] + "\n"
										break
									}
									newWalls += wsplit[i]
								}
							}
							walls = newWalls
							rootContainer.state = "default"
						}
					}
				}]
		}

        Image {
			id: playerRect
			anchors.centerIn: parent
            width: labyrinth.width / size
            height: labyrinth.height / size
            source: "../images/hero.svg"
        }

		Rectangle {
			id: controlsContainer
			anchors.fill: labyrinth
//			z: -1
			color: "transparent"
			property bool enabled: rootContainer.state == "default" && player.currentTurn
			property string direction: ""

			Image {
				id: moveUpImg
				visible: false
				anchors {	top: parent.top
							bottom: parent.verticalCenter
							horizontalCenter: parent.horizontalCenter
						}
				width: height
				source: "../images/move.svg"
			}
			ColorOverlay {
				id: upEffect
				property bool highlighted: controlsContainer.direction == "Up"
				anchors.fill: moveUpImg
				source: moveUpImg
				rotation: 0
				opacity: controlsContainer.enabled ? (highlighted ? 1.0 : 0.6) : 0.5
				color: controlsContainer.enabled ? (highlighted ? Theme.highlightColor : Theme.primaryColor) : Theme.secondaryColor
			}

			Image {
				id: moveDownImg
				visible: false
				anchors {	bottom: parent.bottom
							top: parent.verticalCenter
							horizontalCenter: parent.horizontalCenter
						}
				width: height
				source: "../images/move.svg"
			}
			ColorOverlay {
				id: downEffect
				property bool highlighted: controlsContainer.direction == "Down"
				anchors.fill: moveDownImg
				source: moveDownImg
				rotation: 180
				opacity: controlsContainer.enabled ? (highlighted ? 1.0 : 0.6) : 0.5
				color: controlsContainer.enabled ? (highlighted ? Theme.highlightColor : Theme.primaryColor) : Theme.secondaryColor
			}

			Image {
				id: moveLeftImg
				visible: false
				anchors {	left: parent.left
							right: parent.horizontalCenter
							verticalCenter: parent.verticalCenter
						}
				height: width
				source: "../images/move.svg"
			}
			ColorOverlay {
				id: leftEffect
				property bool highlighted: controlsContainer.direction == "Left"
				anchors.fill: moveLeftImg
				source: moveLeftImg
				rotation: -90
				opacity: controlsContainer.enabled ? (highlighted ? 1.0 : 0.6) : 0.5
				color: controlsContainer.enabled ? (highlighted ? Theme.highlightColor : Theme.primaryColor) : Theme.secondaryColor
			}

			Image {
				id: moveRightImg
				visible: false
				anchors {	left: parent.horizontalCenter
							right: parent.right
							verticalCenter: parent.verticalCenter
						}
				height: width
				source: "../images/move.svg"
			}
			ColorOverlay {
				id: rightEffect
				property bool highlighted: controlsContainer.direction == "Right"
				anchors.fill: moveRightImg
				source: moveRightImg
				rotation: 90
				opacity: controlsContainer.enabled ? (highlighted ? 1.0 : 0.6) : 0.5
				color: controlsContainer.enabled ? (highlighted ? Theme.highlightColor : Theme.primaryColor) : Theme.secondaryColor
			}
		}

		MouseArea {
			anchors.fill: parent
			preventStealing: true

			function checkPosition() {
				if(controlsContainer.enabled) {
					var relativeX = mouseX - width/2
					var relativeY = mouseY - height/2
					if(relativeY <= -Math.abs(relativeX)) {
						// Upper quarter
						controlsContainer.direction = "Up"
                        playerRect.rotation = 180
					} else if(relativeX >= Math.abs(relativeY)) {
						// Right quarter
						controlsContainer.direction = "Right"
                        playerRect.rotation = 270
                    } else if(relativeY >= Math.abs(relativeX)) {
						// Bottom quarter
						controlsContainer.direction = "Down"
                        playerRect.rotation = 0
                    } else {
						// Left quarter
						controlsContainer.direction = "Left"
                        playerRect.rotation = 90
                    }
				}
			}

			onMouseXChanged: checkPosition()
			onMouseYChanged: checkPosition()
			onReleased: {
				if(controlsContainer.direction && rootContainer.state == "default" && player.currentTurn) {
					if(player.move(controlsContainer.direction))
						rootContainer.state = "moving" + controlsContainer.direction
					else {
						var pos = ~~(size / 2)

						if(controlsContainer.direction === "Up") {
							if(walls.split("\n").indexOf(pos + "\\" + pos + "\\t") < 0) {
								walls += pos + "\\" + pos + "\\t\n"
							}
						} else if(controlsContainer.direction === "Down") {
							if(walls.split("\n").indexOf(pos + "\\" + (pos + 1) + "\\t") < 0) {
								walls += pos + "\\" + (pos + 1) + "\\t\n"
							}
						} else if(controlsContainer.direction === "Left") {
							if(walls.split("\n").indexOf(pos + "\\" + pos + "\\l") < 0) {
								walls += pos + "\\" + pos + "\\l\n"
							}
						} else if(controlsContainer.direction === "Right")
							if(walls.split("\n").indexOf((pos + 1) + "\\" + pos + "\\l") < 0) {
								walls += (pos + 1) + "\\" + pos + "\\l\n"
							}
					}
					controlsContainer.direction = ""
				}
			}

			onDoubleClicked: console.log(walls)
		}
	}
	// Now here're some dirty machinations with opacity effects
	OpacityRampEffect {
		id: eff1
		sourceItem: labyrinth
		direction: OpacityRamp.LeftToRight
		offset: 1 - (1 / size)
		slope: size + 1
	}
	OpacityRampEffect {
		id: eff2
		sourceItem: eff1
		direction: OpacityRamp.TopToBottom
		offset: 1 - (1 / size)
		slope: size + 1
	}
	OpacityRampEffect {
		id: eff3
		sourceItem: eff2
		direction: OpacityRamp.RightToLeft
		offset: 1 - (1/ size)
		slope: size + 1
	}
	OpacityRampEffect {
		id: eff4
		sourceItem: eff3
		direction: OpacityRamp.BottomToTop
		offset: 1 - (1 / size)
		slope: size + 1
	}

	Rectangle {
		id: toolbar
		anchors {
			top: labyrinth.bottom
			left: parent.left
			right: parent.right
			bottom: parent.bottom
			margins: Theme.paddingMedium
		}
		color: "transparent"
		border.color: "white"

		Label {
			anchors.centerIn: parent
			text: "Here could be <s>your advertisement</s><br> an inventory or a toolbar"
			textFormat: Text.RichText
		}
	}

	Rectangle {
		id: background
		anchors.fill: parent
		color: "black"
		visible: false
	}

	ShaderEffect {
		z: -1
		opacity: 0.3
		anchors.fill: background
		property variant src: background
		property real r: labyrinth.width / 2
		property real cx: labyrinth.x + labyrinth.width / 2
		property real cy: labyrinth.y + labyrinth.height / 2
		property real w: width
		property real h: height

		vertexShader: "
			uniform highp mat4 qt_Matrix;
			attribute highp vec4 qt_Vertex;
			attribute highp vec2 qt_MultiTexCoord0;
			varying highp vec2 coord;
			void main() {
				coord = qt_MultiTexCoord0;
				gl_Position = qt_Matrix * qt_Vertex;
			}"
		fragmentShader: "
			varying highp vec2 coord;
			varying lowp float alfa;
			uniform sampler2D src;
			uniform lowp float r;
			uniform lowp float cx;
			uniform lowp float cy;
			uniform lowp float w;
			uniform lowp float h;
			uniform lowp float qt_Opacity;
			void main() {
				lowp float alfa = (pow(coord.x*w-cx,2.0)+pow(coord.y*h-cy,2.0)) / (r*r);
				if(alfa > 1.0)
					alfa = 1.0;
				gl_FragColor = texture2D(src, coord) * qt_Opacity * alfa;
			}"
	}
}
