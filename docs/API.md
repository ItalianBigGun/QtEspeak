 ## 一. espeak库API
***************<br/>
### 1. 初始化函数
===================================================================================<br/>
    ESPEAK_API int espeak_Initialize(espeak_AUDIO_OUTPUT output, int buflength, const char *path, int options);<br/>
翻：<br/>
    必须在调用任何合成函数之前调用。<br/>
    output: 音频数据可以由eSpeak播放，也可以由SynthCallback函数传递回来。<br/>
<br/>
    buflength:  传递给SynthCallback函数的声音缓冲区的长度(mS)。<br/>
            Value=0 设置默认值200mS.<br/>
            此参数仅用于AUDIO_OUTPUT_RETRIEVAL和AUDIO_OUTPUT_SYNCHRONOUS模式。<br/>
<br/>
    path: 包含speak-data目录的目录，默认位置为NULL。<br/>
<br/>
    options: bit 0:  1=allow espeakEVENT_PHONEME事件。<br/>
            bit 1:  1=espeakEVENT_PHONEME 事件给出IPA音素名，而不是eSpeak音素名<br/>
            bit 15: 1=don't exit 如果没有找到espeak_data (used for --help)<br/>
<br/>
    Returns: 采样速率(Hz), or -1 (EE_INTERNAL_ERROR).<br/>
<br/>
原：<br/>
    Must be called before any synthesis functions are called.<br/>
    output: the audio data can either be played by eSpeak or passed back by the SynthCallback function.<br/>
<br/>
    buflength:  The length in mS of sound buffers passed to the SynthCallback function.<br/>
            Value=0 gives a default of 200mS.<br/>
            This paramater is only used for AUDIO_OUTPUT_RETRIEVAL and AUDIO_OUTPUT_SYNCHRONOUS modes.<br/>
<br/>
    path: The directory which contains the espeak-data directory, or NULL for the default location.<br/>
<br/>
    options: bit 0:  1=allow espeakEVENT_PHONEME events.<br/>
            bit 1:  1= espeakEVENT_PHONEME events give IPA phoneme names, not eSpeak phoneme names<br/>
            bit 15: 1=don't exit if espeak_data is not found (used for --help)<br/>
<br/>
    Returns: sample rate in Hz, or -1 (EE_INTERNAL_ERROR).<br/>
<br/>
### 2. 设置同步回调函数<br/>
===================================================================================<br/>
    ESPEAK_API void espeak_SetSynthCallback(t_espeak_callback* SynthCallback);<br/>
翻：<br/>
    必须在调用任何合成函数之前调用。<br/>
    这指定了调用程序中的一个函数，当缓冲区<br/>
    语音数据已经产生，函数会被调用。<br/>
<br/>
    回调函数的形式如下:<br/>
<br/>
    int SynthCallback(short *wav, int numsamples, espeak_EVENT *events);<br/>
<br/>
    wav:  代表已产生的语音声音数据。<br/>
      NULL 表示合成已经完成。<br/>

    numsamples: 代表wav中的条目数。这个数字可以变化，可以小于<br/>
        espeak_Initialize中给出的buflength参数所隐含的值，并且可以<br/>
        有时为零(这并不表示合成的结束)。   <br/>
<br/>
    events: 一个espeak_EVENT项数组，它指示单词和句子事件，以及<br/>
        如果\<mark\>和\<audio\>元素在文本中出现。列表中<br/>
        事件会被类型为0的事件终止。<br/>
<br/>
<br/>
    Callback returns: 0=继续合成,  1=中止合成.<br/>
<br/>
原：<br/>
    Must be called before any synthesis functions are called.<br/>
    This specifies a function in the calling program which is called when a buffer of<br/>
    speech sound data has been produced.<br/>
<br/>
<br/>
    The callback function is of the form:<br/>
<br/>
    int SynthCallback(short *wav, int numsamples, espeak_EVENT *events);<br/>
