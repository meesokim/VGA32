class AY8910 {
    #define AY8910_BASE     111861 /* Base frequency for AY8910  */
    #define AY8910_CHANNELS 6      /* 3 melodic + 3 noise chanls */

    #define AY8910_ASYNC    0      /* Asynchronous emulation     */
    #define AY8910_SYNC     1      /* Synchronous emulation      */
    #define AY8910_FLUSH    2      /* Flush buffers only         */
    #define AY8910_DRUMS    0x80   /* Hit drums for noise chnls  */

    #ifndef uint8_t_TYPE_DEFINED
    #define uint8_t_TYPE_DEFINED
    typedef unsigned char uint8_t;
    #endif 
                                /* SetSound() arguments:      */
    #define SND_MELODIC     0      /* Melodic sound (default)    */
    #define SND_RECTANGLE   0      /* Rectangular wave           */
    #define SND_TRIANGLE    1      /* Triangular wave (1/2 rect.)*/
    #define SND_NOISE       2      /* White noise                */    
    /**
     * Sound Queue entry
     */
    typedef struct {
        int chn;
        int freq;
        int vol;
        int time; // in ms
    } TSndQEntry;    
    #define MAX_SNDQ 1000	// 1000 is enough for most cases
    /**
     * Sound Queue structure
     */
    typedef struct {
        int front;
        int rear;
        TSndQEntry qentry[MAX_SNDQ];
    } TSndQ;

    TSndQ SndQ;
    int LastBufTime = 0;
#ifdef SDL    
    SDL_mutex *sound_mutex;
#endif
    const int DEVFREQ = 44100;
    // static int Freq[6];
    int Vol[6];
    int Interval[6];
    int NoiseInterval[6];
    int Phase[6];
    int DevFreq;
    volatile int JF = 0;
    const unsigned char Envelopes[16][32] =
    {
        { 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 },
        { 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 },
        { 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15 },
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 },
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15 },
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 },
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
    };
    const int Volumes[16] =
    { 0,1,2,4,6,8,11,16,23,32,45,64,90,128,180,255 };
    typedef struct
    {
    uint8_t R[16];                  /* PSG s contents     */
    int Freq[AY8910_CHANNELS];   /* Frequencies (0 for off)    */
    int Volume[AY8910_CHANNELS]; /* Volumes (0..255)           */
    int Clock;                   /* Base clock used by PSG     */
    int First;                   /* First used Sound() channel */
    uint8_t Changed;                /* Bitmap of changed channels */
    uint8_t Sync;                   /* AY8910_SYNC/AY8910_ASYNC   */
    uint8_t Latch;                  /* Latch for the  num */
    int EPeriod;                 /* Envelope step in msecs     */
    int ECount;                  /* Envelope step counter      */
    int EPhase;                  /* Envelope phase             */
    } AY8910_REG;
