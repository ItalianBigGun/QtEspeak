#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
extern "C" {
#include <speak_lib.h>

#ifdef __WIN32__
#include <windows.h>
#define sleep(x) Sleep(1000*x)
#endif

int callback(short* wav, int num, espeak_EVENT *pEv)
{
  int cEv = 0;
  while (pEv->type) {
    cEv++;
    pEv++;
    }
  qDebug("callback, events: %d\n", cEv);
  return 0;
}

void espeak_test(const char *to_sp_text)
{
  int nRate = espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 10000, NULL, 0);
  qDebug("nRate: %d\n", nRate);
  if (nRate < 0)
    return;
  espeak_SetSynthCallback(callback);
  espeak_SetParameter(espeakRATE, 175, 0);
  int rv;
  rv = espeak_Char('c');
  qDebug("rv: %d\n", rv);
  qDebug("To speak: %s\n", to_sp_text);
  const char* sText = to_sp_text;
  rv = espeak_Synth(sText, 100, 0, POS_CHARACTER, 0, 0, 0, 0);
  qDebug("rv: %d (full: %d, internal: %d)\n",
    rv, EE_BUFFER_FULL, EE_INTERNAL_ERROR);
  sleep(2); // 2 seconds
  puts("slept");
  espeak_Terminate();
}

}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playBtn_clicked()
{
    QString speak_text = ui->speak_text->toPlainText();
    espeak_test(speak_text.toStdString().c_str());
}
