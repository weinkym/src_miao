import QtQuick 2.0

Item {
    id:id_root
    clip: true
    property int sx: -id_train.width
    property bool needTimer: true
    property bool running: false
    Rectangle{
        color:"#21ff00aa"
        anchors.fill: parent
        MouseArea{
            anchors.fill: parent
            onClicked: {
                console.log("onClicked")
                if(id_root.needTimer)
                {
                    console.log(id_timer.running)
                    if(id_timer.running)
                    {
                        console.log("stop")
                        id_timer.stop()
                    }
                    else
                    {
                        console.log("start")
                        id_timer.start()
                    }
                }
                else
                {
                    id_animation_train.doClicked()
                }

            }
        }
    }
    Image{
        id:id_train
        source: "qrc:/images/train1.png"
        width:411
        height: 52
        anchors.verticalCenter: parent.verticalCenter
        x:0
    }

    Timer {
        id: id_timer
        interval: 5
        repeat: true
        running: id_root.running && id_root.needTimer
        onTriggered: {
            if(id_train.x > parent.width)
            {
                id_train.x = -id_train.width
            }
            else
            {
                id_train.x = id_train.x + 1
            }
        }
    }

    PropertyAnimation
    {
        id:id_animation_train
        target: id_train
        property: "x"
        from:sx;
        to: parent.width;
        duration: 10000;
        loops: Animation.Infinite
        running: id_root.running && !id_root.needTimer
        onStarted: {
            console.log("onStarted")
        }
        onStopped: {
            console.log("onStopped")
            sx=id_train.x
        }

        function doClicked(){
            console.log("doClicked")
            if(running)
            {
                id_animation_train.stop()
            }
            else
            {
                id_animation_train.start()
            }
        }
    }

}
