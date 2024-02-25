import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

ApplicationWindow  {
    visibility: Window.Maximized
    visible: true
    title: qsTr("Flow Book Data Helper")
    Shortcut {
        sequence: StandardKey.Open
        onActivated: {
            // Handle Ctrl+O shortcut activation
            console.log("Ctrl+O pressed");
            fileDialog.open()
        }
    }

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Open"
                onTriggered: {
                    fileDialog.open()
                }
            }
        }
    }

    Rectangle {
        FileDialog {
            id: fileDialog
            title: "Open File"
            nameFilters: ["All Files (*)"]

            onAccepted: {
                // Handle accepted file
                console.log("You chose: " + fileDialog.selectedFile)
            }

            onRejected: {
                // Handle rejected file dialog
                console.log("File dialog rejected");
            }
        }

        property var rects: []
        property real lastW: 100
        property real lastH: 50
        property var selectedSource
        id: root
        anchors.fill: parent
        color: "lightblue"
        Image {
            id: name
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            source: fileDialog.selectedFile// "file:./15.png"
            fillMode: Image.PreserveAspectFit
            onSourceChanged: {
                root.reset()
            }

        }

        MouseArea {
            anchors.fill: parent
            pressAndHoldInterval: 300
            onPressAndHold: {
                var component = Qt.createComponent("MyRect.qml");
                var lastW
                if(root.rects.length > 0) {
                    root.lastW = root.rects[root.rects.length-1].width
                    root.lastH = root.rects[root.rects.length-1].height
                }

                var newRect = component.createObject(parent, {"x": mouseX, "y": mouseY,
                                                         "width": root.lastW, "height": root.lastH})

                root.rects.push(newRect)
            }
        }

        TextArea {
            id: textArea
            anchors.right: parent.right
            anchors.margins: 3
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            font.pointSize: 20
            width: 500
            wrapMode: TextEdit.Wrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: "black"
            opacity: 0.4
        }

        Row {
            anchors.bottom: pageRow.top
            anchors.horizontalCenter: textArea.horizontalCenter
            Text {
                text: qsTr("Activity: ")
            }
            ComboBox {
                id: activityBox
                width: 200
                model: ["dragdroppicture", "fillpicture", "circle", "matchTheWords", "rightOrWrongWithColors"]
                currentIndex: 0
            }

            Button {
                text: "generate"
                onClicked: {
                    var xRatio = name.sourceSize.width / name.paintedWidth
                    var yRatio = name.sourceSize.height / name.paintedHeight
                    var bufX = name.width /2 - name.paintedWidth/2
                    var bufY = name.height /2 - name.paintedHeight/2
                    var total = "\"activity\": {"

                    if(activityBox.currentText === "dragdroppicture") {
                        total += "\"type\":\"dragdroppicture\","
                        total += "\"words\":["

                        var shuffledArr = []
                        for(var y = 0; y < root.rects.length; y++) {
                            shuffledArr.push(root.rects[y].text)
                        }

                        for (var i1 = shuffledArr.length - 1; i1 > 0; i1--) {
                            var j1 = Math.floor(Math.random() * (i1 + 1));
                            // Swap originalArray[i1] and originalArray[j1]
                            var temp = shuffledArr[i1];
                            shuffledArr[i1] = shuffledArr[j1];
                            shuffledArr[j1] = temp;
                        }

                        for(var i = 0; i < shuffledArr.length; i++) {
                            total += "\""+ shuffledArr[i] + "\""
                            if( i !== shuffledArr.length-1) {
                                total += ","
                            }
                        }
                        total += "],"
                        var sectionPath = fileDialog.selectedFile.toString();
                        var index =  sectionPath.indexOf("books")
                        sectionPath = sectionPath.substring(index)
                        print(sectionPath)
                        total += "\"section_path\":\"./"+ sectionPath + "\","
                        total += "\"answer\": ["

                        for(var j = 0; j < root.rects.length; j++) {
                            var originalX = parseInt(root.rects[j].x * xRatio - bufX)
                            var originalY = parseInt(root.rects[j].y * yRatio - bufY)

                            var originalW = parseInt(root.rects[j].width * xRatio)
                            var originalH = parseInt(root.rects[j].height * yRatio)

                            total += "{ \"coords\":{"
                            total += "\"x\":" + originalX + ","
                            total += "\"y\":" + originalY + ","
                            total += "\"w\":" + originalW + ","
                            total += "\"h\":" + originalH
                            total +="},"
                            total += "\"text\":\"" + root.rects[j].text + "\""
                            total += "}"

                            if( j !== root.rects.length-1) {
                                total += ","
                            }
                        }
                        total += "]"
                        total += "}"
                    }
                    else if(activityBox.currentText === "fillpicture") {
                        total += "\"type\":\"fillpicture\","
                        var sectionPath = fileDialog.selectedFile.toString();
                        var index =  sectionPath.indexOf("books")
                        sectionPath = sectionPath.substring(index)
                        print(sectionPath)
                        total += "\"section_path\":\"./"+ sectionPath + "\","
                        total += "\"answer\": ["

                        for(var j = 0; j < root.rects.length; j++) {
                            var originalX = parseInt(root.rects[j].x * xRatio - bufX)
                            var originalY = parseInt(root.rects[j].y * yRatio - bufY)

                            var originalW = parseInt(root.rects[j].width * xRatio)
                            var originalH = parseInt(root.rects[j].height * yRatio)

                            total += "{ \"coords\":{"
                            total += "\"x\":" + originalX + ","
                            total += "\"y\":" + originalY + ","
                            total += "\"w\":" + originalW + ","
                            total += "\"h\":" + originalH
                            total +="},"
                            total += "\"text\":\"" + root.rects[j].text + "\""
                            total += "}"

                            if( j !== root.rects.length-1) {
                                total += ","
                            }
                        }
                        total += "]"
                        total += "}"
                    }
                    else if(activityBox.currentText === "circle") {
                        total += "\"type\":\"circle\","
                        var sectionPath = fileDialog.selectedFile.toString();
                        var index =  sectionPath.indexOf("books")
                        sectionPath = sectionPath.substring(index)
                        print(sectionPath)
                        total += "\"section_path\":\"./"+ sectionPath + "\","
                        total += "\"answer\": ["

                        for(var j = 0; j < root.rects.length; j++) {
                            var originalX = parseInt(root.rects[j].x * xRatio - bufX)
                            var originalY = parseInt(root.rects[j].y * yRatio - bufY)

                            var originalW = parseInt(root.rects[j].width * xRatio)
                            var originalH = parseInt(root.rects[j].height * yRatio)

                            total += "{ \"coords\":{"
                            total += "\"x\":" + originalX + ","
                            total += "\"y\":" + originalY + ","
                            total += "\"w\":" + originalW + ","
                            total += "\"h\":" + originalH
                            total +="},"
                            total += "\"isCorrect\":true"
                            total += "}"

                            if( j !== root.rects.length-1) {
                                total += ","
                            }
                        }
                        total += "]"
                        total += "}"
                    }
                    else if(activityBox.currentText === "rightOrWrongWithColors") {
                        total += "\"type\":\"rightOrWrongWithColors\","
                        var sectionPath = fileDialog.selectedFile.toString();
                        var index =  sectionPath.indexOf("books")
                        sectionPath = sectionPath.substring(index)
                        print(sectionPath)
                        total += "\"section_path\":\"./"+ sectionPath + "\","
                        total += "\"answer\": ["

                        for(var j = 0; j < root.rects.length; j++) {
                            var originalX = parseInt(root.rects[j].x * xRatio - bufX)
                            var originalY = parseInt(root.rects[j].y * yRatio - bufY)

                            var originalW = parseInt(root.rects[j].width * xRatio)
                            var originalH = parseInt(root.rects[j].height * yRatio)

                            total += "{ \"coords\":{"
                            total += "\"x\":" + originalX + ","
                            total += "\"y\":" + originalY + ","
                            total += "\"w\":" + originalW + ","
                            total += "\"h\":" + originalH
                            total += "},"
                            if(root.rects[j].text.length > 0) {
                                total += "\"isCorrect\":true"
                            } else {
                                total += "\"isCorrect\":false"
                            }

                            total += "}"

                            if( j !== root.rects.length-1) {
                                total += ","
                            }
                        }
                        total += "]"
                        total += "}"
                    }

                    textArea.text = total
                }

            }
        }

        Row {
            id: pageRow
            anchors.bottom: clearButton.top
            anchors.horizontalCenter: textArea.horizontalCenter
            Text {
                text: qsTr("Page: ")
            }
            ComboBox {
                id: comboBox
                width: 200
                model: ["Sound", "Fill", "Circle", "freeTextField", "multipleChoiceTest"]
                currentIndex: 1
            }
            Button {
                id: generateButton
                text: "generate"
                onClicked: {

                    var xRatio = name.sourceSize.width / name.paintedWidth
                    var yRatio = name.sourceSize.height / name.paintedHeight
                    var bufX = name.width /2 - name.paintedWidth/2
                    var bufY = name.height /2 - name.paintedHeight/2
                    var total = "{";

                    if(comboBox.currentText === "Fill") {
                        total += "\"type\": \"fill\","
                        total += "\"answer\": ["
                    } else if(comboBox.currentText === "Circle") {
                        total += "\"type\": \"circle\","
                        total += "\"answer\": ["
                    } else if(comboBox.currentText === "freeTextField") {
                        total += "\"type\": \"freeTextField\","
                        total += "\"freeTextFields\": ["
                    } else if(comboBox.currentText === "multipleChoiceTest") {
                        total += "\"type\": \"multipleChoiceTest\","
                        total += "\"answer\": ["
                    }

                    for(var i = 0; i <root.rects.length; i++) {
                        var originalX = parseInt(root.rects[i].x * xRatio - bufX)
                        var originalY = parseInt(root.rects[i].y * yRatio - bufY)

                        var originalW = parseInt(root.rects[i].width * xRatio)
                        var originalH = parseInt(root.rects[i].height * yRatio)

                        if(comboBox.currentText === "Fill") {
                            total += "{ \"coords\":{"
                            total += "\"x\":" + originalX + ","
                            total += "\"y\":" + originalY + ","
                            total += "\"w\":" + originalW + ","
                            total += "\"h\":" + originalH
                            total +="},"
                            total += "\"text\":\"" + root.rects[i].text + "\""
                            total += "}"

                            if( i !== root.rects.length-1) {
                                total += ","
                            }

                        } else if (comboBox.currentText === "Circle"){
                            total += "{ \"coords\":{"
                            total += "\"x\":" + originalX + ","
                            total += "\"y\":" + originalY + ","
                            total += "\"w\":" + originalW + ","
                            total += "\"h\":" + originalH
                            total +="},"
                            total += "\"isCorrect\":true"
                            total += "}"

                            if( i !== root.rects.length-1) {
                                total += ","
                            }
                        } else if (comboBox.currentText === "Sound"){

                            var sectionPath = fileDialog.selectedFile.toString();
                            var index =  sectionPath.indexOf("books")
                            sectionPath = sectionPath.substring(index)
                            var pathParts = sectionPath.split('/')

                            var bookName = pathParts[1]

                            total += "\"type\": \"audio\","
                            total += "\"audio_path\":\"./books/ " + bookName+ "/audio/"+root.rects[i].text + ".mp3\","

                            total += "\"coords\":{"
                            total += "\"x\":" + originalX + ","
                            total += "\"y\":" + originalY + ","
                            total += "\"w\":" + originalW + ","
                            total += "\"h\":" + originalH
                            total +="}"
                            total +="}"
                        } else if (comboBox.currentText === "freeTextField") {
                            total += "{ \"coords\":{"
                            total += "\"x\":" + originalX + ","
                            total += "\"y\":" + originalY + ","
                            total += "\"w\":" + originalW + ","
                            total += "\"h\":" + originalH
                            total +="}"
                            total += "}"

                            if( i !== root.rects.length-1) {
                                total += ","
                            }
                        } else if (comboBox.currentText === "multipleChoiceTest") {
                                total += "{ \"coords\":{"
                                total += "\"x\":" + originalX + ","
                                total += "\"y\":" + originalY + ","
                                total += "\"w\":" + originalW + ","
                                total += "\"h\":" + originalH
                                total +="},"
                                if(root.rects[i].text.length > 0) {
                                    total += "\"isCorrect\":true"
                                } else {
                                    total += "\"isCorrect\":false"
                                }
                                total += "}"
                                if( i !== root.rects.length-1) {
                                    total += ","
                                }
                        }
                    }
                    if(comboBox.currentText === "Fill" || comboBox.currentText === "Circle" || comboBox.currentText === "freeTextField" || comboBox.currentText === "multipleChoiceTest") {
                        total += "]"
                        total += "}"
                    }
                    textArea.text = total
                }
            }
        }

        Button {
            id: clearButton
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: textArea.horizontalCenter
            text: "Clear"
            onClicked: {
                root.reset()
                textArea.text = ""
            }
        }

        function reset() {
            for(var i = 0 ; i < root.rects.length; i++) {
                if(root.rects[i])
                    root.rects[i].destroy()
            }
            root.rects = []
        }

    }


}
