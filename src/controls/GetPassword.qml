import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.mauikit.controls as Maui
import org.kde.novarewind 1.0

Maui.Page {
    id: getPasswordPage

    Maui.Controls.showCSD: true

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
            Layout.preferredWidth: parent.width
            font.pixelSize: 28
            text: "Verify your identity"
        }
        Label {
            Layout.preferredWidth: parent.width
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            text: "You must enter an administrator password to verify that you have privileges to complete certain tasks"
        }
        TextField {
            id: textField
            Layout.topMargin: 25
            echoMode: TextInput.Password
            onAccepted: {
                password = text
                Snapshot.setPassword(password)
                if (Snapshot.getMode() == "create") {
                    stackView.push("Create.qml")
                }
                else if (Snapshot.getMode() == "restore") {
                    stackView.push("Restore.qml")
                }
                else if (Snapshot.getMode() == "remove") {
                    stackView.push("Remove.qml")
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
                stackView.push("Create.qml")
            }
            else if (Snapshot.getMode() == "restore") {
                stackView.push("Restore.qml")
            }
            else if (Snapshot.getMode() == "remove") {
                stackView.push("Remove.qml")
            }
        }
    }
}
