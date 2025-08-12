#include "DataLogger.h"
#include <QVBoxLayout>
#include <QDir>
#include <QDebug>

DataLogger::DataLogger(QWidget *parent)
    : QMainWindow(parent)
    , saveTimer(new QTimer(this))
    , chart(new QChart())
    , chartView(new QChartView(chart))
    , axisX(new QDateTimeAxis)
    , axisY(new QValueAxis)
{
    setupUI();

    // JSON kayıt dizinini oluştur
    QString homeDir = QDir::homePath();
    QString saveDir = homeDir + "/modbus_logs";
    QDir().mkpath(saveDir);
    
    // Timestamp ile dosya adı oluştur
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    logFilePath = saveDir + "/modbus_data_" + timestamp + ".json";

    // Timer ayarları
    saveTimer->setInterval(20000); // 20 saniye
    connect(saveTimer, &QTimer::timeout, this, &DataLogger::saveToJson);
    saveTimer->start();

    // Pencere ayarları
    resize(800, 600);
    setWindowTitle("Modbus Data Logger");
}

DataLogger::~DataLogger()
{
    saveToJson(); // Son verileri kaydet
    saveTimer->stop();
}

void DataLogger::setupUI()
{
    // Grafik ayarları
    chart->legend()->show();
    chart->createDefaultAxes();
    
    // Zaman ekseni
    axisX->setFormat("HH:mm:ss");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);

    // Değer ekseni
    axisY->setTitleText("Value");
    chart->addAxis(axisY, Qt::AlignLeft);

    // ChartView ayarları
    chartView->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(chartView);
}

void DataLogger::logData(int address, const QVariant& value, const QString& unit)
{
    QDateTime timestamp = QDateTime::currentDateTime();
    
    // JSON verisi oluştur
    QJsonObject dataPoint;
    dataPoint["timestamp"] = timestamp.toString(Qt::ISODate);
    dataPoint["address"] = address;
    dataPoint["value"] = value.toDouble();
    dataPoint["unit"] = unit;
    
    dataArray.append(dataPoint);

    // Grafiği güncelle
    updateChart(address, value.toDouble(), timestamp);
}

void DataLogger::updateChart(int address, double value, const QDateTime& timestamp)
{
    // Her adres için ayrı seri oluştur
    if (!dataSeries.contains(address)) {
        QLineSeries* series = new QLineSeries();
        series->setName(QString("Address %1").arg(address));
        dataSeries[address] = series;
        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }

    // Veriyi ekle
    dataSeries[address]->append(timestamp.toMSecsSinceEpoch(), value);

    // Eksenleri güncelle
    axisX->setRange(QDateTime::currentDateTime().addSecs(-300), QDateTime::currentDateTime());
    
    // Y ekseni otomatik ayarlanır
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();
    for (const auto& series : dataSeries) {
        const auto points = series->points();
        for (const auto& point : points) {
            minY = qMin(minY, point.y());
            maxY = qMax(maxY, point.y());
        }
    }
    axisY->setRange(minY - 1, maxY + 1);
}

void DataLogger::saveToJson()
{
    QJsonDocument doc(dataArray);
    QFile file(logFilePath);
    
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        qDebug() << "Data saved to:" << logFilePath;
    } else {
        qDebug() << "Failed to save data to:" << logFilePath;
    }
}

QString DataLogger::getCurrentDateTime() const
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
}