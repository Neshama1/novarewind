import QtQuick 2.15
import QtQuick.Controls 2.15
import org.mauikit.controls 1.3 as Maui
import QtQuick.Layouts 1.15
import QtQml 2.15
import org.kde.novarewind 1.0

Maui.Page {
    id: createPage

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
        Snapshot.createSnapshot(getMntPoint())
    }

    PropertyAnimation {
        id: xAnimation
        target: createPage
        properties: "x"
        from: parent.width
        to: 0
        duration: 2000
        easing.type: Easing.OutExpo
    }

    function getMntPoint() {
        return `findmnt / --output=SOURCE | grep "[/@]" | awk '{gsub(/\\[/,"");}1' | sed 's;/@;;' | awk '{gsub(/\\]/,"");}1'`
    }

    function getDateTime() {
        return Qt.formatDateTime(new Date(), "yyyyMMdd-hhmmss")
    }

    Connections {
        target: Snapshot
        onFailed: {
            password = ""
            labelTitle.text = "Oops!"
            labelDescription.text = "You should try entering your password again"
            snapshotRect.visible = false
            banner.visible = false
        }
        onFinished: {
            labelTitle.text = "Success"
            labelDescription.text = "A new restore point has been created successfully"
            snapshotRect.visible = true
            banner.visible = true
            snapshotLabel.text = "Restore point: " + data
        }
    }

    ColumnLayout {
        id: labelColumn
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20

        Label {
            id: labelTitle
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 28
        }
        Label {
            id: labelDescription
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 16
            wrapMode: Text.WordWrap
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
        }
    }

    Maui.IconItem
    {
        id: banner
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/pixabay-login-8396701_1280.png"
        imageSizeHint: 400
        opacity: 0.70
        scale: 0.80
    }
}
