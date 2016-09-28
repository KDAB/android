import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Connections {
        target : pingPong
        onPong: pongLabel.text = msg
    }
    ColumnLayout {
        Label {
            id: pongLabel
            Layout.fillWidth: true
        }
        TextField {
            Layout.fillWidth: true
            placeholderText : "Type a message"
            onTextChanged: pingPong.ping(text)
        }
        Item {
           Layout.fillHeight: true
        }
    }
}
