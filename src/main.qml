import QtQuick 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtCharts 2.3
import QtQuick.Window 2.3
import QtQml.Models 2.2
import TableModel 0.1
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    id: window
    visible: true
    width: 500
    height: 300
    visibility: Window.Maximized
    title: "Stockcheck"

    Material.theme: Material.Dark
    Material.accent: Material.Orange

    style: ApplicationWindowStyle {
        background: Rectangle {
            color: Material.color(Material.DeepOrange)
        }
    }

    TableModel{
        id: tableModel
    }

    property LineSeries chartSeriesEarnings
    property LineSeries chartSeriesDividend
    property LineSeries chartSeriesRevenues
    property LineSeries chartScatterEarnings
    property LineSeries chartScatterDividend
    property LineSeries chartScatterRevenues

    // --- Functions and Actions ------------------------------------------

    function onMenuItemTriggered(name)
    {
        for (var p in theDatabaseMenu.items)
        {
            if (theDatabaseMenu.items[p].action.text !== name)
                theDatabaseMenu.items[p].action.checked = false
            else
                theDatabaseMenu.items[p].action.checked = true
        }
        tableModel.onDatabaseChanged(name);
        if(stocksTable.rowCount > 0)
        {
            stocksTable.selection.deselect(0, stocksTable.rowCount - 1);
            stocksTable.selection.select(0);
            addseries("");
        }
    }

    function onActualize()
    {
        tableModel.onActualize();
    }

    // --- About Dialog ---------------------------------------------------

    Dialog {
        id: aboutDialog
        title: "Stockcheck -- Beschreibung"

        height: 0.8*window.height
        width: 0.4*window.width

        Rectangle {
            Label {
                anchors.fill: parent
                width: aboutDialog.availableWidth
                text:
"Der Stockcheck ist ein Programm zur Analyse von Aktien. Relevant und notwendig
für die Bewertung einer Aktie sind der Umsatz, der Gewinn/Aktie und die Dividende/Aktie.

Für jede Größe (Umsatz, Gewinn/Aktie und Dividende/Aktie) wird die Compound Annual Growth
Rate (CAGR) auf Basis der letzten 5 Jahre berechnet. Außerdem wird die kumulierte
Ausschüttungsquote der letzen 5 Jahre berechnet.

Auf Basis dieser berechneten Größen werden jeweils bis zu 5 Punkte wie folgt vergeben,
jeweils für den Umsatz, den Gewinn/Aktie und die Dividende/Aktie:

CAGR > 0.00% p.a.: +1 Punkt
CAGR > 1.25% p.a.: +1 Punkt
CAGR > 2.50% p.a.: +1 Punkt
CAGR > 3.75% p.a.: +1 Punkt
CAGR > 5.00% p.a.: +1 Punkt
------------------------

Für die Auschüttungsquote (AQ) werden bis zu 5 Punkte wie folgt vergeben:

5%  < AQ < 95%: +1 Punkt
10% < AQ < 90%: +1 Punkt
15% < AQ < 85%: +1 Punkt
20% < AQ < 80%: +1 Punkt
30% < AQ < 70%: +1 Punkt
------------------------

Eine Aktie kann so bis zu 20 Punkte erhalten.
Die Bewertung A, B oder C der Aktie wird wie folgt ermittelt:

14 <= Punkte: Bewertung A
04 <= Punkte: Bewertung B
sonst: Bewertung C
-------------------------

Die Aktien werden anschließend nach den erreichten Punkten, und hernach nach
dem Dividenden CAGR sortiert."
                font.pixelSize: 12
            }
        }
    }

    // --- Functions to bind to outside ---------------------------------------

    function addToMenu(databaseNames, currentDatabase) {
        theDatabaseMenu.clear()
        var item;
        if(databaseNames.length > 0)
        {
            theDatabaseMenu.visible = true;
            var component = Qt.createComponent("Action.qml");
            for(var i = 0; i < databaseNames.length; i++){
                item = theDatabaseMenu.addItem(databaseNames[i]);
                item.text = databaseNames[i];

                // unfortunately there is no possibility to create actions dynamically
                if(i === 0)
                    item.action = action0
                else if(i === 1)
                    item.action = action1
                else if(i === 2)
                    item.action = action2
                else if(i === 3)
                    item.action = action3
                else if(i === 4)
                    item.action = action4
                item.action.text = databaseNames[i]

                if(databaseNames[i] === currentDatabase)
                    item.action.checked = true
            }
        } else theDatabaseMenu.visible = false
    }

    function addseries(tablename)
    {
        chartViewBig.removeAllSeries();

        chartSeriesEarnings = chartViewBig.createSeries(ChartView.SeriesTypeLine, "Gewinn/Aktie", xAxis, yAxis);
        chartScatterEarnings = chartViewBig.createSeries(ChartView.SeriesTypeLine, "", xAxis, yAxis);

        chartSeriesDividend = chartViewBig.createSeries(ChartView.SeriesTypeLine, "Dividende/Aktie", xAxis, yAxis);
        chartScatterDividend = chartViewBig.createSeries(ChartView.SeriesTypeLine, "", xAxis, yAxis);

        chartSeriesRevenues = chartViewBig.createSeries(ChartView.SeriesTypeLine, "Umsatz", xAxis, axisY2);
        chartScatterRevenues = chartViewBig.createSeries(ChartView.SeriesTypeLine, "", xAxis, axisY2);

        tableModel.getLineSeriesPerShare(tablename,
                                         chartSeriesEarnings,
                                         chartScatterEarnings,
                                         chartSeriesDividend,
                                         chartScatterDividend,
                                         xAxis,
                                         yAxis);

        tableModel.getLineSeriesRevenue(tablename,
                                        chartSeriesRevenues,
                                        chartScatterRevenues,
                                        axisY2);

        chartSeriesDividend.color = "blue"
        chartScatterDividend.color = "blue"
        chartScatterDividend.markerSize = 8;
        chartSeriesEarnings.color = "green"
        chartScatterEarnings.color = "green"
        chartScatterEarnings.markerSize = 8
        chartSeriesRevenues.color = "grey"
        chartScatterRevenues.color = "grey"
        chartScatterRevenues.markerSize = 8

        chartSeriesDividend.width = 5
        chartSeriesEarnings.width = 5
        chartSeriesRevenues.width = 5

        chartViewBig.title = tableModel.getTableName(tablename);

        xAxis.tickCount = chartSeriesDividend.count;
        xAxis.labelFormat = "%d"
    }

    // --- Menu ---------------------------------------------------------------
    menuBar: MenuBar {
        id: stockcheckMenu

        Menu {
            id: theDatabaseMenu
            title: qsTr('Datenbank')

        }
        Menu {
            title: qsTr('Stockcheck')
            MenuItem{
                text: "Aktualisieren"
                shortcut: "F5"
                onTriggered: onActualize()
            }
            MenuItem{
                text: "Beschreibung"
                onTriggered: aboutDialog.open()
            }
        }

        style: MenuBarStyle{
            background: Rectangle {
                color: "LightGrey"
            }
        }
    }

    // --- SplitView ----------------------------------------------------------
    SplitView {
        orientation: Qt.Vertical
        anchors.fill: parent
        // --- Table ----------------------------------------------------------
        TableView {
            model: tableModel
            id : stocksTable
            sortIndicatorVisible: true;

            onSortIndicatorColumnChanged : {
                selection.deselect(0, rowCount - 1);
                model.sort(sortIndicatorColumn, sortIndicatorOrder);
            }

            onSortIndicatorOrderChanged: {
                selection.deselect(0, rowCount - 1);
                model.sort(sortIndicatorColumn, sortIndicatorOrder)
            }

            onClicked: addseries(currentRow)
            onCurrentRowChanged: addseries(currentRow)

            height: window.height/2
            width: window.width
            TableViewColumn {
                role: "name"
                title: "Name"
                width: 200
                movable: false
            }
            TableViewColumn {
                role: "year"
                title: "Jahr"
                width: 60
                movable: false
            }
            TableViewColumn {
                role: "rate"
                title: "Rating"
                width: 60
                movable: false
            }
            TableViewColumn {
                role: "percentage"
                title: "Punkte"
                width: 60
                movable: false
            }
            TableViewColumn {
                role: "revenueGrowth"
                title: "Umsatz CAGR"
                width: 100
                movable: false
            }
            TableViewColumn {
                role: "earningGrowth"
                title: "Gewinn CAGR"
                width: 100
                movable: false
            }
            TableViewColumn {
                role: "dividendGrowth"
                title: "Dividenden CAGR"
                width: 100
                movable: false
            }
            TableViewColumn {
                role: "payoutRatio"
                title: "Auschüttungsquote"
                width: 100
                movable: false
            }
            TableViewColumn {
                role: "remark"
                title: "Bemerkung"
                width: 350
                movable: false
            }

            style: TableViewStyle {
                backgroundColor: "White"
                alternateBackgroundColor: "LightGrey"
            }
        }
        // --- Chart ----------------------------------------------------------
        Rectangle {
            ChartView {
                id: chartViewBig
                title: ""
                anchors.fill: parent
                antialiasing: true
                backgroundColor: "LightGrey"
                width: window.height/2 * 0.78
                height: window.width/2
                legend.font.pointSize: 12
                legend.font.bold: true
                legend.font.family: "Helvetica"

                titleFont.pointSize: 20
                titleFont.bold: true
                //titleFont.italic: true
                titleFont.family: "Helvetica"
                axes: [
                    ValueAxis{
                        id: xAxis
                    },
                    ValueAxis{
                        id: yAxis
                        color: "green"
                    },
                    ValueAxis {
                        id: axisY2
                        color: "gray"
                    }

                ]
            }
        }

        // --- Actions for dynamicall MenuItems -------------------------------

        // unfortunately there is no possibility to create actions dynamically
        // so we create some actions in preperation
        Action {
            id: action0
            checkable: true
            checked: false
            onTriggered: onMenuItemTriggered(text)
        }

        Action {
            id: action1
            checkable: true
            checked: false
            onTriggered: onMenuItemTriggered(text)
        }

        Action {
            id: action2
            checkable: true
            checked: false
            onTriggered: onMenuItemTriggered(text)
        }

        Action {
            id: action3
            checkable: true
            checked: false
            onTriggered: onMenuItemTriggered(text)
        }

        Action {
            id: action4
            checkable: true
            checked: false
            onTriggered: onMenuItemTriggered(text)
        }        
    }
}
