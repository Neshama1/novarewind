import QtQuick 2.15
import QtQml 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import org.mauikit.controls 1.3 as Maui
import org.kde.novarewind 1.0
import QtQuick.Window 2.15
import Qt.labs.settings 1.0

Maui.ApplicationWindow
{
    id: root
    title: qsTr("")

    property string password
    property int removeIndex
    property int restoreIndex

    property int keepDaily: 3
    property int keepWeekly: 1
    property int keepMonthly: 1
    property bool daily: true
    property bool weekly: true
    property bool monthly: true

    Settings {
        property alias keepDaily: root.keepDaily
        property alias keepWeekly: root.keepWeekly
        property alias keepMonthy: root.keepMonthly
        property alias daily: root.daily
        property alias weekly: root.weekly
        property alias monthy: root.monthly
    }

    width: Screen.desktopAvailableWidth - Screen.desktopAvailableWidth * 50 / 100
    height: Screen.desktopAvailableHeight - Screen.desktopAvailableHeight * 25 / 100

    ListModel { id: snapshotModel }

    Connections {
        target: Snapshot
        onFailed: {
            password = ""
        }
    }

    Component.onCompleted: {
        Snapshot.setDailyConfig(daily, keepDaily)
        Snapshot.setWeeklyConfig(weekly, keepWeekly)
        Snapshot.setMonthlyConfig(monthly, keepMonthly)
    }

    SettingsDialog
    {
        id: settingsDialog
    }

    Maui.PopupPage
    {
        id: popupPage
        hint: 1

        title: i18n("Terminal")

        width: 400

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
            color: Maui.Theme.backgroundColor
            Column {
                anchors.left: parent.left
                anchors.right: parent.right
                height: parent.height / 3
                Label {
                    id: lbRecoveryTitle
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 10
                    //height: 15
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 17
                    wrapMode: Text.WordWrap
                    text: "Recovery options in Nova Flow OS"
                }
                Label {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: lbRecoveryTitle.bottom
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    anchors.topMargin: 10
                    //height: parent.height / 3
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    text: "It is possible to restore the system without a graphical interface in case your PC does not boot" //\n\nTurn on your pc\nAdvanced options for Nova Flow\nNova Flow, with Linux 6.7.2-1-default (recovery mode)\nEnter your password (no prompt shown)\nType exit to restart"
                }
            }
            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 10
                height: parent.height / 2
                radius: 3
                color: Qt.lighter(Maui.Theme.backgroundColor,1.05)
                Label {
                    anchors.fill: parent
                    anchors.margins: 10
                    height: parent.height / 2
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    text: "Turn on your pc\nAdvanced options for Nova Flow\nNova Flow, with Linux X.X.X-X-default (recovery mode)\nEnter your password (no prompt shown)\nType exit to restart"
                }
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
                        text: i18n("Settings")
                        icon.name: "settings-configure"
                        onTriggered: settingsDialog.open()
                    }
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
