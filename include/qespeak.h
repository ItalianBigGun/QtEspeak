#ifndef QESPEAK_H
#define QESPEAK_H

#include <QObject>
#include <QMediaPlayer>
extern "C" {
#include <speak_lib.h>

#ifdef __WIN32__
#include <windows.h>
#define sleep(x) Sleep(1000*x)
#endif
}

class QEspeak : public QObject
{
    Q_OBJECT

public:
    QEspeak(const char *_esp_data_path);
    virtual ~QEspeak();
    int setSpeakFile(const QString& filename);

    int setRate(int rate);
    void updateValume(int val);
    int setFormat(int format);

    int setGender(uchar gender);
    int setAge(uchar age);
    int setVariant(uchar var);
    int setLanguage(const QString& language);
    int setVoiceName(const QString& voicename);
    int setVoice(const QString& voice);

    int setSpeakText(const QString& text);
    int setVoiceByProperties();
    int setVoiceByProperties(const QString& voicename, const QString& language, uchar gender, uchar age, uchar variant);
    int setOutputMethod(espeak_AUDIO_OUTPUT output);
    int reInitialize();
    int saveToFile(const QString& filename);
    int speakText(const QString& text);
    int speakFile(const QString& filename);
    size_t setSpeakTextSize(size_t size);
public slots:

    int togglePause();
    int speak();
    int stop();
    int reset();
private:
    static int saveToFileCallback(short* wav, int num, espeak_EVENT *pEv);
    static int directSpeakCallback(short* wav, int num, espeak_EVENT *pEv);
    const espeak_VOICE *searchVoiceSpecByLanguege(const QString &languege);
signals:
    void sendEvent(const espeak_EVENT *event_type);
private:
    /*
    *    m_qMediaPlayer is used to play the .wav file under the Qt.
    */
    QMediaPlayer *m_qMediaPlayer;
    QString play_file_name;
    //

    static QString save_file_name;
    int save_file_flag;
    QString to_speak_text;
    const char *pause_speak_position;
    int pause_flag;

    QString voice_name;
    QString voice_language;
    uchar m_gender, m_age, m_variant;
    int set_voice_ret;

    /*
    *   Below parameters are used for espeak_SetParameter function.
    *   Set the value of the para.
    *
    *   espeakRATE:    说话速度，以每分钟的字来表示。值80到450。
    *   espeakVOLUME:  音量范围0-200或更多。
    *                 0=沉默, 100=正常的满音量，较大的值可能产生振幅压缩或失真
    *   espeakPITCH:   基音，范围0-100。50 =正常
    *   espeakRANGE:   螺距范围，范围0-100。0=单调 50=正常
    *
    *   espeakWORDGAP:  单词之间的停顿，单位为10mS(默认速度)
    */
    int para_rate_value;            //  值80到450。
    int para_valume_value;      //  音量范围0-200或更多。
    int para_pitch_value;       //  基音，范围0-100。50 =正常
    int para_range_value;       //  螺距范围，范围0-100。0=单调 50=正常
    int para_workgap_value;   //  单词之间的停顿，单位为10mS(默认速度)
    int set_para_ret;
    //

    // for espeak_Initialize();
    espeak_AUDIO_OUTPUT init_output;
    int init_buflenght;
    char *init_path;
    int init_options;
    int init_ret;
    //
    const char *espeak_data_path;

    //
    espeak_VOICE m_voice_spec;
    const espeak_VOICE **pp_voice_spec_list;
    size_t sp_text_size;

};

#endif // QESPEAK_H
