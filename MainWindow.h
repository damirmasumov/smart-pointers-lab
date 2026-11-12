#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    void setupUI();
    void runAndDisplayTests();

    QTableWidget* tableWidget;
    QChartView* chartView;
};