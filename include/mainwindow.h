#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "include/qespeak.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void recvEvent(const espeak_EVENT *event_type);
    void on_playBtn_clicked();

    void on_speakWavBtn_clicked();

    void on_VolumeHSdr_sliderMoved(int position);

    void on_RateHSdr_sliderMoved(int position);

    void on_showEventChkBox_clicked();

private:
    Ui::MainWindow *ui;
    QEspeak *m_espeak;
};

#endif // MAINWINDOW_H
