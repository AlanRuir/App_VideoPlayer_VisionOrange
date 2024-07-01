import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow 
{
    visible: true
    width: 1920
    height: 1080
    title: qsTr("视频播放器")
    flags: Qt.Window | Qt.WindowMinimizeButtonHint | Qt.WindowCloseButtonHint

    Image 
    {
        id: videoOutput
        anchors.fill: parent
        source: "image://video/currentFrame"
        fillMode: Image.PreserveAspectFit      
        sourceSize.width: videoOutput.width    
        sourceSize.height: videoOutput.height   

        anchors 
        {
            top: parent.top     
            left: parent.left
            topMargin: 0       
            leftMargin: 0      
        }
    }

    Component.onCompleted: 
    {
        videoPlayer.play("rtsp://127.0.0.1:554/live/test")
    }

    Connections 
    {
        target: videoPlayer
        function onFrameReady() 
        {
            videoOutput.source = "image://video/currentFrame?" + Date.now();
        }
    }
}