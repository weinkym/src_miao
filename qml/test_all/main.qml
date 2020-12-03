import QtQuick 2.12
import QtQuick.Window 2.12
import QtWebView 1.14
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "./heart" as ZHeart

ApplicationWindow {
    visible: true
    width: 840
    height: 680
    title: qsTr("Hello World")


    header: TabBar {
        id: bar
        width: parent.width
        TabButton {
            text: qsTr("ZTrain")
        }
        TabButton {
            text: qsTr("WebView")
        }
        TabButton {
            text: qsTr("ListView")
        }
        TabButton {
            text: qsTr("others")
        }
    }

    StackLayout {
        //        width: parent.width
        currentIndex: bar.currentIndex
        anchors.fill: parent

        ZTrain{
            x:100
            y:10
            width:600
            height: 120
            running: true
        }
        WebView{
            url:"https://www.baidu.com/"
            width: 800
            height: 300
            y:300
            onLoadingChanged: {
                console.log(loadRequest.url.toString() + loadRequest.status)

            }
        }
        ListView{
            width: 100
            height: 320
            model: ListModel{
                id:id_listModel
                Component.onCompleted: {
                    for (var i = 0; i < 10; i++)
                    {
                        id_listModel.append({"Name":"item"+i})
                    }
                }
            }
            delegate:Text{
                text: index + "=====" +Name
                color:ListView.isCurrentItem  ? "green":"red"
            }
        }
        Item {
            RowLayout{
                ZHeart.HollowHeart{

                }
                ZHeart.SolidHeart{

                }
                ZHeart.CircleHeart{

                }
            }
        }
    }
}
