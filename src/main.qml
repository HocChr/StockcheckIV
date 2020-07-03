import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtCharts 2.3
import QtQuick.Window 2.3

ApplicationWindow {
    id: window
    visible: true
    width: 500
    height: 300
    visibility: Window.Maximized

    // --- Menu ---------------------------------------------------------------
    menuBar: MenuBar {
        Menu {
            title: qsTr('Datenbank')
        }
        Menu {
            title: qsTr('Info')
        }
    }

    // --- SplitView ----------------------------------------------------------
    SplitView {
        orientation: Qt.Vertical
        anchors.fill: parent

        // --- Table ----------------------------------------------------------
        TableView {
            height: window.height/2
            TableViewColumn {
                role: "name"
                title: "Name"
                width: 200
            }
            TableViewColumn {
                role: "year"
                title: "Jahr"
                width: 60
            }
            TableViewColumn {
                role: "percentage"
                title: "Rang"
                width: 60
            }
            TableViewColumn {
                role: "earningCorrelation"
                title: "Gewinnkorrelation"
                width: 120
            }
            TableViewColumn {
                role: "earningGrowth"
                title: "Gewinnwachstum"
                width: 120
            }
            TableViewColumn {
                role: "dividendGrowth"
                title: "Dividendenwachstum"
                width: 120
            }
            TableViewColumn {
                role: "payoutRatio"
                title: "Auschüttungsquote"
                width: 120
            }
            TableViewColumn {
                role: "remark"
                title: "Bemerkung"
                width: 250
            }
            model: libraryModel
        }
        // --- Chart ----------------------------------------------------------
        Rectangle {
            ChartView {
                title: "Line"
                anchors.fill: parent
                antialiasing: true

                LineSeries {
                    name: "LineSeries"
                    XYPoint { x: 0; y: 0 }
                    XYPoint { x: 1.1; y: 2.1 }
                    XYPoint { x: 1.9; y: 3.3 }
                    XYPoint { x: 2.1; y: 2.1 }
                    XYPoint { x: 2.9; y: 4.9 }
                    XYPoint { x: 3.4; y: 3.0 }
                    XYPoint { x: 4.1; y: 3.3 }
                }
            }
        }
    }
}
