#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
extern "C" {
#include <speak_lib.h>

#ifdef __WIN32__
#include <windows.h>
#define sleep(x) Sleep(1000*x)
#endif
typedef  struct languege_map_{
    int index;
    const char *languege_str;
} LanguegeMap_t;
static LanguegeMap_t  languege_maps[] = {
    {0, "zh"}, {1, "zh-yue"}, {2, "en-uk-north"}, {3, "en-us"}
};
typedef  struct  event_table_s {
    int type;
    const char *event_str;
} EventTable_t;
static EventTable_t event_tables[] ={
   {0, "espeakEVENT_LIST_TERMINATED"},
        {1, "espeakEVENT_WORD"} ,
        {2, "espeakEVENT_SENTENCE"} ,
        {3, "espeakEVENT_MARK"} ,
        {4, "espeakEVENT_PLAY"} ,
        {5, "espeakEVENT_END"} ,
        {6, "espeakEVENT_MSG_TERMINATED"} ,
        {7, "espeakEVENT_PHONEME"} ,
        {8, "espeakEVENT_SAMPLERATE"} ,
        {9, "OTHER"}
};
int callback(short* wav, int num, espeak_EVENT *pEv)
{
  int cEv = 0;
  while (pEv->type) {
    cEv++;
    pEv++;
    }
  if (wav)
      qDebug() << "wav data value:" << *wav;
  qDebug("callback, events: %d\n", cEv);
  return 0;
}

void espeak_test(const char *to_sp_text)
{
  int nRate = 1;//espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 10000, NULL, 0);
  qDebug("nRate: %d\n", nRate);
  if (nRate < 0)
    return;
  espeak_SetSynthCallback(callback);
  espeak_SetParameter(espeakRATE, 125, 0);
  int rv;
  rv = espeak_Char('c');
  qDebug("rv: %d\n", rv);
  qDebug("To speak: %s\n", to_sp_text);
  const char* sText = to_sp_text;
  rv = espeak_Synth(sText, 1000, 0, POS_CHARACTER, 0, 0, 0, 0);
  qDebug("rv: %d (full: %d, internal: %d)\n",
    rv, EE_BUFFER_FULL, EE_INTERNAL_ERROR);
  sleep(2); // 2 seconds
  puts("slept");
}

}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_espeak = new QEspeak(nullptr);
    //espeak_Initialize(AUDIO_OUTPUT_PLAYBACK/*AUDIO_OUTPUT_PLAYBACK*/, 10000, NULL, 0);
}

MainWindow::~MainWindow()
{
    //espeak_Terminate();
    delete m_espeak;
    delete ui;
}

void MainWindow::recvEvent(const espeak_EVENT *event_type)
{
    qDebug() <<(event_type->type );
    if (event_type->type < sizeof(event_tables))
        ui->eventListWidget->addItem(event_tables[event_type->type].event_str);
}

void MainWindow::on_playBtn_clicked()
{
    m_espeak->setSpeakTextSize(1000);
    m_espeak->setAge((uchar)ui->ageSpinBox->value());
    m_espeak->setGender((uchar)ui->genderComBox->currentIndex()+1);
    qDebug() <<languege_maps[ui->VoiceCombox->currentIndex()].languege_str;
    m_espeak->setLanguage(languege_maps[ui->VoiceCombox->currentIndex()].languege_str);
    qDebug() << ui->ageSpinBox->value() << "," << ui->genderComBox->currentIndex();
    m_espeak->setSpeakText(ui->speak_text->toPlainText());
    m_espeak->speak();
    /*  Set the proprties of the voice.  */
//    espeak_VOICE m_voice_spec;
//    m_voice_spec.age = 50;
//    m_voice_spec.gender = 2;
//    espeak_SetVoiceByProperties(&m_voice_spec);
//    //
//    espeak_test(ui->speak_text->toPlainText().toStdString().c_str());
}

void MainWindow::on_speakWavBtn_clicked()
{
//    QString filename = QFileDialog::getOpenFileName(this);
//    qDebug() << filename;
//    if (filename.size())
//        m_espeak->speakFile(filename);
}

void MainWindow::on_VolumeHSdr_sliderMoved(int position)
{
    static int o_position = 0;
    int val = position - o_position;
    o_position = position;
    qDebug() << "update volume:" << val;
    m_espeak->updateValume(val);
}

void MainWindow::on_RateHSdr_sliderMoved(int position)
{
    static int o_position = 0;
    int val = position - o_position;
    o_position = position;
    qDebug() << "update rate:" << val;
    m_espeak->setRate(val);
}

void MainWindow::on_showEventChkBox_clicked()
{
    if (ui->showEventChkBox->isChecked()) {
        connect(m_espeak, &QEspeak::sendEvent, this, &MainWindow::recvEvent);
    } else {
        disconnect(m_espeak, &QEspeak::sendEvent, this, &MainWindow::recvEvent);
    }
}