<br/>
    wav:  is the speech sound data which has been produced.<br/>
      NULL indicates that the synthesis has been completed.<br/>
<br/>
    numsamples: is the number of entries in wav.  This number may vary, may be less than<br/>
      the value implied by the buflength parameter given in espeak_Initialize, and may<br/>
      sometimes be zero (which does NOT indicate end of synthesis).<br/>
<br/>
    events: an array of espeak_EVENT items which indicate word and sentence events, and<br/>
      also the occurance if \<mark\> and \<audio\> elements within the text.  The list of<br/>
      events is terminated by an event of type = 0.<br/>
<br/>
<br/>
    Callback returns: 0=continue synthesis,  1=abort synthesis.<br/>
<br/>
3. 设置同步回调函数<br/>
===================================================================================<br/>
ESPEAK_API void espeak_SetUriCallback(int (*UriCallback)(int, const char*, const char*));<br/>
<br/>
翻：<br/>
    此函数可以在使用合成函数之前调用，以便处理<br/>
    <音频>标签。它指定了一个回调函数，当\<audio\>元素为<br/>
    遇到并允许调用程序指示声音文件是否<br/>
    在\<audio\>元素中指定。<br/>
<br/>
    回调函数的形式是:<br/>
<br/>
    int UriCallback(int type, const char *uri, const char *base);<br/>
<br/>
    type:  回调事件的类型。目前只有1= \<audio\>元素<br/>
<br/>
    uri:   \<audio\>元素中的"src"属性<br/>
<br/>
    base:  \<speak\>元素的"xml:base"属性(如果有的话)<br/>
<br/>
    Return: 1=不要播放声音，除非要说的文本被替代。<br/>
           0=在事件列表中\<audio\>元素处放置一个PLAY事件<br/>
              发生。然后调用程序可以在此时播放声音。<br/>
<br/>
原：
    This function may be called before synthesis functions are used, in order to deal with
    \<audio\> tags.  It specifies a callback function which is called when an \<audio\> element is
    encountered and allows the calling program to indicate whether the sound file which
    is specified in the \<audio\> element is available and is to be played.

    The callback function is of the form:

    int UriCallback(int type, const char *uri, const char *base);

    type:  type of callback event.  Currently only 1= \<audio\> element

    uri:   the "src" attribute from the \<audio\> element

    base:  the "xml:base" attribute (if any) from the \<speak\> element

    Return: 1=don't play the sound, but speak the text alternative.
           0=place a PLAY event in the event list at the point where the \<audio\> element
             occurs.  The calling program can then play the sound at that point.

4. 合成语音函数
===================================================================================
ESPEAK_API espeak_ERROR espeak_Synth(const void *text,
	size_t size,
	unsigned int position,
	espeak_POSITION_TYPE position_type,
	unsigned int end_position,
	unsigned int flags,
	unsigned int* unique_identifier,
	void* user_data);
