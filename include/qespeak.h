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
public slots:
    void updateValume(int val);
    int saveToFile(QString filename);
    int speakText(QString text);
    int speakFile(QString filename);
private:
    QString save_file_name;
    QString to_speak_text;
    int valume;

};

#endif // QESPEAK_H