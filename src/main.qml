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
"Der Stockcheck ist ein Programm zur Analyse von Aktien. Auf Basis der Dividende pro Aktie und des
Gewinn pro Aktie der mindestens letzten 8 Jahre wir eine Berechnung des Gewinnwachstums,
der Gewinnstabilität, der Dividendenstabilität, des Dividendenwachstums und der Auschüttungsquote
durchgeführt.

Die Aktien werden in 2 Kategorien unterteilt. Zum einen in Dividendenaktien, zum anderen in
Wachstumsaktien. Für beide Kategorien wird ein Punktzahl ermittelt und auf Basis der Punktzahl
eine Rating A, B oder C.

Eine Wachstumsaktie ist eine Aktie, die in ihrer Vergangenheit keine Dividende gezahlt hat und
die mindestens eine Punktezahl von 50 in der Wachstums-Aktien-Bewertung erreicht.
Eine Dividendenaktie ist eine Aktie, die keine Wachstumsaktie ist, die aber mindestens 50 Punkte
nach der Dividenden-Aktien-Bewertung erreicht.

Die Bewertung funktioniert wie folgt. Für jedes zu bewertende Kriterium wird zwischen einer
unteren und einer oberen Grenze linear interpoliert. D.h. erreicht eine Aktie das Minimum eines
Kriteriums nicht, erhält sie 0 Punkte. Erreicht die Aktie das Maximum eines Kriteriums, erhält
sie 100 Punkte. Dazwischen wird interpoliert.

Schließlich wird der Mittelwert über die Anzahl der herangezogenen Kriterien ermittelt.

Die Kriterien für eine Wachstumsaktie sind

    1. Beträgt die Compound Annual Growth Rate (CAGD) des Gewinns pro Aktie über die letzten 3 Jahre
       0% p.a oder weniger, erhält die Aktie 0 Punkte. Beträgt das CAGD 10% p.a. oder mehr, 100 Punkte.
    2. Beträgt die Stabilität des Gewinns 0.5 oder weniger -> 0 Punkte, bei 1 -> 100 Punkte.

    3. Beträgt die Compound Annual Growth Rate (CAGD) des Umsatzes über die letzten 3 Jahre
       0% p.a oder weniger, erhält die Aktie 0 Punkte. Beträgt das CAGD 10% p.a. oder mehr, 100 Punkte.
    4. Beträgt die Stabilität des Umsatzes 0.5 oder weniger -> 0 Punkte, bei 1 -> 100 Punkte.

Die Kriterien für eine Dividendenaktie sind

    1. Beträgt die CAGD des Gewinns pro Aktie über die letzten 3 Jahre 0% p.a oder weniger -> 0 Punkte.
       Beträgt das CAGD 5% p.a. oder mehr, 100 Punkte.
    2. Beträgt die Stabilität des Gewinns 0.0 oder weniger -> 0 Punkte, beträgt sie 1 -> 100 Punkte.

    3. Beträgt die CAGD des Umsatzes über die letzten 3 Jahre 0% p.a oder weniger -> 0 Punkte.
       Beträgt das CAGD 5% p.a. oder mehr, 100 Punkte.
    4. Beträgt die Stabilität des Umsatzes 0.0 oder weniger -> 0 Punkte, beträgt sie 1 -> 100 Punkte.

    5. Die Anzahl der Jahre ohne Dividendenkürzung ist mindestens 0 -> 0 Punkte. 10 Jahre oder mehr -> 100 Punkte.
    6. Beträgt die CAGD des Dividende pro Aktie über die letzten 3 Jahre beträgt 0% p.a oder weniger -> 0 Punkte.
       Beträgt das CAGD 5% p.a. oder mehr, 100 Punkte.

    7. Die Auschüttungsquote wird zu 100% erreicht, wenn sie innerhalb [40%, 60%] liegt.
       Darüber/darunter wird linear bis auf 100%/0% interpoliert.

Erreicht eine Aktie mindestens 50 Punkte als als Wachstumsaktie oder Dividendenaktie, erhält sie
das Rating B. Erhält sie mindestens 75 Punkte, erhält sie das Rating A. Ansonsten erhält sie das
Rating C."
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
                if(i == 0)
                    item.action = action0
                else if(i == 1)
                    item.action = action1
                else if(i == 2)
                    item.action = action2
                else if(i == 3)
                    item.action = action3
                else if(i == 4)
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
        chartSeriesDividend = chartViewBig.createSeries(ChartView.SeriesTypeLine, "Dividende/Aktie", xAxis, yAxis);
        chartSeriesRevenues = chartViewBig.createSeries(ChartView.SeriesTypeLine, "Umsatz", xAxis, axisY2);

        tableModel.getLineSeriesPerShare(tablename,
                                         chartSeriesEarnings,
                                         chartSeriesDividend,
                                         xAxis,
                                         yAxis);

        tableModel.getLineSeriesRevenue(tablename,
                                        chartSeriesRevenues,
                                        axisY2);

        chartSeriesDividend.color = "blue"
        chartSeriesEarnings.color = "green"
        chartSeriesRevenues.color = "grey"

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
                role: "revenueCorrelation"
                title: "Umsatzkorrelation"
                width: 120
                movable: false
            }
            TableViewColumn {
                role: "revenueGrowth"
                title: "Umsatzwachstum"
                width: 120
                movable: false
            }
            TableViewColumn {
                role: "earningCorrelation"
                title: "Gewinnkorrelation"
                width: 120
                movable: false
            }
            TableViewColumn {
                role: "earningGrowth"
                title: "Gewinnwachstum"
                width: 120
                movable: false
            }
            TableViewColumn {
                role: "dividendGrowth"
                title: "Dividendenwachstum"
                width: 120
                movable: false
            }
            TableViewColumn {
                role: "payoutRatio"
                title: "Auschüttungsquote"
                width: 120
                movable: false
            }
            TableViewColumn {
                role: "stockType"
                title: "Kategorie"
                width: 120
                movable: false
            }
            TableViewColumn {
                role: "remark"
                title: "Bemerkung"
                width: 250
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