public:
    AY8910_REG D;
    void reset(int first) {
        static uint8_t RegInit[16] =
        {
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFD,
            0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00
        };
        int J;
        #define ClockHz 4000000
        /* Reset state */
        memcpy(D.R,RegInit,sizeof(D.R));
        D.EPhase  = 0;
        D.Clock   = ClockHz>>4;
        D.First   = first;
        D.Sync    = AY8910_ASYNC;
        D.Changed = 0x00;
        D.EPeriod = 0;
        D.ECount  = 0;
        D.Latch   = 0x00;

        /* Set sound types */
        SetSound(0+first,SND_MELODIC);
        SetSound(1+first,SND_MELODIC);
        SetSound(2+first,SND_MELODIC);
        SetSound(3+first,SND_NOISE);
        SetSound(4+first,SND_NOISE);
        SetSound(5+first,SND_NOISE);

        /* Silence all channels */
        for(J=0;J<AY8910_CHANNELS;J++)
        {
            D.Freq[J]=D.Volume[J]=0;
            Sound(J+first,0,0);
        }
#ifdef SDL        
        /* Open the audio device */
        SDL_AudioSpec *desired;
        int SndBufSize = 2048; // 1024 for 22050, 2048 for 44100
        int Rate = DEVFREQ;
        // printf("OpenSoundDevice\n");
        sound_mutex=SDL_CreateMutex();

        /* Allocate a desired SDL_AudioSpec */
        desired=(SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));

        /* set audio parameters */
        desired->freq=Rate;
        desired->format=AUDIO_S16LSB; /* 16-bit signed audio */
        desired->samples=SndBufSize;  /* size audio buffer */
        desired->channels=2;

        /* Our callback function */
        desired->callback=DSPCallBack;
        desired->userdata=this;
        // printf("userdata=%08x\n",  desired.userdata);

        /* Open the audio device */
        if(SDL_OpenAudio(desired, NULL)<0)
        {
            // printf("Audio Open failed\n");

            // SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &desired, NULL, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
            // SDL_PauseAudioDevice(dev, 0);
            return;
        }
        // printf("Audio Open successed\n");
        // SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &desired, NULL, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
        SDL_PauseAudio(0);
        // Rate=obtaineD.freq;
        // obtaineD.userdata=(void*)calloc(1, 2048*2);
        /* Adjust buffer size to the obtained buffer size */
        // SndBufSize=obtaineD.samples;

        /* Start playing */
        // unpauseAudio();
        // printf("Buff=%08x, SndBufSize=%08x\n", 2048*2, SndBufSize);
        /* Free memory */
        // free(obtained);
        free(desired);

        DevFreq = Rate;
        int i;
        SDL_mutexP(sound_mutex);
        LastBufTime = SDL_GetTicks();
        SndQ.front = SndQ.rear = 0;
        SDL_mutexV(sound_mutex);
