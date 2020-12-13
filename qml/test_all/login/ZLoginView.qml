import QtQuick 2.12
//import QtQuick.Window 2.12
import QtQuick 2.12
import QtWebView 1.14
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    id:id_root
    visible: true
    width: 520
    height: 473
    flags: Qt.FramelessWindowHint
    title: qsTr("Hello World")
    background: Rectangle{
        color:"#e6000000"
        radius: 10
    }
    color:"transparent"

    Button{
        id:id_close
        background:Rectangle{
            color:"#00000000"
        }
        width: id_button_image.width
        height: id_button_image.height
        anchors {right: parent.right;}
        Image{
            id:id_button_image
            source:  id_close.hovered ? "qrc:/login/images/close_hover.png" : "qrc:/login/images/close_normal.png"
        }
        onClicked: Qt.quit()
    }

    RowLayout{
        id:id_login_icon
        anchors.top: id_close.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        Image {
            source: "qrc:/login/images/ljlive_icon.png"
        }
        Image {
            source: "qrc:/login/images/ljlive_text.png"
        }
    }

    Rectangle{
        id:id_account
        anchors { left: parent.left; right: parent.right}
        anchors.top: id_login_icon.bottom
        anchors.horizontalCenter: id_root.horizontalCenter
        anchors.topMargin: 30
        anchors.leftMargin: 50
        anchors.rightMargin: 50
        color:"#15ffffff"
        border.color: "#35ffffff"
        border.width:1
        radius:5
        //       width: id_root.width
        height: 183
        ColumnLayout{
            anchors.fill: parent
            spacing: 0
            anchors.topMargin: 1
            ZLoginLineContent {
                Layout.fillWidth: true
                height: 60
                source: "qrc:/login/images/roomid.png"
            }
            Rectangle{
                Layout.fillWidth: true
                height: 1
                anchors.leftMargin: 1
                anchors.rightMargin: 1
                color:"#35ffffff"
            }

            ZLoginLineContent {
                Layout.fillWidth: true

                height: 60
                source: "qrc:/login/images/user.png"
            }
            Rectangle{
                Layout.fillWidth: true
                height: 1
                anchors.leftMargin: 1
                anchors.rightMargin: 1
                color:"#35ffffff"
            }
            ZLoginLineContent {
                Layout.fillWidth: true
                echoMode: TextInput.Password
                height: 60
                source: "qrc:/login/images/password.png"
            }

        }
    }

    RowLayout{
        id:id_login_protocol
        anchors.top: id_account.bottom
        anchors.topMargin: 10
        anchors.leftMargin: 50
        anchors.rightMargin: 50
        spacing: 5
        anchors { left: parent.left; right: parent.right}
        CheckBox{
            id:id_protocol_cb
            text:"Read"
            checked: true
            indicator: Image{
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/login/images/check_checked.png"
            }
            contentItem: Label {
                text: id_protocol_cb.text
                font: id_protocol_cb.font
                color: "#FFFFFF"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                renderType: Text.NativeRendering
                elide: Text.ElideRight
                leftPadding: id_protocol_cb.indicator.width - 2
//                rightPadding: id_protocol_cb.rightPadding
            }

        }
        Text{
            text:"<a class=\"title-content\" href=\"http://stream.xuanyes.com/fixfiles/resource/useragreement.html\"><font color=\"#3070B8\">"
                 + qsTr("User agreement") + "</font></a>"
            linkColor: "#3070B8"
            onLinkActivated: {
                Qt.openUrlExternally(link)
            }
        }
        Item{
            Layout.fillWidth: true
        }

        Rectangle{
            implicitWidth: 92
            implicitHeight: 20
            border.width: 1
            border.color: "#3874B5"
            color:"#00000000"
            RowLayout{
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                spacing: 3
                Label{
                    text: "LianMai"
                    color:"#3874B5"
                }
                Image{
                    source: "qrc:/login/images/lm_login.png"
                    sourceSize.width: 16
                    sourceSize.height: 16
                    smooth: true
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }

    RowLayout{
        id:id_enter
        anchors.top: id_login_protocol.bottom
        anchors.topMargin: 10
        anchors.leftMargin: 50
        anchors.rightMargin: 50
        anchors { left: parent.left; right: parent.right}
        //        height: 150
        Button{
            id:id_button
            implicitHeight: 50
            Layout.fillWidth: true
            text: "enter"
            background: Rectangle{
                color:"#3070B8"
                radius: 5
            }
            contentItem: Text {
                height: 50
                anchors.fill: parent
                anchors.verticalCenter: parent.verticalCenter
                text: id_button.text
                font: id_button.font
                opacity: enabled ? 1.0 : 0.3
                color: id_button.down ? "#17a81a" : "#FFFFFF"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }
    }

    RowLayout{
        anchors.top: id_enter.bottom
        anchors.topMargin: 20
        anchors.leftMargin: 50
        anchors.rightMargin: 50
        anchors { left: parent.left; right: parent.right}
        Item{
            Layout.fillWidth: true
        }
        Label{
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text:"2.1.0(x64)"
            color:"#999999"
            font.family: "Helvetica"
            font.pointSize: 12
        }
        Item{
            Layout.fillWidth: true
        }
    }
}
