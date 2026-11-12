#include "MainWindow.h"
#include "tests.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QtCharts/QChart>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    runAndDisplayTests();
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels({"Количество элементов", "Сырые указатели (с)", "ShrdPtr (с)", "std::shared_ptr (с)"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setMinimumHeight(150);

    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(tableWidget);
    layout->addWidget(chartView);
    
    setCentralWidget(centralWidget);
    resize(800, 600);
    setWindowTitle("Тестирование умных указателей");
}

void MainWindow::runAndDisplayTests() {
    RunFunctionalTests(); 
    std::vector<LoadTestResult> results = RunAllLoadTests();

    tableWidget->setRowCount(results.size());

    QLineSeries* rawSeries = new QLineSeries();
    rawSeries->setName("Сырые указатели");
    QLineSeries* shrdSeries = new QLineSeries();
    shrdSeries->setName("Наш ShrdPtr");
    QLineSeries* stlSeries = new QLineSeries();
    stlSeries->setName("std::shared_ptr");

    for (size_t i = 0; i < results.size(); ++i) {
        const auto& res = results[i];
        
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(res.elements_count)));
        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(res.time_raw, 'f', 6)));
        tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(res.time_shrd, 'f', 6)));
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(res.time_stl, 'f', 6)));

        rawSeries->append(res.elements_count, res.time_raw);
        shrdSeries->append(res.elements_count, res.time_shrd);
        stlSeries->append(res.elements_count, res.time_stl);
    }

    QChart* chart = new QChart();
    chart->addSeries(rawSeries);
    chart->addSeries(shrdSeries);
    chart->addSeries(stlSeries);
    chart->setTitle("Сравнение времени аллокации и удаления");
    chart->createDefaultAxes();
    
    chartView->setChart(chart);
}