#endif

        for (i = 0; i < 6; i++)
            Sound(i, 0, 0);
    }
    int SndEnqueue(int Chn, int Freq, int Vol)
    {
        int tfront;
#ifdef SDL
        SDL_mutexP(sound_mutex);
#endif        
        tfront = (SndQ.front + 1) % MAX_SNDQ;
        if (tfront == SndQ.rear)
        {
    //#define QUEUEFULL_PROCESS
    #ifdef QUEUEFULL_PROCESS // not working yet
            TSndQEntry *qentry;
            printf("Queue Full!!\n");
            qentry = SndDequeue();
            Sound(qentry->chn, qentry->freq, qentry->vol);
    #else
            printf("Queue Full!!\n");
    #endif
#ifdef SDL
            SDL_mutexV(sound_mutex);
#endif            
            return 0;
        }
        SndQ.qentry[SndQ.front].chn = Chn;
        SndQ.qentry[SndQ.front].freq = Freq;
        SndQ.qentry[SndQ.front].vol = Vol;
        SndQ.qentry[SndQ.front].time = SDL_GetTicks() - LastBufTime;
        SndQ.front = tfront;
#ifdef SDL        
        SDL_mutexV(sound_mutex);
#endif        
        return 1;
    }    
    TSndQEntry *SndDequeue(void)
    {
        int trear;

#ifdef SDL
        SDL_mutexP(sound_mutex);
        trear = SndQ.rear;
        if (SndQ.front == SndQ.rear)
        {
            SDL_mutexV(sound_mutex);
            return NULL; // queue empty
        }
        SndQ.rear = (SndQ.rear + 1) % MAX_SNDQ;
        SDL_mutexV(sound_mutex);
#endif
        return &(SndQ.qentry[trear]);
    }
    int inline SndPeekQueue() 
    {
        return ((SndQ.front == SndQ.rear) ? -1: SndQ.qentry[SndQ.rear].time);
    } 
    void Sound(int Chn,int Freq,int Volume)
    {
        int interval;

        interval = Freq? (DevFreq/Freq):0;
        if (interval != Interval[Chn])
        {
            Interval[Chn] = interval;
            Phase[Chn] = 0;
        }
        if (Interval[Chn] == 0)
            Vol[Chn] = 0;
        else
            Vol[Chn] = Volume; // do not exceed 32767
    }
    void callback(unsigned char *stream, int len)
    {
         int   J;
        static int R1 = 0,R2 = 0;
        int i;
        int vTime, qTime; // virtual Time, queue Time
        TSndQEntry *qentry = NULL;
        for(J=0;J<len;J+=4) // for the requested amount of buffer
        {
            vTime = (250*J)/DevFreq;
            qTime = SndPeekQueue();

            while ((qTime != -1) && (vTime - qTime) > 0) // Check Sound Queue
            {
                qentry = SndDequeue();

                Sound(qentry->chn, qentry->freq, qentry->vol);
                qTime = SndPeekQueue();
            }

            R1 = 0;
            for (i = 0; i < 3; i++) // Tone Generation
            {
                if (Interval[i] && Vol[i])
                {
                    if (Phase[i] < (Interval[i]/2))
                    {
                        R1 += Vol[i];
                        R1 = (R1 > 32767)? 32767: R1;
                    }
                    else if (Phase[i] >= (Interval[i]/2) && Phase[i] < Interval[i])
                    {
                        R1 -= Vol[i];
                        R1 = (R1 < -32768)? -32768: R1;
                    }
                    Phase[i] ++;
                    if (Phase[i] >= Interval[i])
                    {
                        Phase[i] = 0;
                        //R1 += Vol[i];
                    }
                }
            }

            for (i = 3; i < 6; i++) // Noise Generation
            {
                if (Interval[i] && Vol[i])
                {
                    if (Phase[i] == 0)
                        NoiseInterval[i] = Interval[i] + (((150 * rand()) / RAND_MAX) - 75);
                    if (Phase[i] < (NoiseInterval[i]/2))
                    {
                        R1 += Vol[i];
                        R1 = (R1 > 32767)? 32767: R1;
                    }
                    else if (Phase[i] >= (NoiseInterval[i]/2) && Phase[i] < NoiseInterval[i])
                    {
                        R1 -= Vol[i];
                        R1 = (R1 < -32768)? -32768: R1;
                    }
                    Phase[i] ++;
                    if (Phase[i] >= NoiseInterval[i])
                    {
                        Phase[i] = 0;
                        //R1 += Vol[i];
                    }
                }
            }

            R2 = R1;
            stream[J+0]=R2&0x00FF;
            stream[J+1]=R2>>8;
            stream[J+2]=R1&0x00FF;
            stream[J+3]=R1>>8;
        }
#ifdef SDL
        LastBufTime = SDL_GetTicks();
        while ((qentry = SndDequeue()))
            Sound(qentry->chn, qentry->freq, qentry->vol);        
#endif            
    }
    static void DSPCallBack(void* cb, unsigned char *stream, int len)
    {
        AY8910 *audio = (AY8910 *)cb;
        if (audio)
            audio->callback(stream, len);
    }
    void SetSound(int Channel,int NewType)
    {
    //	printf("SetSound: Chn=%d, NewType=%d\n", Channel, NewType);
    }
    uint8_t read_data()
    {
        return(D.R[D.Latch]);
    }    
    void write_data(uint8_t V)
    {
        uint8_t R = D.Latch;
         int J,I;

        /* Exit if no change */
        if((R>15)||((V==D.R[R])&&(R<8)&&(R>10))) return;
        switch(R)
        {
            case 1:
            case 3:
            case 5:
            V&=0x0F;
            /* Fall through */
            case 0:
            case 2:
            case 4:
            /* Write value */
            D.R[R]=V;
            /* Exit if the channel is silenced */
            if(D.R[7]&(1<<(R>>1))) return;
            /* Go to the first  in the pair */
            R&=0xFE;
            /* Compute frequency */
            J=((int)(D.R[R+1]&0x0F)<<8)+D.R[R];
            /* Compute channel number */
            R>>=1;
            /* Assign frequency */
            D.Freq[R]=D.Clock/(J? J:0x1000);
            /* Compute changed channels mask */
            D.Changed|=1<<R;
            break;

            case 6:
            /* Write value */
            D.R[6]=V&=0x1F;
            /* Exit if noise channels are silenced */
            if(!(~D.R[7]&0x38)) return;
            /* Compute and assign noise frequency */
            /* Shouldn't do <<2, but need to keep frequency down */
            J=D.Clock/((V&0x1F? (V&0x1F):0x20)<<2);
            if(!(D.R[7]&0x08)) D.Freq[3]=J;
            if(!(D.R[7]&0x10)) D.Freq[4]=J;
            if(!(D.R[7]&0x20)) D.Freq[5]=J;
            /* Compute changed channels mask */
            D.Changed|=0x38&~D.R[7];
            break;

            case 7:
            if (V & 0x80)
            {
                //printf("Print Strobe Req.\n");
            }
            /* Find changed channels */
            R=(V^D.R[7])&0x3F;
            D.Changed|=R;
            /* Write value */
            D.R[7]=V;
            /* Update frequencies */
            for(J=0;R&&(J<AY8910_CHANNELS);J++,R>>=1,V>>=1)
                if(R&1)
                {
                if(V&1) D.Freq[J]=0;
                else if(J<3)
                {
                    I=((int)(D.R[J*2+1]&0x0F)<<8)+D.R[J*2];
                    D.Freq[J]=D.Clock/(I? I:0x1000);
                }
                else
                {
                    /* Shouldn't do <<2, but need to keep frequency down */
                    I=D.R[6]&0x1F;
                    D.Freq[J]=D.Clock/((I? I:0x20)<<2);
                }
                }
            break;

            case 8: // amplitude control
            case 9:
            case 10:
            /* Write value */
            D.R[R]=V&=0x1F;
            /* Compute channel number */
            R-=8;
            /* Compute and assign new volume */
            J=Volumes[V&0x10? Envelopes[D.R[13]&0x0F][D.EPhase]:(V&0x0F)];
            D.Volume[R]=J;
            D.Volume[R+3]=(J+1)>>1;
            /* Compute changed channels mask */
            D.Changed|=(0x09<<R)&~D.R[7];
            break;

            case 11: // envelope period control
            case 12:
            /* Write value */
            D.R[R]=V;
            /* Compute envelope period (why not <<4?) */
            J=((int)D.R[12]<<8)+D.R[11];
            D.EPeriod=1000*(J? J:0x10000)/D.Clock;
            /* No channels changed */
            return;

            case 13: // envelope shape/cycle control (ionique. originally the same as 14,15)
            /* Write value */
            D.R[R]=V&=0x0F;
            /* Reset envelope */
            D.ECount = 0;
            D.EPhase = 0;
            for(J=0;J<AY8910_CHANNELS/2;++J)
                if(D.R[J+8]&0x10)
                {
                I = Volumes[Envelopes[V][0]];
                D.Volume[J]   = I;
                D.Volume[J+3] = (I+1)>>1;
                D.Changed    |= (0x09<<J)&~D.R[7];
                }
            break;

            //case 13:
            case 14:
            return;
            case 15:
            /* Write Printer value */
            //printf("PRT<-%c (%d)\n", V, V);
            D.R[R]=V;
            /* No channels changed */
            return;

            default:
            /* Wrong , do nothing */
            return;        
        }
        /* For asynchronous mode, make Sound() calls right away */
        if(!D.Sync&&D.Changed) sync(AY8910_FLUSH);        
    }    
    void sync( uint8_t Sync)
    {
         int J,I;

        /* Hit MIDI drums for noise channels, if requested */
        #define DRM_MIDI        0x100  /* MIDI drum (ORable)         */
        if(Sync&AY8910_DRUMS)
        {
            Sync&=~AY8910_DRUMS;
            J = (D.Freq[3]? D.Volume[3]:0)
            + (D.Freq[4]? D.Volume[4]:0)
            + (D.Freq[5]? D.Volume[5]:0);
            //if(J) Drum(DRM_MIDI|28,(J+2)/3);
        }

        if(Sync!=AY8910_FLUSH) D.Sync=Sync;

        for(J=0,I=D.Changed;I&&(J<AY8910_CHANNELS);J++,I>>=1)
            if(I&1) Sound(J+D.First,D.Freq[J],D.Volume[J]);
            //if(I&1) SndEnqueue(J+D.First,D.Freq[J],D.Volume[J]); // ionique
        D.Changed=0x00;
    }   

    void loop(int mS)
    {
         int J,I;
        /* Exit if no envelope running */
        if(!D.EPeriod) return;

        /* Count milliseconds */
        D.ECount += mS;
        if(D.ECount<D.EPeriod) return;

        /* Count steps */
        J = D.ECount/D.EPeriod;
        D.ECount -= J*D.EPeriod;

        /* Count phases */
        D.EPhase += J;
        if(D.EPhase>31)
            D.EPhase = (D.R[13]&0x09)==0x08? (D.EPhase&0x1F):31;

        /* Set envelope volumes for relevant channels */
        for(I=0;I<3;++I)
            if(D.R[I+8]&0x10)
            {
                J = Volumes[Envelopes[D.R[13]&0x0F][D.EPhase]];
                D.Volume[I]   = J;
                D.Volume[I+3] = (J+1)>>1;
                D.Changed    |= (0x09<<I)&~D.R[7];
            }

        /* For asynchronous mode, make Sound() calls right away */
        if(!D.Sync&&D.Changed) sync(AY8910_FLUSH);
    }
    void write_control(uint8_t V)
    {
        D.Latch=V&0x0F;
    }        
};

