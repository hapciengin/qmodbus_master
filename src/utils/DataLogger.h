#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QMainWindow>

QT_CHARTS_USE_NAMESPACE

class DataLogger : public QMainWindow {
    Q_OBJECT

public:
    explicit DataLogger(QWidget *parent = nullptr);
    ~DataLogger();

public slots:
    void logData(int address, const QVariant& value, const QString& unit = "");
    void saveToJson();

private:
    QTimer* saveTimer;
    QJsonArray dataArray;
    QString logFilePath;
    
    // Grafik bileşenleri
    QChart* chart;
    QChartView* chartView;
    QMap<int, QLineSeries*> dataSeries;
    QDateTimeAxis* axisX;
    QValueAxis* axisY;

    void setupUI();
    void updateChart(int address, double value, const QDateTime& timestamp);
    QString getCurrentDateTime() const;
};

#endif // DATA_LOGGER_H