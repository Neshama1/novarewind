import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml
import org.mauikit.controls as Maui
import org.kde.novarewind 1.0

Maui.Page {
    id: removePage

    Maui.Controls.showCSD: true

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
            stackView.push("Home.qml")
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
        target: removePage
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
            text: "Remove a restore point"
        }
        Label {
            id: labelDescription
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            text: "Choose a restore point to delete"
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
                    removeIndex = index
                    stackView.push("RemoveConfirmation.qml")
                }
            }
        }
    }
}
