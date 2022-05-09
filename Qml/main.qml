import QtQuick
import Board 1.0

Window {
    width: 720
    height: 720
    visible: true
    title: qsTr("Candy Crush")

    Image{
        anchors.fill: parent
        source: "qrc:/img/Background/background2.png"
        fillMode: Image.PreserveAspectCrop
        Board{
            id:board
            width: 720
            height: 720
            anchors.centerIn: parent
            focus : true

            Keys.onPressed: function(event){
                if(event.key === Qt.Key_Space)
                {
                      board.scan();
                }
            }
        }
    }
}
