#include "include/qespeak.h"
#include <functional>

FILE *f_wavfile;
static QEspeak *m_instance = nullptr;
//  be used to read and write wav data.
static void Write4Bytes(FILE *f, int value)
{//=================================
// Write 4 bytes to a file, least significant first
	int ix;

	for(ix=0; ix<4; ix++)
	{
		fputc(value & 0xff,f);
		value = value >> 8;
	}
}

FILE* OpenWavFile(const char *path, int rate)
//===================================
{
    FILE *fp;
	static unsigned char wave_hdr[44] = {
		'R','I','F','F',0x24,0xf0,0xff,0x7f,'W','A','V','E','f','m','t',' ',
		0x10,0,0,0,1,0,1,0,  9,0x3d,0,0,0x12,0x7a,0,0,
		2,0,0x10,0,'d','a','t','a',  0x00,0xf0,0xff,0x7f};

    if(path == nullptr)
        return nullptr;

	while(isspace(*path)) path++;

    fp = nullptr;
	if(path[0] != 0)
	{
		if(strcmp(path,"stdout")==0)
			fp = stdout;
		else
			fp = fopen(path,"wb");
	}
	
    if(fp == nullptr)
	{
		fprintf(stderr,"Can't write to: '%s'\n",path);
        return nullptr;
	}


	fwrite(wave_hdr,1,24,fp);
	Write4Bytes(fp,rate);
	Write4Bytes(fp,rate * 2);
	fwrite(&wave_hdr[32],1,12,fp);
	return fp;
}   //  end of OpenWavFile

static int CloseWavFile(FILE **f_wavfile)
//========================
{
	unsigned int pos;

    if((*f_wavfile==nullptr) || (*f_wavfile == stdout))
        return -1;

	fflush(*f_wavfile);
	pos = ftell(*f_wavfile);

	fseek(*f_wavfile,4,SEEK_SET);
	Write4Bytes(*f_wavfile,pos - 8);

	fseek(*f_wavfile,40,SEEK_SET);
	Write4Bytes(*f_wavfile,pos - 44);

	fclose(*f_wavfile);
    *f_wavfile = nullptr;
    return 0;

} // end of CloseWavFile

/*  completed   */
QEspeak::QEspeak(const char *_esp_data_path)
    :   espeak_data_path(_esp_data_path)
{
    init_output = AUDIO_OUTPUT_PLAYBACK;
    init_ret = espeak_Initialize(init_output, 10000, espeak_data_path, 0);
    m_qMediaPlayer = new QMediaPlayer(this);
    m_instance = this;
    sp_text_size = 100;
    voice_language = "zh";
    pp_voice_spec_list = espeak_ListVoices(NULL);

}

/*  completed   */
QEspeak::~QEspeak()
{
    reset();
    espeak_Terminate();
    m_instance = nullptr;
}

int QEspeak::setSpeakFile(const QString& filename)
{
    play_file_name = filename;
    return 0;
}

int QEspeak::setVoice(const QString& voice)
{
    voice_name = voice;
    return set_voice_ret = espeak_SetVoiceByName(voice_name.toUtf8().toStdString().c_str());
}

/*  completed   */
int QEspeak::setRate(int rate)
{
    return set_para_ret = espeak_SetParameter(espeakRATE, rate, 1);
}

int QEspeak::setFormat(int format)
{
    /*
    *   In this version, we don't choose the method to play the media.
    */
    return 0;
}

int QEspeak::setGender(uchar gender)
{
    m_gender = gender;
    m_voice_spec.gender = m_gender;
    return espeak_SetVoiceByProperties(&m_voice_spec);
}

int QEspeak::setAge(uchar age)
{
    m_age = age;
    m_voice_spec.age = m_age;
    return espeak_SetVoiceByProperties(&m_voice_spec);
}

int QEspeak::setVariant(uchar var)
{
    m_variant = var;
    m_voice_spec.variant = m_variant;
    return espeak_SetVoiceByProperties(&m_voice_spec);
}

int QEspeak::setLanguage(const QString &language)
{
    voice_language = language;
    m_voice_spec.languages = voice_language.toStdString().c_str();
    return espeak_SetVoiceByProperties(&m_voice_spec);
}

int QEspeak::setVoiceName(const QString &voicename)
{
    voice_name = voicename;
    m_voice_spec.name = voice_name.toStdString().c_str();
    return espeak_SetVoiceByProperties(&m_voice_spec);
}

/*  completed   */
int QEspeak::setSpeakText(const QString& text)
{
    to_speak_text = text;
    return 0;
}

int QEspeak::setVoiceByProperties()
{
    return espeak_SetVoiceByProperties(&m_voice_spec);
}

int QEspeak::setVoiceByProperties(const QString& name, const QString& language, uchar gender, uchar age, uchar variant)
{
    voice_name = name;
    voice_language = language;
    m_gender = gender;
    m_age = age;
    m_variant = variant;
    //  to do.
    m_voice_spec.name = voice_name.toStdString().c_str();
    m_voice_spec.languages = voice_language.toStdString().c_str();
    m_voice_spec.gender = m_gender;
    m_voice_spec.age = m_age;
    m_voice_spec.variant = m_variant;
    return espeak_SetVoiceByProperties(&m_voice_spec);
}

