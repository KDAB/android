import QtQuick 2.5
import QtQuick.Controls 1.4
import com.kdab.android 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("SurfaceTexture example")

    SurfaceTexture {
        id: videoItem
        anchors.fill: parent

        // Set media player's video out
        Component.onCompleted: _mediaPlayer.videoOut = videoItem;

        MouseArea {
            anchors.fill: parent
            onClicked: _mediaPlayer.playFile("/sdcard/testfile.mp4");
        }
    }
}
