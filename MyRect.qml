import QtQuick
import QtQuick.Controls
Rectangle {
    property var text: textInput.text
    id: root
    width: 100
    height: 50
    color: "red"
    opacity: .4

    MouseArea {
        anchors.fill: parent
        drag.target: root
        onDoubleClicked: {
            textInput.visible = true
            textInput.focus = true
        }
    }

    Rectangle {
        id: resizeImg
        width: 10
        height: 10
        radius: height/2
        color: "blue"
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        MouseArea {
            anchors.fill: parent
            drag{ target: parent; axis: Drag.XAndYAxis }
            onPositionChanged: {
                if(drag.active){
                    root.width = root.width + mouseX
                    if(root.width < 20)
                        root.width = 20

                    root.height = root.height + mouseY
                    if(root.height < 10)
                        root.height = 10
                }
            }

        }
    }

    Rectangle {
        id: close
        width: 10
        height: 10
        radius: height/2
        color: "black"
        anchors.top: parent.top
        anchors.right: parent.right

        MouseArea {
            anchors.fill: parent
            onDoubleClicked: {
                root.destroy()
            }
        }
    }

    TextArea {
        id: textInput
        anchors.fill: parent
        anchors.margins: 3
        width: 100
        wrapMode: TextEdit.Wrap
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        visible: false
        color: "black"
    }

}
