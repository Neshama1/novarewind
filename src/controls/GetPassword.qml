import QtQuick 2.15
import QtQuick.Controls 2.15
import org.mauikit.controls 1.3 as Maui
import QtQuick.Layouts 1.15
import org.kde.novarewind 1.0

Maui.Page {
    id: getPasswordPage

    showCSDControls: true

    headBar.background: Rectangle {
        anchors.fill: parent
        Maui.Theme.inherit: false
        Maui.Theme.colorSet: Maui.Theme.View
        color: Maui.Theme.backgroundColor
    }

    Component.onCompleted: {
        xAnimation.start()
    }

    PropertyAnimation {
        id: xAnimation
        target: getPasswordPage
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
            text: "Verify your identity"
        }
        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            text: "You must enter an administrator password to verify that you have privileges to complete certain tasks"
        }
        TextField {
            id: textField
            Layout.topMargin: 25
            onAccepted: {
                password = text
                Snapshot.setPassword(password)
                if (Snapshot.getMode() == "create") {
                    stackView.push("qrc:/Create.qml")
                }
                else if (Snapshot.getMode() == "restore") {
                    stackView.push("qrc:/Restore.qml")
                }
                else if (Snapshot.getMode() == "remove") {
                    stackView.push("qrc:/Remove.qml")
                }
            }
        }
    }

    Maui.IconItem
    {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: labelColumn.bottom
        anchors.bottom: parent.bottom
        anchors.leftMargin: 50
        imageSource: "qrc:/pixabay-phishing-6573326_1280.png"
        opacity: 0.70
        scale: 0.80
    }

    RoundButton {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 40
        //width: 50
        height: 50
        text: "Go to next page"
        onClicked: {
            password = textField.text
            Snapshot.setPassword(password)
            if (Snapshot.getMode() == "create") {
                stackView.push("qrc:/Create.qml")
            }
            else if (Snapshot.getMode() == "restore") {
                stackView.push("qrc:/Restore.qml")
            }
            else if (Snapshot.getMode() == "remove") {
                stackView.push("qrc:/Remove.qml")
            }
        }
    }
}
