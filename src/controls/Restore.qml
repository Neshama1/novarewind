import QtQuick 2.15
import QtQuick.Controls 2.15
import org.mauikit.controls 1.3 as Maui
import QtQuick.Layouts 1.15
import QtQml 2.15
import org.kde.novarewind 1.0

Maui.Page {
    id: restorePage

    showCSDControls: true

    headBar.background: Rectangle {
        anchors.fill: parent
        Maui.Theme.inherit: false
        Maui.Theme.colorSet: Maui.Theme.View
        color: Maui.Theme.backgroundColor
    }

    headBar.leftContent: ToolButton
    {
        icon.name: "draw-arrow-back"
        onClicked: {
            stackView.push("qrc:/Home.qml")
        }
    }

    Component.onCompleted: {
        xAnimation.start()
        getSnapshots()
    }

    function getSnapshots() {
        snapshotModel.clear()
        Snapshot.listSnapshots()
        for (var i = 0; i < Snapshot.points.length; i++) {
            snapshotModel.append({"point": Snapshot.points[i].dateTime,"type": Snapshot.points[i].type})
        }
    }

    Connections {
        target: Snapshot
        onFailed: {
            password = ""
            labelTitle.text = "Oops!"
            labelDescription.text = "You should try entering your password again"
        }
    }

    PropertyAnimation {
        id: xAnimation
        target: restorePage
        properties: "x"
        from: parent.width
        to: 0
        duration: 2000
        easing.type: Easing.OutExpo
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: list.top
        anchors.top: parent.top
        color: Maui.Theme.backgroundColor
        z: 1
    }

    ColumnLayout {
        id: labelColumn
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20
        z: 2

        Label {
            id: labelTitle
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 28
            text: "Restore system to a previous state"
        }
        Label {
            id: labelDescription
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            text: "Choose a point to restore"
        }
    }

    Maui.ListBrowser {
        id: list
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: labelColumn.bottom
        anchors.bottom: parent.bottom
        anchors.margins: 20

        horizontalScrollBarPolicy: ScrollBar.AsNeeded
        verticalScrollBarPolicy: ScrollBar.AsNeeded

        spacing: 10

        model: snapshotModel

        delegate: Rectangle {
            color: "transparent"
            width: ListView.view.width
            height: 50
            Rectangle {
                anchors.fill: parent
                anchors.margins: 0
                color: Maui.Theme.alternateBackgroundColor
                radius: 4

                Label {
                    id: snapshotLabel
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pixelSize: 16
                    text: "Restore point: " + point
                }

                Maui.Badge
                {
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 10
                    property string snapshotType: type
                    visible: type == "daily" || type == "weekly" || type == "monthly" ? true : false
                    text: type == "daily" || type == "weekly" || type == "monthly" ? snapshotType.charAt(0).toUpperCase() : ""
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    restoreIndex = index
                    stackView.push("qrc:/RestoreConfirmation.qml")
                }
            }
        }
    }
}
