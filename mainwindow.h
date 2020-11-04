#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaRecorder>
#include <QCamera>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonStartCamera_clicked();
    void on_pushButtonRecord_clicked();
    void on_pushButtonStopRecord_clicked();
    void recorderError(QMediaRecorder::Error error);
    void errorOccurred(QCamera::Error value);

private:
    Ui::MainWindow *ui;
    QCamera* _camera;
    QMediaRecorder* _recorder;
    QVideoWidget* _vWidget;
};
#endif // MAINWINDOW_H
