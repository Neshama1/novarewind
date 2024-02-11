import QtQuick 2.15
import QtQuick.Controls 2.15
import org.mauikit.controls 1.3 as Maui
import QtQuick.Layouts 1.15
import QtQml 2.15
import org.kde.novarewind 1.0

Maui.Page {
    id: restoreResultPage

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
    }

    PropertyAnimation {
        id: xAnimation
        target: restoreResultPage
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

        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 28
            text: "Success"
        }
        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            text: "A new point has been restored successfully"
        }
    }

    Rectangle {
        id: snapshotRect
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

    Label {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 20
        font.pixelSize: 16
        text: "You must restart your computer for the changes to take effect"
    }

    Maui.IconItem
    {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/pixabay-login-8396701_1280.png"
        imageSizeHint: 400
        opacity: 0.70
        scale: 0.80
    }
}
