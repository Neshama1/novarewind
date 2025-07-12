import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml
import org.mauikit.controls as Maui
import org.kde.novarewind 1.0

Maui.Page {
    id: restoreConfirmationPage

    Maui.Controls.showCSD: true

    headBar.background: Rectangle {
        anchors.fill: parent
        Maui.Theme.inherit: false
        Maui.Theme.colorSet: Maui.Theme.View
        color: Maui.Theme.backgroundColor
    }

    Connections {
        target: Snapshot
        onFailed: {
        }
        onFinished: {
            stackView.push("RestoreResult.qml")
        }
    }

    Component.onCompleted: {
        xAnimation.start()
    }

    PropertyAnimation {
        id: xAnimation
        target: restoreConfirmationPage
        properties: "x"
        from: parent.width
        to: 0
        duration: 2000
        easing.type: Easing.OutExpo
    }

    ColumnLayout {
        id: labelColumn
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20
        z: 2

        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 28
            text: "Are you sure?"
        }
        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            text: "Do you really want to restore this snapshot?"
        }
    }

    Rectangle {
        color: Maui.Theme.alternateBackgroundColor
        radius: 4
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: labelColumn.bottom
        anchors.margins: 20
        height: 50

        Label {
            id: snapshotLabel
            anchors.fill: parent
            anchors.margins: 10
            font.pixelSize: 16
            text: "Restore point: " + Snapshot.points[restoreIndex].dateTime
        }
    }

    RowLayout {
        anchors.centerIn: parent
        RoundButton {
            height: 50
            text: "Cancel operation"
            onClicked: {
                stackView.pop()
            }
        }
        RoundButton {
            height: 50
            text: "Restore snapshot"
            onClicked: {
                Snapshot.restoreSnapshot(Snapshot.points[restoreIndex].dateTime)
            }
        }
    }

    Maui.IconItem
    {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/pixabay-gears-6126071_1280.png"
        imageSizeHint: 400
        opacity: 0.70
        scale: 0.80
    }
}
