import QtQuick 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.13
import QtCharts 2.3
import QtQuick.Window 2.12
//import QtQuick.Controls 1.4 as OldControls
import TableModel 0.1

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
            //anchors.fill: parent
            height: window.height/2

            columnSpacing: 1
            rowSpacing: 1
            clip: true

            model: TableModel {}

            delegate: Rectangle {
            //    //implicitWidth: 100
            //    //implicitHeight: 50
                Text {
                    text: display
                }
            }
        }

        //OldControls.TableView {
        //    id : stocksTable
        //
        //    height: window.height/2
        //    OldControls.TableViewColumn {
        //        role: "name"
        //        title: "Name"
        //        width: 200
        //    }
        //    OldControls.TableViewColumn {
        //        role: "year"
        //        title: "Jahr"
        //        width: 60
        //    }
        //    OldControls.TableViewColumn {
        //        role: "percentage"
        //        title: "Rang"
        //        width: 60
        //    }
        //    OldControls.TableViewColumn {
        //        role: "earningCorrelation"
        //        title: "Gewinnkorrelation"
        //        width: 120
        //    }
        //    OldControls.TableViewColumn {
        //        role: "earningGrowth"
        //        title: "Gewinnwachstum"
        //        width: 120
        //    }
        //    OldControls.TableViewColumn {
        //        role: "dividendGrowth"
        //        title: "Dividendenwachstum"
        //        width: 120
        //    }
        //    OldControls.TableViewColumn {
        //        role: "payoutRatio"
        //        title: "Auschüttungsquote"
        //        width: 120
        //    }
        //    OldControls.TableViewColumn {
        //        role: "remark"
        //        title: "Bemerkung"
        //        width: 250
        //    }
        //    model: libraryModel
        //}
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
