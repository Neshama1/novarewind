// SettingsDialog.qml

import QtQuick
import QtQuick.Controls
import org.mauikit.controls as Maui
import org.kde.novarewind 1.0

Maui.SettingsDialog
{
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

    Maui.SectionGroup {

        title: i18n("Daily")
        //description: i18n("Daily snapshots")
        template.label1.font.pixelSize: 16

        Maui.FlexSectionItem
        {
            label1.text: i18n("Enable")
            label2.text: i18n("Activate snapshots")
            Switch {
                checkable: true
                checked: daily ? true : false
                onToggled: {
                    visualPosition == 0 ? daily = false : daily = true
                    visualPosition == 0 ? spinBoxDaily.enabled = false : spinBoxDaily.enabled = true
                    Snapshot.setDailyConfig(daily, keepDaily)
                }
            }
        }

        Maui.FlexSectionItem {
            //label1.text: i18n("Days")
            label2.text: i18n("Create restore point for ") + keepDaily + i18n(" days")
            wide: false
            SpinBox {
                id: spinBoxDaily
                from: 1
                to: 100
                value: keepDaily
                enabled: daily

                onValueModified: {
                    keepDaily = value
                    Snapshot.setDailyConfig(daily, keepDaily)
                    if (password != "") {
                        getSnapshots()
                    }
                }
            }
        }
    }

    Maui.SectionGroup {

        title: i18n("Weekly")
        //description: i18n("Weekly snapshots")
        template.label1.font.pixelSize: 16

        Maui.FlexSectionItem
        {
            label1.text: i18n("Enable")
            label2.text: i18n("Activate snapshots")
            Switch {
                checkable: true
                checked: weekly ? true : false
                onToggled: {
                    visualPosition == 0 ? weekly = false : weekly = true
                    visualPosition == 0 ? spinBoxWeekly.enabled = false : spinBoxWeekly.enabled = true
                    Snapshot.setWeeklyConfig(weekly, keepWeekly)
                }
            }
        }

        Maui.FlexSectionItem {
            //label1.text: i18n("Weeks")
            label2.text: i18n("Create restore point for ") + keepWeekly + i18n(" weeks")
            wide: false
            SpinBox {
                id: spinBoxWeekly
                from: 1
                to: 100
                value: keepWeekly
                enabled: weekly

                onValueModified: {
                    keepWeekly = value
                    Snapshot.setWeeklyConfig(weekly, keepWeekly)
                    if (password != "") {
                        getSnapshots()
                    }
                }
            }
        }
    }

    Maui.SectionGroup {

        title: i18n("Monthly")
        //description: i18n("Monthly snapshots")
        template.label1.font.pixelSize: 16

        Maui.FlexSectionItem
        {
            label1.text: i18n("Enable")
            label2.text: i18n("Activate snapshots")
            Switch {
                checkable: true
                checked: monthly ? true : false
                onToggled: {
                    visualPosition == 0 ? monthly = false : monthly = true
                    visualPosition == 0 ? spinBoxMonthly.enabled = false : spinBoxMonthly.enabled = true
                    Snapshot.setMonthlyConfig(monthly, keepMonthly)
                }
            }
        }

        Maui.FlexSectionItem {
            //label1.text: i18n("Months")
            label2.text: i18n("Create restore point for ") + keepDaily + i18n(" months")
            wide: false
            SpinBox {
                id: spinBoxMonthly
                from: 1
                to: 100
                value: keepMonthly
                enabled: monthly

                onValueModified: {
                    keepMonthly = value
                    Snapshot.setMonthlyConfig(monthly, keepMonthly)
                    if (password != "") {
                        getSnapshots()
                    }
                }
            }
        }
    }

    function getSnapshots() {
        snapshotModel.clear()
        Snapshot.listSnapshots()
        for (var i = 0; i < Snapshot.points.length; i++) {
            snapshotModel.append({"point": Snapshot.points[i].dateTime,"type": Snapshot.points[i].type})
        }
    }
}