class PSG_Audio {
    // uint8_t regnum;
    enum casmode {CAS_STOP, CAS_PLAY, CAS_REC};
    typedef struct
    {
        int button;
        int rdVal;
        Uint32 lastTime;
        Uint32 cnt0, cnt1;

        int wrVal;
        Uint32 wrRisingT;	// rising time
    } CassetteTape;
    typedef struct
    {
        unsigned int length;
        uint8_t bufs[1024*1024];
        uint8_t poweron;
    } Printer;
    CassetteTape cas;
    Printer prt;
    AY8910 sound;
    int b = 0;
    // std::string s = "tap/*.tap";
    // Files files(s.c_str());
public:
    // FILE *wfp, *rfp;
    // TapeFiles files{"tap.zip"};
    PSG_Audio() {
        initialize();
    }
    void initialize() {
        sound.reset(0);
        sys.tape.motor = 1;
        sys.tape.pulse = 0;
        cas.button = CAS_PLAY;
        sys.turbo(0);
        // files = new Files("tap");
        // files->traverse((char *)"tap", 0);
        // files->find_file("tap", std::regex("*.tap"));
    }
	uint8_t read(uint8_t Port) {
    	uint8_t retval = 0x1f;
        // sys.tape.motor = 1;
        // static Uint32 clock = 0;
		if (Port & 0x01) // Data
		{
			if (sound.D.Latch == 14)
			{
				// 0x80 - cassette data input
				// 0x40 - motor status
				// 0x20 - print status
//				if (spcsys.prt.poweron)
//                {
//                    printf("Print Ready Check.\n");
//                    retval &= 0xcf;
//                }
//                else
//                {
//                    retval |= 0x20;
//                }
				if (cas.button == CAS_PLAY && sys.tape.motor)
				{
                    // printf("%d(%d)\n", sys.cycles, 0);
                    // clock = cycles;
                    retval = CasRead() ? retval | 0x80 : retval & 0x7f;
					// if (CasRead() == 1)
					// 	retval |= 0x80; // high
					// else
					// 	retval &= 0x7f; // low
					// printf("%d", retval & 0x80 ? 1 : 0);
					// printf("c:%d\n", cycles);
				}
				if (sys.tape.motor)
					retval &= (~(0x40)); // 0 indicates Motor On
				else
					retval |= 0x40;
                // printf("%d", (retval & 0x80)>0);
			}
			else 
			{
				int data = sound.read_data();
				//printf("r(%d,%d)\n", spcsys.psgRegNum, data);
				return data;
			}
		} else if (Port & 0x02)
		{
            retval = (ReadVal() == 1 ? retval | 0x80 : retval & 0x7f);
		} else if (Port & 0x03)
        {
            return (sys.tape.rfp ? 1 : 0);
		} else if (Port & 0x04)
        {
            uint8_t retval = 0;
            //int pos = ftell(spconf.rfp);
            for(int i = 0; i < 8; i++) 
            {
                if (ReadVal())
                    retval += 1 << (7 - i);
            }
            //fseek(spconf.rfp, pos, 0);
            ReadVal();
            //printf("%02x,", retval);
            return retval;
        }
		return retval;  
    }
    void write(uint8_t Port, uint8_t Value) {
		if (Port & 0x01) // Data
		{
		    if (sound.D.Latch == 15) // Line Printer
            {
                if (Value != 0)
                {
                    prt.bufs[prt.length++] = Value;
//                    printf("PRT <- %c (%d)\n", Value, Value);
//                    printf("%s(%d)\n", spcsys.prt.bufs, spcsys.prt.length);
                }
            }
			//printf("reg:%d,%d\n", spcsys.psgRegNum, Value);
			sound.write_data(Value);
		}
		else // Reg Num
		{
			// regnum = Value;
            // if (Value == 14)
            // {
            //     printf("reg%d\n", Value);
            // }
			sound.write_control(Value);
		}        
    }
    #define STONE (180)
    #define LTONE (STONE*2)
    int CasRead()
    {
        int ttime;
        int ret = 0;
        int t;
 
        t = (sys.cycles - cas.lastTime);
        // printf("cycle:%d,", t);
        // if (t > LTONE * 2) {
        //     cas.lastTime = cycles;
        // }
        // else 
        {
            ttime = (cas.rdVal ? LTONE : STONE);
            ret = t > ttime/2 ? 0:1;
            if (t > ttime)
            {
                cas.rdVal = ReadVal();
                // printf("(%d,%d),",cycles, cas.rdVal);
                // printf("(%d/%d)", cas.rdVal, t);
                cas.lastTime = sys.cycles;
            }
        }
        // printf("%d,%d[%d]\n", ret,t,cas.rdVal);
        return ret; // low for other cases
    }
    void CasWrite1(int val)
    {
        sys.tape.putc('0'+val);
        printf("%d", val);
        cas.wrVal = val;
        return;
    }
// #define DEBUG_MODE
    void CasWrite(int val)
    {
        // Uint32 curTime;
        int t;

        t = (sys.cycles - cas.lastTime) >> 5;
        if (t > 100)
            cas.cnt0 = cas.cnt1 = 0;
        cas.lastTime = sys.cycles;
        if (cas.wrVal == 1)
        {
            if (val == 0)
            {
                if (t > STONE/2) 
                {
                    printf("1");
                    cas.cnt0 = 0;
                    sys.tape.putc('1');
                } else {
                    if (cas.cnt0++ < 100)
                    {
                        printf("0");
                        sys.tape.putc('0');
                    }
                }
            }
        }
        cas.wrVal = val;
    }
    int ReadVal(void)
    {
        int c;
        // static int b = 0;
        static int EOF_flag = 0;
        if (b > 50) 
        {
            //printf("\b b=%d\n", b);
            while((c = sys.tape.getc()) == '0');
            b = 0;
        }
        else
            c = sys.tape.getc();
        if (c == EOF)
        {
            if (!EOF_flag)
                EOF_flag = 1;
            c = -1;
        }
        else
        {
            EOF_flag = 0;
            c -= '0';
        }
        b = (c == 0 ? b+1 : 0);
        return c;
    }    
    void loop() {
        sound.loop(1);
    }
    void smode(uint8_t value) {
        if (cas.button != CAS_STOP)
        {

            if (value & 0x02) // Motor
            {
                sys.tape.pulse = !sys.tape.pulse;
            }
            else
            {
                if (sys.tape.pulse)
                {
                    sys.tape.pulse = 0;
//                     if (sys.tape.motor)
//                     {
//                         sys.tape.motor = 0;
// #define DEBUG_MODE
// #ifdef DEBUG_MODE
//                         printf("Motor Off\n");
// #endif
//                         sys.turbo(0);
//                     }
//                     else
                    {
                        sys.tape.motor = 1;
#ifdef DEBUG_MODE
                        printf("Motor On\n");
#endif
                        sys.turbo(10);
                        cas.lastTime = sys.cycles;
                        // ResetCassette(&spcsys.cas);
                    }
                }
                // printf("Cycles:%d\n", sys.cycles);
            }
        }

        if (cas.button == CAS_REC && sys.tape.motor)
        {
            CasWrite1(value & 0x01);
        }        
        // printf("smode:%02x\n", value);

    }
};