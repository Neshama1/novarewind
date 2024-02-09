import QtQuick 2.15
import QtQml 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import org.mauikit.controls 1.3 as Maui
import org.kde.novarewind 1.0
import QtQuick.Window 2.15

Maui.ApplicationWindow
{
    id: root
    title: qsTr("")

    property string password
    property int removeIndex
    property int restoreIndex

    width: Screen.desktopAvailableWidth - Screen.desktopAvailableWidth * 50 / 100
    height: Screen.desktopAvailableHeight - Screen.desktopAvailableHeight * 25 / 100

    ListModel { id: snapshotModel }

    Connections {
        target: Snapshot
        onFailed: {
            password = ""
        }
    }

    Maui.PopupPage
    {
        id: popupPage
        hint: 1

        title: i18n("Terminal")

        headBar.background: Maui.ShadowedRectangle {
            anchors.fill: parent

            Maui.Theme.inherit: false
            Maui.Theme.colorSet: Maui.Theme.View

            border.width: 0
            border.color: Qt.lighter("#dadada",1.08)
            shadow.size: 15
            shadow.color: Maui.ColorUtils.brightnessForColor(Maui.Theme.backgroundColor) == Maui.ColorUtils.Light ? Qt.darker("#dadada",1.1) : "#2c2c2c"
            shadow.xOffset: -1
            shadow.yOffset: 0

            color: Maui.Theme.backgroundColor
            corners.topLeftRadius: 6
            corners.topRightRadius: 6
        }

        background: Maui.ShadowedRectangle {
            anchors.fill: parent

            Maui.Theme.inherit: false
            Maui.Theme.colorSet: Maui.Theme.View

            border.width: 0
            border.color: Qt.lighter("#dadada",1.08)
            shadow.size: 15
            shadow.color: Maui.ColorUtils.brightnessForColor(Maui.Theme.backgroundColor) == Maui.ColorUtils.Light ? Qt.darker("#dadada",1.1) : "#2c2c2c"
            shadow.xOffset: -1
            shadow.yOffset: 0

            color: Maui.Theme.backgroundColor
            corners.topLeftRadius: 6
            corners.topRightRadius: 6
            corners.bottomLeftRadius: 6
            corners.bottomRightRadius: 6
        }

        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 200
            radius: 4
            color: Maui.Theme.alternateBackgroundColor
            Label {
                    anchors.fill: parent
                    anchors.margins: 10
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    text: "It is possible to restore the system without a graphical interface in case your PC does not boot\n\nTurn on your pc\nAdvanced options for Nova Flow\nNova Flow, with Linux 6.7.2-1-default (recovery mode)\nEnter your password (no prompt shown)\nsudo su your_user\ncd /home/your_user"
            }
        }

        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 40
            radius: 4
            color: Maui.Theme.alternateBackgroundColor
            Label {
                    anchors.fill: parent
                    anchors.margins: 10
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    text: "novarewind --help"
            }
        }

        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 40
            radius: 4
            color: Maui.Theme.alternateBackgroundColor
            Label {
                    anchors.fill: parent
                    anchors.margins: 10
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    text: "novarewind list"
            }
        }

        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 40
            radius: 4
            color: Maui.Theme.alternateBackgroundColor
            Label {
                    anchors.fill: parent
                    anchors.margins: 10
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    text: "novarewind create"
            }
        }
        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 40
            radius: 4
            color: Maui.Theme.alternateBackgroundColor
            Label {
                    anchors.fill: parent
                    anchors.margins: 10
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    text: "novarewind restore 20240208-185814"
            }
        }

        Rectangle
        {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 40
            radius: 4
            color: Maui.Theme.alternateBackgroundColor
            Label {
                    anchors.fill: parent
                    anchors.margins: 10
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    text: "novarewind remove 20240208-185814"
            }
        }
    }

    Maui.SideBarView
    {
        id: _sideBarView
        anchors.fill: parent

        sideBarContent:  Maui.Page
        {
            anchors.fill: parent
            Maui.Theme.colorSet: Maui.Theme.Window

            headBar.leftContent: [
                Maui.ToolButtonMenu
                {
                    icon.name: "application-menu"

                    MenuItem
                    {
                        text: i18n("Terminal")
                        icon.name: "akonadiconsole"
                        onTriggered: popupPage.open()
                    }
                    MenuItem
                    {
                        text: i18n("About")
                        icon.name: "documentinfo"
                        onTriggered: root.about()
                    }
                }
            ]
        }

        Maui.Page
        {
            anchors.fill: parent

            headBar.visible: false

            Component.onCompleted: {
                stackView.push("qrc:/Home.qml")
            }

            StackView {
                id: stackView
                anchors.fill: parent
            }
        }
    }
}
