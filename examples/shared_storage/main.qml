/*
 * SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
 * Author: BogDan Vatra <bogdan.vatra@kdab.com>
 * SPDX-License-Identifier: MIT
*/
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Very simple Android shared storage example")
    header: ToolBar {
        RowLayout {
            spacing: 5
            ToolButton {
                text: "New file dialog"
                onClicked: __controller.newFile(newFileName.text)
            }
            ToolButton {
                text: "Open file dialog"
                onClicked: __controller.openFile()
            }
            ToolButton {
                text: "Open folder dialog"
                onClicked: __controller.openDir()
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: false
            spacing: 10
            RowLayout {
                spacing: 10
                Label {
                    text: "New file name:"
                }
                TextField {
                    Layout.fillWidth: true
                    id: newFileName
                    text: "file.txt"
                }
            }

            RowLayout {
                spacing: 5
                Label {
                    wrapMode: Text.Wrap
                    text: "Opened document :" + __controller.fileName
                }
                Item {
                    Layout.fillWidth: true
                }
                Button {
                    enabled: !__controller.hasTreeUri
                    text: "Remove file"
                    onClicked: __controller.remove()
                }
            }

            RowLayout {
                Label {
                    text: "File content:"
                }
                Item {
                    Layout.fillWidth: true
                }
                Button {
                    enabled: __controller.hasSingleUri
                    text: "Save content"
                    onClicked: __controller.saveContent(fileContent.text)
                }
            }
            TextArea {
                Layout.fillWidth: true
                Layout.fillHeight: true
                id: fileContent
                text: __controller.fileContent
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            enabled:  __controller.hasTreeUri
            spacing: 10
            Label {
                text: "Folder content"
            }

            RowLayout {
                Layout.fillHeight: false
                Layout.fillWidth: true
                spacing: 20
                Button {
                    enabled: __controller.hasParent
                    text: "Up"
                    onClicked: {
                        folderContent.currentIndex = -1;
                        __controller.cdUp();
                    }
                }
                Button {
                    text: "New file"
                    onClicked: __controller.newTreeFile(newFileName.text)
                }
                Button {
                    text: "New Folder"
                    onClicked: __controller.newTreeFolder(newFileName.text)
                }
                Item {
                    Layout.fillHeight: true
                }
            }

            ListView {
                id: folderContent

                Layout.fillHeight: true
                Layout.fillWidth: true

                clip: true
                focus: true
                highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                model: __controller

                delegate: RowLayout {
                    width: folderContent.width
                    height: 64
                    Label {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.fillWidth: true
                        text: name + "\nSize " + size + "\nMine type " + mime
                    }

                    Button {
                        text: "Open " +  (type === "dir" ? "folder" : "file")
                        onClicked: {
                            folderContent.currentIndex = type === "file" ? index : -1;
                            __controller.openTreeItem(index);
                        }
                    }
                    Button {
                        text: "Remove"
                        onClicked: __controller.removeTreeItem(index)
                    }
                    Button {
                        text: "Rename"
                        onClicked: __controller.renameTreeItem(index, newFileName.text)
                    }
                }
            }
        }
    }
}