翻：
    为指定的文本合成语音。语音声音数据被传递给呼叫方
    通过espeak_SetSynthCallback()指定的回调函数在缓冲区中执行程序。
    该命令是异步的:它在内部被缓冲并尽快返回。
    如果之前使用AUDIO_OUTPUT_PLAYBACK作为参数调用了espeak_Initialize，声音数据将由eSpeak播放。

    text: 以零字符结束的要讲的文本。它可以是8位字符，
        宽字符(wchar_t)或UTF8编码。这些是由"旗帜"决定的
        参数。

    size: 等于(或大于)文本数据的大小，以字节为单位。这是按顺序使用的
        为文本分配内部存储空间。此值不用于
        AUDIO_OUTPUT_SYNCHRONOUS模式。

    position:  课文中开始说话的位置。零表示从
        正文的开头。

    position_type:  确定“position”是否为字符、单词或句子的数量。
      Values:

    end_position:  如果设置了，这将给出一个字符位置，在此位置将停止说话。一个值
            0表示没有结束位置。

    flags:  These may be OR'd together:
      Type of character codes, one of:
         espeakCHARS_UTF8     UTF8 encoding
         espeakCHARS_8BIT     The 8 bit ISO-8859 character set for the particular language.
         espeakCHARS_AUTO     8 bit or UTF8  (this is the default)
         espeakCHARS_WCHAR    Wide characters (wchar_t)
         espeakCHARS_16BIT    16 bit characters.

      espeakSSML   Elements within < > are treated as SSML elements, or if not recognised are ignored.

      espeakPHONEMES  Text within [[ ]] is treated as phonemes codes (in espeak's Hirshenbaum encoding).

      espeakENDPAUSE  If set then a sentence pause is added at the end of the text.  If not set then
         this pause is suppressed.

    unique_identifier: 它必须是NULL，或者指向一个整型变量
        eSpeak编写消息标识符号。
        eSpeak在espeak_EVENT消息中包含这个数字，这些消息是结果
        espeak_Synth()的调用。

    user_data: 一个指针(或NULL)，它将被传递给回调函数
            espeak_EVENT消息。     

    Return: 
        EE_OK: 操作实现
        EE_BUFFER_FULL: 命令不能被缓冲;
                您可以稍后再尝试调用该函数。
	   EE_INTERNAL_ERROR.

原：
    Synthesize speech for the specified text.  The speech sound data is passed to the calling
    program in buffers by means of the callback function specified by espeak_SetSynthCallback(). 
    The command is asynchronous: it is internally buffered and returns as soon as possible. 
    If espeak_Initialize was previously called with AUDIO_OUTPUT_PLAYBACK as argument, the sound data are played by eSpeak.

    text: The text to be spoken, terminated by a zero character. It may be either 8-bit characters,
      wide characters (wchar_t), or UTF8 encoding.  Which of these is determined by the "flags"
      parameter.

    size: Equal to (or greatrer than) the size of the text data, in bytes.  This is used in order
      to allocate internal storage space for the text.  This value is not used for
      AUDIO_OUTPUT_SYNCHRONOUS mode.

    position:  The position in the text where speaking starts. Zero indicates speak from the
      start of the text.

    position_type:  Determines whether "position" is a number of characters, words, or sentences.
      Values:

    end_position:  If set, this gives a character position at which speaking will stop.  A value
      of zero indicates no end position.

    flags:  These may be OR'd together:
      Type of character codes, one of:
         espeakCHARS_UTF8     UTF8 encoding
         espeakCHARS_8BIT     The 8 bit ISO-8859 character set for the particular language.
         espeakCHARS_AUTO     8 bit or UTF8  (this is the default)
         espeakCHARS_WCHAR    Wide characters (wchar_t)
         espeakCHARS_16BIT    16 bit characters.

      espeakSSML   Elements within < > are treated as SSML elements, or if not recognised are ignored.

      espeakPHONEMES  Text within [[ ]] is treated as phonemes codes (in espeak's Hirshenbaum encoding).

      espeakENDPAUSE  If set then a sentence pause is added at the end of the text.  If not set then
         this pause is suppressed.

    unique_identifier: This must be either NULL, or point to an integer variable to
       which eSpeak writes a message identifier number.
       eSpeak includes this number in espeak_EVENT messages which are the result of
       this call of espeak_Synth().

    user_data: a pointer (or NULL) which will be passed to the callback function in
       espeak_EVENT messages.

    Return: EE_OK: operation achieved
           EE_BUFFER_FULL: the command can not be buffered;
             you may try after a while to call the function again.
	   EE_INTERNAL_ERROR.

5. 设置同步回调函数
===================================================================================
ESPEAK_API espeak_ERROR espeak_Synth_Mark(const void *text,
	size_t size,
	const char *index_mark,
	unsigned int end_position,
	unsigned int flags,
	unsigned int* unique_identifier,
	void* user_data);
翻：
    为指定的文本合成语音。类似于espeak_Synth()，但起始位置是
    由文本中<mark>元素的名称指定。

    index_mark:  在文本中，<mark>元素的"name"属性指定
        合成开始的点。UTF8字符串。

    其他参数, 请查看 espeak_Synth()

    Return: EE_OK:  操作实现
           EE_BUFFER_FULL: 命令不能被缓冲;
                您可以稍后再尝试调用该函数。
	   EE_INTERNAL_ERROR.

原：
    Synthesize speech for the specified text.  Similar to espeak_Synth() but the start position is
    specified by the name of a <mark> element in the text.

    index_mark:  The "name" attribute of a <mark> element within the text which specified the
      point at which synthesis starts.  UTF8 string.

    For the other parameters, see espeak_Synth()

    Return: EE_OK: operation achieved
           EE_BUFFER_FULL: the command can not be buffered;
             you may try after a while to call the function again.
	   EE_INTERNAL_ERROR.

6. 设置同步回调函数
===================================================================================
ESPEAK_API espeak_ERROR espeak_Key(const char *key_name);
翻：
    说出一个键盘键的名称。
    如果key_name是单个字符，则表示该字符的名称。
    否则，它将key_name作为文本字符串。

    Return: EE_OK:  操作实现
           EE_BUFFER_FULL: 命令不能被缓冲;
                您可以稍后再尝试调用该函数。
	   EE_INTERNAL_ERROR.

原：
    Speak the name of a keyboard key.
    If key_name is a single character, it speaks the name of the character.
    Otherwise, it speaks key_name as a text string.

    Return: EE_OK: operation achieved
           EE_BUFFER_FULL: the command can not be buffered;
             you may try after a while to call the function again.
	   EE_INTERNAL_ERROR.


7. ？函数
===================================================================================
ESPEAK_API espeak_ERROR espeak_Char(wchar_t character);
翻：
    说出给定角色的名字

    Return: EE_OK:  操作实现
           EE_BUFFER_FULL: 命令不能被缓冲;
                您可以稍后再尝试调用该函数。
	   EE_INTERNAL_ERROR.

原：
    Speak the name of the given character

    Return: EE_OK: operation achieved
           EE_BUFFER_FULL: the command can not be buffered;
             you may try after a while to call the function again.
	   EE_INTERNAL_ERROR.

8. 设置参数函数
===================================================================================
ESPEAK_API espeak_ERROR espeak_SetParameter(espeak_PARAMETER parameter, int value, int relative);
翻：
    设置标识的参数的值
        relative=0   设置参数的绝对值。
        relative=1   设置参数的相对值。

    parameter:
      espeakRATE:    说话速度，以每分钟的字来表示。值80到450。

      espeakVOLUME:  音量范围0-200或更多。
                     0=沉默, 100=正常的满音量，较大的值可能产生振幅压缩或失真

      espeakPITCH:   基音，范围0-100。50 =正常

      espeakRANGE:   螺距范围，范围0-100。0=单调 50=正常

      espeakPUNCTUATION:  要宣布的标点字符:
         value in espeak_PUNCT_TYPE (none, all, some),
         请参见espeak_GetParameter()来指定要宣布的字符。

      espeakCAPITALS: 以以下方式宣布大写字母:
         0=none,
         1=sound icon,
         2=spelling,
         3 或更高，通过提高音高。这个值给出了以Hz为单位的音高
            表示一个单词的大写字母。

      espeakWORDGAP:  单词之间的停顿，单位为10mS(默认速度)

    Return: EE_OK:  操作实现
           EE_BUFFER_FULL: 命令不能被缓冲;
                您可以稍后再尝试调用该函数。
	   EE_INTERNAL_ERROR.

原：
    Sets the value of the specified parameter.
        relative=0   Sets the absolute value of the parameter.
        relative=1   Sets a relative value of the parameter.

    parameter:
      espeakRATE:    speaking speed in word per minute.  Values 80 to 450.

      espeakVOLUME:  volume in range 0-200 or more.
                     0=silence, 100=normal full volume, greater values may produce amplitude compression or distortion

      espeakPITCH:   base pitch, range 0-100.  50=normal

      espeakRANGE:   pitch range, range 0-100. 0-monotone, 50=normal

      espeakPUNCTUATION:  which punctuation characters to announce:
         value in espeak_PUNCT_TYPE (none, all, some),
         see espeak_GetParameter() to specify which characters are announced.

      espeakCAPITALS: announce capital letters by:
         0=none,
         1=sound icon,
         2=spelling,
         3 or higher, by raising pitch.  This values gives the amount in Hz by which the pitch
            of a word raised to indicate it has a capital letter.

      espeakWORDGAP:  pause between words, units of 10mS (at the default speed)

    Return: EE_OK: operation achieved
           EE_BUFFER_FULL: the command can not be buffered;
             you may try after a while to call the function again.
	   EE_INTERNAL_ERROR.

9. 获取参数函数
===================================================================================
ESPEAK_API int espeak_GetParameter(espeak_PARAMETER parameter, int current);
翻：
    current=0  返回指定参数的默认值。
    current=1  返回由SetParameter()设置的指定参数的当前值。

原：
    current=0  Returns the default value of the specified parameter.
    current=1  Returns the current value of the specified parameter, as set by SetParameter()

10. 设置标点符号列表函数
===================================================================================
ESPEAK_API espeak_ERROR espeak_SetPunctuationList(const wchar_t *punctlist);
翻：
    标识要读出其名称的标点字符列表，
        当标点参数的值设置为“some”。

    punctlist:  以零字符结束的字符代码列表。

    Return: EE_OK:  操作实现
           EE_BUFFER_FULL: 命令不能被缓冲;
                您可以稍后再尝试调用该函数。
	   EE_INTERNAL_ERROR.

原：
    Specified a list of punctuation characters whose names are to be spoken when the
    value of the Punctuation parameter is set to "some".

    punctlist:  A list of character codes, terminated by a zero character.

    Return: EE_OK: operation achieved
           EE_BUFFER_FULL: the command can not be buffered;
             you may try after a while to call the function again.
	   EE_INTERNAL_ERROR.

11. 设置音素跟踪函数
===================================================================================
ESPEAK_API void espeak_SetPhonemeTrace(int value, FILE *stream);
翻：
    控制文本的音素符号的输出
        value=0  没有音素输出(默认)
        value=1  输出文本的翻译音素符号
        value=2  as(1)，但还输出翻译如何完成的跟踪(匹配规则和列表项)
        value=3  as(1)，但产生IPA而不是ascii音素名

    stream   音素符号(和跟踪)的输出流。如果stream=NULL，则使用stdout。

原：
    Controls the output of phoneme symbols for the text
        value=0  No phoneme output (default)
        value=1  Output the translated phoneme symbols for the text
        value=2  as (1), but also output a trace of how the translation was done (matching rules and list entries)
        value=3  as (1), but produces IPA rather than ascii phoneme names

    stream   output stream for the phoneme symbols (and trace).  If stream=NULL then it uses stdout.
12. 文本转换音素函数
===================================================================================
ESPEAK_API const char *espeak_TextToPhonemes(const void **textptr, int textmode, int phonememode);
翻：  
    将文本翻译成音素。首先调用espeak_SetVoiceByName()来选择一种语言。

    它返回一个指向字符串的指针，该字符串包含到的文本的音素
    或逗号、分号、冒号或类似的标点符号。

    textptr: 指向以零字符结束的输入文本的指针的地址。
        返回时，说明指针已经向前移动，超过了已翻译或设置的文本
        到NULL表示已经到达文本的末尾。

    textmode: 字符编码的类型，其中之一:
         espeakCHARS_UTF8     UTF8 encoding
         espeakCHARS_8BIT     The 8 bit ISO-8859 character set for the particular language.
         espeakCHARS_AUTO     8 bit or UTF8  (this is the default)
         espeakCHARS_WCHAR    Wide characters (wchar_t)
         espeakCHARS_16BIT    16 bit characters.

    phonememode: （音素模式）
        bits0-3:
        0= 只有音素。
        1= 包含超过一个字母的音素名(U+361)。
        2= 包含一个以上字母的音素名的零宽度joiner。
        3= 用下划线分隔音素。

	    bits 4-7:
        0= eSpeak的ascii音素名。
        1= 国际音标字母(UTF-8字符)。

原：  
    Translates text into phonemes.  Call espeak_SetVoiceByName() first, to select a language.

    It returns a pointer to a character string which contains the phonemes for the text up to
    end of a sentence, or comma, semicolon, colon, or similar punctuation.

    textptr: The address of a pointer to the input text which is terminated by a zero character.
      On return, the pointer has been advanced past the text which has been translated, or else set
      to NULL to indicate that the end of the text has been reached.

    textmode: Type of character codes, one of:
         espeakCHARS_UTF8     UTF8 encoding
         espeakCHARS_8BIT     The 8 bit ISO-8859 character set for the particular language.
         espeakCHARS_AUTO     8 bit or UTF8  (this is the default)
         espeakCHARS_WCHAR    Wide characters (wchar_t)
         espeakCHARS_16BIT    16 bit characters.

    phonememode: 
        bits0-3:
        0= just phonemes.
        1= include ties (U+361) for phoneme names of more than one letter.
        2= include zero-width-joiner for phoneme names of more than one letter.
        3= separate phonemes with underscore characters.

	    bits 4-7:
        0= eSpeak's ascii phoneme names.
        1= International Phonetic Alphabet (as UTF-8 characters).


13. 编译发音文档函数
===================================================================================
ESPEAK_API void espeak_CompileDictionary(const char *path, FILE *log, int flags);
翻：
    编译对应于当前语言的发音字典选定的声音。
    在调用此功能之前，需要选择所需的声音。

    path:  包含该语言的'_rules'和'_list'文件的目录。
        'path'应该以路径分隔符('/')结束。
    log:   错误报告和统计信息流。如果log=NULL则使用stderr。

    flags:  Bit 0: 包含用于调试目的的源代码行信息(这将与-X命令行选项)。

原：
    Compile pronunciation dictionary for a language which corresponds to the currently
    selected voice.  The required voice should be selected before calling this function.

    path:  The directory which contains the language's '_rules' and '_list' files.
          'path' should end with a path separator character ('/').
    log:   Stream for error reports and statistics information. If log=NULL then stderr will be used.

    flags:  Bit 0: include source line information for debug purposes (This is displayed with the
          -X command line option).
14. 获取声音列表函数
===================================================================================
ESPEAK_API const espeak_VOICE **espeak_ListVoices(espeak_VOICE *voice_spec);
翻：
    从from espeak-data/voices读取Voice文件并创建一个espeak_VOICE指针数组。
    列表以NULL指针作为中断操作。

    如果voice_spec被设置为NULL，所有的voices将会被列出。
    如果voice_spec被设置有效，则只有和voice_spec相匹配的才会被列出，并按照一定优先级排序。

原：
    Reads the voice files from espeak-data/voices and creates an array of espeak_VOICE pointers.
    The list is terminated by a NULL pointer

    If voice_spec is NULL then all voices are listed.
    If voice spec is given, then only the voices which are compatible with the voice_spec
    are listed, and they are listed in preference order.

15. 通过名字设置声音函数
===================================================================================
ESPEAK_API espeak_ERROR espeak_SetVoiceByName(const char *name);
翻：
    通过name参数去搜索和匹配一个voice。语言是不需要考虑的。name参数需要是一个UTF8字符串。

    Return: EE_OK:  操作实现
           EE_BUFFER_FULL: 命令不能被缓冲;
                您可以稍后再尝试调用该函数。
	   EE_INTERNAL_ERROR.

原：
    Searches for a voice with a matching "name" field.  Language is not considered.
    "name" is a UTF8 string.

    Return: EE_OK: operation achieved
           EE_BUFFER_FULL: the command can not be buffered;
             you may try after a while to call the function again.
	   EE_INTERNAL_ERROR.

16. 设置声音属性函数
===================================================================================
ESPEAK_API espeak_ERROR espeak_SetVoiceByProperties(espeak_VOICE *voice_spec);
翻：
    espeak_VOICE结构用于传递选择语音的标准。以下任何一项
    字段可以设置:

    name     NULL, 或者是一个voice名。

    languages  NULL, 或者是一个语言字符串 (和可选的方言), eg. "en-uk", or "en"

    gender   0=not specified, 1=male, 2=female

    age      0=not specified, or an age in years

    variant  在生成候选列表、评分和排序后，使用“variant”进行索引
            然后选择一个声音。
            variant=0 获取最前面的voice (i.e. best match). variant=1 获取下一个voice, etc

原：
    An espeak_VOICE structure is used to pass criteria to select a voice.  Any of the following
    fields may be set:

    name     NULL, or a voice name

    languages  NULL, or a single language string (with optional dialect), eg. "en-uk", or "en"

    gender   0=not specified, 1=male, 2=female

    age      0=not specified, or an age in years

    variant  After a list of candidates is produced, scored and sorted, "variant" is used to index
            that list and choose a voice.
            variant=0 takes the top voice (i.e. best match). variant=1 takes the next voice, etc

17. 获取当前声音函数
===================================================================================
ESPEAK_API espeak_VOICE *espeak_GetCurrentVoice(void);
翻：
    返回当前所选voice的espeak_VOICE数据。
    这不会受到SSML元素(如\<voice\>,\<s\>)引起的临时语音变化的影响。
原：
    Returns the espeak_VOICE data for the currently selected voice.
    This is not affected by temporary voice changes caused by SSML elements such as \<voice\> and \<s\>

18. 结束合成语音函数
===================================================================================
ESPEAK_API espeak_ERROR espeak_Cancel(void);
翻：
    立即停止当前文本的合成和音频输出。当这个
    函数返回，音频输出完全停止，合成器准备好
    合成一条新信息。

    Return: EE_OK: 操作完成
	   EE_INTERNAL_ERROR.
原：
    Stop immediately synthesis and audio output of the current text. When this
    function returns, the audio output is fully stopped and the synthesizer is ready to
    synthesize a new message.

    Return: EE_OK: operation achieved
	   EE_INTERNAL_ERROR.

19. 判断播放函数
===================================================================================
ESPEAK_API int espeak_IsPlaying(void);
翻：
    声音被播放返回1，其他返回0。
原：
    Returns 1 if audio is played, 0 otherwise.

20. 等待同步结束函数
===================================================================================
ESPEAK_API espeak_ERROR espeak_Synchronize(void);
翻：
    这个函数在所有数据都播放完后返回。
    Return: EE_OK: 操作完成
	   EE_INTERNAL_ERROR.
原：
    This function returns when all data have been spoken.
    Return: EE_OK: operation achieved
	   EE_INTERNAL_ERROR.

21. 去初始化函数
===================================================================================
ESPEAK_API espeak_ERROR espeak_Terminate(void);
翻：
    API释放时，调用的函数。
    Return: EE_OK: 操作完成
	   EE_INTERNAL_ERROR.
原：
    last function to be called.
    Return: EE_OK: operation achieved
	   EE_INTERNAL_ERROR.

22. 获取版本信息函数
===================================================================================
ESPEAK_API const char *espeak_Info(const char **path_data);
翻：
    返回版本号字符串。
    path_data  返回espak_data的路径
原：
    Returns the version number string.
    path_data  returns the path to espeak_data
