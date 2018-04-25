import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import com.kdab.android 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("SurfaceTexture example")
    GridLayout {
        columns: 2
        anchors.fill: parent
        Repeater {
            model: 4
            SurfaceTexture {
                id: surfaceTexture
                Layout.fillWidth: true
                Layout.fillHeight: true

                AndroidMediaPlayer {
                    id: player
                    videoOut: surfaceTexture
                }

                Text {
                    id: info
                    anchors.centerIn: parent
                    color: "white"
                    text: "Tap to start the player"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked:
                        if (info.visible) {
                            info.visible = false;
                            player.playFile("http://download.blender.org/peach/bigbuckbunny_movies/BigBuckBunny_320x180.mp4");
                        }
                }
            }
        }
    }
}
