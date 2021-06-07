#ifndef QESPEAK_H
#define QESPEAK_H

#include <QObject>
extern "C" {
#include <speak_lib.h>

#ifdef __WIN32__
#include <windows.h>
#define sleep(x) Sleep(1000*x)
#endif
}
class QEspeak
{
    Q_OBJECT

public:
    QEspeak();
    virtual ~QEspeak();
    int setSpeakFile(QString filename);
    int setVoice(QString voice);
    int setRate(int rate);
    int setFormat(int format);
    int setSpeakText(QString text);

public slots:
    void updateValume(int val);
    int saveToFile(QString filename);
    int speakText(QString text);
    int speakFile(QString filename);
    int togglePause();
    int stop();
    int reset();
signals:
    void sendEvent(int event_type);
private:
    QString save_file_name;
    QString to_speak_text;
    int valume;

};

#endif // QESPEAK_H