import QtQuick 2.15
import QtQuick.Controls 2.15
import org.mauikit.controls 1.3 as Maui
import QtQuick.Layouts 1.15
import org.kde.novarewind 1.0

Maui.Page {
    id: homePage

    showCSDControls: true

    headBar.background: Rectangle {
        anchors.fill: parent
        Maui.Theme.inherit: false
        Maui.Theme.colorSet: Maui.Theme.View
        color: Maui.Theme.backgroundColor
    }

    Column {
        id: labelColumn
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20
        //height: 200

        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 28
            text: "Welcome"
        }
        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            text: "Nova Rewind allows you to create restore points, providing an easy way to revert to a previous state in case of unexpected issues or system malfunctions. It supports the Btrfs file system, creating instant restore points"
        }
    }

    Maui.IconItem
    {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: labelColumn.bottom
        anchors.bottom: buttonsRow.top
        imageSource: "qrc:/pixabay-backend-4496461_1280.png"
        opacity: 0.50
        scale: 0.80
    }

    RowLayout {
        id: buttonsRow
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 50
        height: 200
        spacing: 30
        Rectangle {
            color: Maui.Theme.alternateBackgroundColor
            Layout.fillWidth: true
            Layout.preferredWidth: 200
            Layout.preferredHeight: 200
            radius: 8

            ColumnLayout {
                anchors.fill: parent
                Maui.IconItem
                {
                    Layout.topMargin: 60
                    anchors.left: parent.left
                    anchors.right: parent.right
                    iconSource: "list-add"
                    //imageSizeHint: parent.height
                    maskRadius: Maui.Style.radiusV
                    fillMode: Image.PreserveAspectCrop
                }
                Label {
                    Layout.bottomMargin: 60
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 16
                    wrapMode: Text.WordWrap
                    text: "Create a restore point"
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Snapshot.setMode("create")
                    password == "" ? stackView.push("qrc:/GetPassword.qml") : stackView.push("qrc:/Create.qml")
                }
            }
        }
        Rectangle {
            color: Maui.Theme.alternateBackgroundColor
            Layout.fillWidth: true
            Layout.preferredWidth: 200
            Layout.preferredHeight: 200
            radius: 8

            ColumnLayout {
                anchors.fill: parent
                Maui.IconItem
                {
                    Layout.topMargin: 60
                    anchors.left: parent.left
                    anchors.right: parent.right
                    iconSource: "accept_time_event"
                    //imageSizeHint: parent.height
                    maskRadius: Maui.Style.radiusV
                    fillMode: Image.PreserveAspectCrop
                }
                Label {
                    Layout.bottomMargin: 60
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 16
                    wrapMode: Text.WordWrap
                    text: "Restore to a previous state"
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Snapshot.setMode("restore")
                    password == "" ? stackView.push("qrc:/GetPassword.qml") : stackView.push("qrc:/Restore.qml")
                }
            }
        }
        Rectangle {
            color: Maui.Theme.alternateBackgroundColor
            Layout.fillWidth: true
            Layout.preferredWidth: 200
            Layout.preferredHeight: 200
            radius: 8
            ColumnLayout {
                anchors.fill: parent
                Maui.IconItem
                {
                    Layout.topMargin: 60
                    anchors.left: parent.left
                    anchors.right: parent.right
                    iconSource: "list-remove"
                    //imageSizeHint: parent.height
                    maskRadius: Maui.Style.radiusV
                    fillMode: Image.PreserveAspectCrop
                }
                Label {
                    Layout.bottomMargin: 60
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 16
                    wrapMode: Text.WordWrap
                    text: "Remove a restore point"
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Snapshot.setMode("remove")
                    password == "" ? stackView.push("qrc:/GetPassword.qml") : stackView.push("qrc:/Remove.qml")
                }
            }
        }
    }
}
