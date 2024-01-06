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
