import QtQuick 2.0
import QtQuick.Layouts 1.14
//import QtQuick.Controls 1.4
import QtQuick.Controls 2.14

Item {
    property alias source:id_icon.source
    property alias text:id_text.text
    property alias echoMode:id_text.echoMode
    Rectangle{
        color:"#00000000"
        anchors {verticalCenter:parent.verticalCenter}
        anchors.fill: parent
        RowLayout{
            spacing: 0
            anchors { left: parent.left; right: parent.right;verticalCenter:parent.verticalCenter}
            anchors.leftMargin: 10
            Image{
                id:id_icon
            }

            TextField{
                id:id_text
                background: Rectangle {
                    anchors.fill: parent
                    implicitWidth: 200
                    implicitHeight: 40
                    color: "#01000000"
                }
                Layout.fillWidth: true
                anchors.leftMargin: 0
                anchors.rightMargin: 20
                selectByMouse: true
                color: "#ffffff"
                placeholderText:"text"
                font.pixelSize: 16
            }
        }
    }


}
