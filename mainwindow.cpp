#include <QCameraInfo>
#include <QCamera>
#include <QMediaRecorder>
#include <QVideoWidget>
#include <QUrl>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _camera(new QCamera(QCameraInfo::defaultCamera(), this))
    , _recorder(new QMediaRecorder(_camera, this))
    , _vWidget(new QVideoWidget)
{
    ui->setupUi(this);

    connect(_camera, &QCamera::errorOccurred, this, &MainWindow::errorOccurred);
    connect(_recorder, QOverload<QMediaRecorder::Error>::of(&QMediaRecorder::error), this, &MainWindow::recorderError);

    _vWidget->setParent(ui->widget);
    _camera->setCaptureMode(QCamera::CaptureVideo);
    _camera->setViewfinder(_vWidget);
}

MainWindow::~MainWindow()
{
    _recorder->stop();
    _camera->stop();
    _camera->setViewfinder((QVideoWidget*)nullptr);

    delete ui;
}

void MainWindow::on_pushButtonStartCamera_clicked()
{
    _vWidget->setGeometry(ui->widget->rect());
    _vWidget->show();
    auto setting = _recorder->videoSettings();
    setting.setBitRate(5000 * 1000);
    setting.setCodec("video/x-h264");                                       // select h264 codec.
    setting.setEncodingMode(QMultimedia::ConstantQualityEncoding);
    setting.setFrameRate(_camera->viewfinderSettings().maximumFrameRate()); // must just match one of webcam fps.
    setting.setQuality(QMultimedia::NormalQuality);
    setting.setResolution(_camera->viewfinderSettings().resolution());      // must just match one of webcam resolutions.
    _recorder->setVideoSettings(setting);
    _camera->start();
    _recorder->setOutputLocation(QUrl::fromLocalFile(qApp->applicationDirPath() + "/" "video.mp4"));
}

void MainWindow::on_pushButtonRecord_clicked()
{
    _recorder->record();
}

void MainWindow::on_pushButtonStopRecord_clicked()
{
    _recorder->stop();
}

void MainWindow::recorderError(QMediaRecorder::Error error)
{
    qDebug() << "***** QMediaREcorder Error *****";
    qDebug() << _recorder->errorString() << error;
}

void MainWindow::errorOccurred(QCamera::Error value)
{
    qDebug() << "***** QCamera Error *****";
    qDebug() << _camera->errorString() << value;
}
