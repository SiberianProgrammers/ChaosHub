import QtQuick 2.14
import Components 1.0
import Sp 1.0

/*******************************************************************************
 * Список записей по заданному тегу.
 ******************************************************************************/
Item {
    id: _recordsList

    property alias tag: recordModel.tag

    RecordModel {
        id: recordModel

        tag: "root" // Debug!!! Временный флаг
    }

    ListView {
        id: listView

        delegate: TextNote {}
        model: recordModel
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }
}