int QEspeak::setOutputMethod(espeak_AUDIO_OUTPUT output)
{
    init_output = output;
}

int QEspeak::reInitialize()
{
    reset();
    espeak_Terminate();
    return init_ret = espeak_Initialize(init_output, 10000, espeak_data_path, 0);
}

/*  completed   */
void QEspeak::updateValume(int val)
{
    m_qMediaPlayer->setVolume(m_qMediaPlayer->volume()+val);
    set_para_ret = espeak_SetParameter(espeakVOLUME, /*val*/val, 1);
}

int QEspeak::saveToFile(const QString& filename)
{
    reset();    //  reset the play status.
    //OpenWavFile(filename.toStdString().c_str(),)
    /*  Start a new speaking work.  */
    save_file_name = filename;
    save_file_flag = 1;
    espeak_SetSynthCallback(saveToFileCallback);
    espeak_Synth(to_speak_text.toStdString().c_str(), sp_text_size, 0, POS_CHARACTER, 0, 0, nullptr, nullptr);
    espeak_Synchronize();
    return CloseWavFile(&f_wavfile);
}

/*  completed   */
int QEspeak::speakText(const QString& text)
{
    to_speak_text = text;
    reset();
    espeak_SetSynthCallback(directSpeakCallback);
    return espeak_Synth(to_speak_text.toStdString().c_str(), sp_text_size, 0, POS_CHARACTER, 0, 0, 0, 0);
}

/* In this function, we don't use the espeak api to speak. */
/*  completed   */
int QEspeak::speakFile(const QString& filename)
{
    reset();
    m_qMediaPlayer->setMedia(QMediaContent(QUrl::fromLocalFile("./sunflower-slow-drag.ogg")));
    m_qMediaPlayer->play();
    return 0;
}

size_t QEspeak::setSpeakTextSize(size_t size)
{
    return sp_text_size = size;
}

int QEspeak::togglePause()
{
    /*
    *   In this function, we should do some works.
    *   Because in the espeak api, there isn't no function to do it.
    */

    /*
    *   1. Save the speak position of the text. Then call the Cancel function.
    *       And we don't change the other data.
    *   2. If current state is paused, we resume it from the paused speak position.
    *       If current state isn't paused, we pause it and save the current speak position.
    */
    if (pause_flag) {
        pause_flag = 0;
        pause_speak_position = nullptr;
    } else {
        pause_flag = 1;
        pause_speak_position = nullptr;
    }
    return 0;
}

int QEspeak::speak()
{
    reset();
    espeak_SetSynthCallback(directSpeakCallback);
    return espeak_Synth(to_speak_text.toStdString().c_str(), sp_text_size, 0, POS_CHARACTER, 0, 0, 0, 0);
}

/*  completed   */
int QEspeak::stop()
{
    return reset();
}

/*  completed   */
int QEspeak::reset()
{
    /*  Stop the speak .wav file. */
    m_qMediaPlayer->stop();

    /*  Stop the speaking.  */
    return espeak_Cancel();
}

QString QEspeak::save_file_name = "";
int QEspeak::saveToFileCallback(short *wav, int num, espeak_EVENT *pEv)
{
    static int samplerate = 0;
    static unsigned int samples_split = 0;
    static unsigned int samples_split_seconds = 0;
    if (wav == nullptr) {
        CloseWavFile(&f_wavfile);
        return 0;
    }
    while (pEv && pEv->type) {
        if (pEv->type == espeakEVENT_SAMPLERATE) {
			samplerate = pEv->id.number;
			samples_split = samples_split_seconds * samplerate;
		}
        if (f_wavfile == NULL) {
            if (samples_split > 0) {
                if (!(f_wavfile = OpenWavFile(save_file_name.toStdString().c_str(), samplerate)))
                    return -1;
            } else {
                if (!(f_wavfile = OpenWavFile(save_file_name.toStdString().c_str(), samplerate)))
                    return -1;
            }
        }
        //  write data into the file.
        if (f_wavfile && num)
            fwrite(wav,num*2,1,f_wavfile);  //  have some, write some to file.
        if (m_instance)
            emit m_instance->sendEvent(pEv);
    }
    return 0;
}

int QEspeak::directSpeakCallback(short *wav, int num, espeak_EVENT *pEv)
{
    if (wav && num)
        ;
    while (pEv && pEv->type) {
        //  write data into the file.
        if (m_instance)
            emit m_instance->sendEvent(pEv);
        pEv++;
    }
    return 0;
}

const espeak_VOICE *QEspeak::searchVoiceSpecByLanguege(const QString &languege)
{
    for (int i = 0; pp_voice_spec_list[i]; i++) {
        if (languege == pp_voice_spec_list[i]->languages)
            return pp_voice_spec_list[i];
    }
    return nullptr;
}
