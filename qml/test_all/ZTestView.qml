import QtQuick 2.12
import QtQuick.Controls 2.14

Item {
    id: container
    property var objList:[]

    width: 500
    height: 500
    Button{
        id:id_test
        width: 60
        height: 30
        visible: true
        text:"TEST"
        background: Rectangle{
            color:"red"
            anchors.fill: parent
        }
        onClicked: {
            var component = Qt.createComponent("ZTrain.qml");
            for (var i=0; i<5; i++) {
                var object = component.createObject(container,{"x":100,
            "y":50,
            "width":600,
            "height": 120,
            "running": false});
                object.y = (object.height + 10) * i;
                container.objList.push(object)
            }
            console.log(container.objList.length)
        }
    }
    Button{
        x:70
        width: 60
        height: 30
        visible: true
        text:"Delete"
        background: Rectangle{
            color:"red"
            anchors.fill: parent
        }
        onClicked: {
            while(container.objList.length)
            {
                var object = container.objList.pop()
                object.destroy()
            }
            console.log(container.objList.length)
        }
    }
}
