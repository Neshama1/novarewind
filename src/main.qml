import QtCore
import QtQuick
import QtQml
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import org.mauikit.controls as Maui
import org.kde.novarewind 1.0
import "controls"

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

    property int styleType: Maui.Style.Auto

    Settings {
        property alias keepDaily: root.keepDaily
        property alias keepWeekly: root.keepWeekly
        property alias keepMonthy: root.keepMonthly
        property alias daily: root.daily
        property alias weekly: root.weekly
        property alias monthy: root.monthly
        property alias styleType: root.styleType
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

        // Theme

        Maui.Style.styleType = styleType === Maui.Style.Auto ? themeManager.styleType : styleType
        Maui.Style.accentColor = "aquamarine"
        Maui.Style.windowControlsTheme = themeManager.windowControlsTheme

        // Load custom settings for automated snapshots

        Snapshot.setDailyConfig(daily, keepDaily)
        Snapshot.setWeeklyConfig(weekly, keepWeekly)
        Snapshot.setMonthlyConfig(monthly, keepMonthly)
    }

    SettingsDialog {
        id: settingsDialog
    }

    // TERMINAL POPUP PAGE

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

        Maui.SectionItem {
            label1.text: "Recovery options for Nova"
            label2.text: "It is possible to restore the system without a graphical interface in case your PC does not boot"
            Rectangle
            {
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 100
                Layout.topMargin: 10
                Layout.bottomMargin: 10
                radius: 4
                color: "black" // Qt.lighter(Maui.Theme.alternateBackgroundColor, 1.02)
                Label {
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pixelSize: 12
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                    opacity: 0.85
                    text: "Turn on your pc\nNova Flow, with Linux x.x.x-x-default (recovery mode)\nEnter your password\nType exit to restart"
                }
            }
            Rectangle
            {
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40
                radius: 4
                color: Maui.Theme.alternateBackgroundColor
                Label {
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    opacity: 0.90
                    text: "> novarewind --help"
                }
            }
            Rectangle
            {
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40
                radius: 4
                color: Maui.Theme.alternateBackgroundColor
                Label {
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    opacity: 0.90
                    text: "> novarewind list"
                }
            }
            Rectangle
            {
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40
                radius: 4
                color: Maui.Theme.alternateBackgroundColor
                Label {
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    opacity: 0.90
                    text: "> novarewind create"
                }
            }
            Rectangle
            {
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40
                radius: 4
                color: Maui.Theme.alternateBackgroundColor
                Label {
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    opacity: 0.90
                    text: "> novarewind restore 20240208-185814"
                }
            }
            Rectangle
            {
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 40
                radius: 4
                color: Maui.Theme.alternateBackgroundColor
                Label {
                    anchors.fill: parent
                    anchors.margins: 10
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    opacity: 0.90
                    text: "> novarewind remove 20240208-185814"
                }
            }
        }
    }

    // APP INTERFACE

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

                    MenuSeparator {}

                    MenuItem
                    {
                        text: i18n("Light")
                        checkable: true
                        autoExclusive: true
                        onTriggered: {
                            Maui.Style.styleType = Maui.Style.Light
                            styleType = Maui.Style.styleType
                        }
                        checked: Maui.Style.styleType === Maui.Style.Light
                    }

                    MenuItem
                    {
                        text: i18n("Dark")
                        checkable: true
                        autoExclusive: true
                        onTriggered: {
                            Maui.Style.styleType = Maui.Style.Dark
                            styleType = Maui.Style.styleType
                        }
                        checked: Maui.Style.styleType === Maui.Style.Dark
                    }

                    MenuItem
                    {
                        text: i18n("Custom")
                        checkable: true
                        autoExclusive: true
                        onTriggered: {
                            Maui.Style.styleType = Maui.Style.Auto
                            styleType = Maui.Style.styleType
                        }
                        checked: Maui.Style.styleType === Maui.Style.Auto
                    }

                    MenuSeparator {}

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
                stackView.push("controls/Home.qml")
            }

            StackView {
                id: stackView
                anchors.fill: parent
            }
        }
    }
}
