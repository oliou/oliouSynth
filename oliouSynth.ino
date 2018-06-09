#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <EEPROMAnything.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
#include <ILI9341_t3.h>
#include <gui-lib.h>

#define TFT_DC      20
#define TFT_CS      21
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI     7
#define TFT_SCLK    14
#define TFT_MISO    12
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
RotaryGauge gauge0 = RotaryGauge(&tft); 
RotaryGauge gauge1 = RotaryGauge(&tft); 
RotaryGauge gauge2 = RotaryGauge(&tft); 
RotaryGauge gauge3 = RotaryGauge(&tft); 
RotaryGauge gauge4 = RotaryGauge(&tft); 
RotaryGauge gauge5 = RotaryGauge(&tft); 
RotaryGauge gauge6 = RotaryGauge(&tft); 
RotaryGauge gauge7 = RotaryGauge(&tft); 
RotaryGauge *gauges[8]={
  &gauge0,
  &gauge1,
  &gauge2,
  &gauge3,
  &gauge4,
  &gauge5,
  &gauge6,
  &gauge7
};

uiSwitch switch0 = uiSwitch(&tft); 
uiSwitch switch1 = uiSwitch(&tft); 
uiSwitch switch2 = uiSwitch(&tft); 
uiSwitch switch3 = uiSwitch(&tft); 
uiSwitch switch4 = uiSwitch(&tft); 
uiSwitch switch5 = uiSwitch(&tft); 
uiSwitch switch6 = uiSwitch(&tft); 
uiSwitch switch7 = uiSwitch(&tft); 
uiSwitch *uiSwitches[8]={
  &switch0,
  &switch1,
  &switch2,
  &switch3,
  &switch4,
  &switch5,
  &switch6,
  &switch7
};

struct VoiceItems{
  int note = -1;
  AudioSynthWaveform *waveform1;
  AudioEffectEnvelope *envelope1;
  AudioFilterStateVariable *filter1;
  AudioMixer4 *mixer2; 
} ;

///////////////////OBJECTS/////////////////////////
typedef struct{
  String label;
  String id;
  int defaultVal;
  int val;
  int minVal;
  int maxVal;
  int type; //jog=0 ; click=1
  int mode;
  int encoder; 
  int ccnum;
  String valueLabel[10];
} VoiceParameter;

class VoiceParameters{
  public:
    int maxIdx=3;
    VoiceParameter p[100];
//    VoiceParameter osc1_type = {"osc1_type","osc1_type",0,0,0,127,0};
//    VoiceParameter filt1_freq = {"Freq","filt1_freq",0,0,0,127,0};

    VoiceParameters(){
      p[0] = {"osc1_type","osc1_type",0,0,0,127,0,1,0,45};
      p[1] = {"Freq","filt1_freq",    0,0,0,127,0,0,0,47};
      p[2] = {"Type","filt1_type",    0,0,0,  2,1,0,0,49,{"Low","Band","High"}}; 
//      p[3] = {"Freq","fildfhdfht1_freq",0,0,0,127,0};
//      p[4] = {"Freq","fildfhdft1_freq",0,0,0,127,0};
//      p[5] = {"Freq","filsdgsht1_freq",0,0,0,127,0};
//      p[6] = {"Freq","filt1dfhdf_freq",0,0,0,127,0};
//      p[7] = {"Freq","fildfhfdht1_freq",0,0,0,127,0};
//      p[8] = {"Freq","filt1_freq",0,0,0,127,0};
//      p[9] = {"Freq","filtdfhdf1_freq",0,0,0,127,0};
//      p[10] = {"Freq","filtdfhfdh1_freq",0,0,0,127,0};

    }

    VoiceParameter * getById(String id){
      for(int idx=0;idx<maxIdx;idx++){
        if( p[idx].id == id){
          return &p[idx];
        }
      }
    }

    VoiceParameter * getByModeEncType(int mode, int encoder, int type){
      for(int idx=0;idx<maxIdx;idx++){
        if( p[idx].mode == mode){
          if( p[idx].encoder == encoder){
            if( p[idx].type == type){
              return &p[idx];
            }
          }
        }
      }
      return NULL;
    }
    
//    void addParam(VoiceParameter vp,int idx){
//      p[idx]=vp;
//    }
};

class VoiceHandler {
  int note;
//    AudioSynthWaveform       waveform1;  
   int maxVoices = 10;
   VoiceItems *voices[10];
   VoiceParameters *params;
 // struct VoiceItems *audioObjects;
    
  public:
    VoiceHandler(VoiceParameters *voiceParams):params(voiceParams){}

    void addVoice(struct VoiceItems *voiceItems, int idx){
      voices[idx]=voiceItems;
    }
    
    void setup() {
        voices[0]->waveform1->pulseWidth(0.7);
        voices[0]->waveform1->begin(0.4, 220, WAVEFORM_PULSE);
      
        voices[0]->envelope1->attack(75);
        voices[0]->envelope1->decay(50);
        voices[0]->envelope1->release(250);
        
      
      //voices[0]->envelope1->noteOn();
  
    }

    void update(){
        float freq = pow((float) params->getById("filt1_freq")->val, 2.05);
        //Serial.println("freq: "+(String) freq);
        voices[0]->filter1->frequency(freq);
        voices[0]->filter1->resonance(2);

        int filterType = params->getById("filt1_type")->val;         
        voices[0]->mixer2->gain(0, (filterType==0)? 1:0);
        voices[0]->mixer2->gain(1, (filterType==1)? 1:0);
        voices[0]->mixer2->gain(2, (filterType==2)? 1:0);        
    }

    void noteOn(int noteVal, int veloVal ) {
      note = noteVal;
      Serial.println("Voice.noteOn()");
      int freq = 440.0 * powf(2.0, (float)(noteVal - 69) * 0.08333333);

      for (int idx = 0; idx < maxVoices; idx++) {
        Serial.println("Voice "+(String)idx+":");
        Serial.println((String) voices[idx]->note);
        if(voices[idx]->note == -1 && !isNoteOn(noteVal)){
          voices[idx]->note = noteVal;
          voices[idx]->waveform1->frequency(freq);
          voices[idx]->envelope1->noteOn();
          break;
        }
        // Replace oldest note with newest note
        if(!anyVoicesLeft()){
          
        }
        Serial.println((String) voices[idx]->note);
        //break;
      }      
    }

    bool isNoteOn(int noteVal){
      for (int idx = 0; idx < maxVoices; idx++) {
         if(voices[idx]->note == noteVal) return true;
      }
      return false;
    }

    bool anyVoicesLeft(){
      for (int idx = 0; idx < maxVoices; idx++) {
         if(voices[idx]->note == -1) return true;
      }
      return false;
    }

    void noteOff(int noteVal) {
      for (int idx = 0; idx < maxVoices; idx++) {
        Serial.println("Voice "+(String)idx+":");
//        Serial.println((String) voices[idx]->note);
        if(voices[idx]->note == noteVal){
          voices[idx]->note = -1;
//          voices[idx]->waveform1->frequency(freq);
          voices[idx]->envelope1->noteOff();
        }
        Serial.println((String) voices[idx]->note);
        //break;
      }    
    }

    int getCurrNote(){
      return note;
    }

    bool isBusy(){
      if(note != -1){
        return true;
      }
      return false;
      
    }
};

/////////ENCODER VARIABLES/////////
const int maxEncNum = 2;
ClickEncoder *encoderList[maxEncNum]={};
int encCurrValList[8]= {0,0,0,0,0,0,0,0}; 
int encPrevValList[8]= {0,0,0,0,0,0,0,0}; 

int16_t last, value;
int sensorPin = A0;
int sensorValue = 0;

// GUItool: begin automatically generated code
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       voice1waveform1; //xy=387.7143020629883,381.7142581939697
AudioSynthWaveform       voice0waveform1; //xy=552.0000171661377,301.71429443359375
AudioEffectEnvelope      voice1envelope1; //xy=701.2857055664062,442.4285583496094
AudioEffectEnvelope      voice0envelope1; //xy=735.0000171661377,298.71429443359375
AudioMixer4              mixer1;         //xy=902,394.71429443359375
AudioFilterStateVariable filter1;        //xy=1048,400
AudioMixer4              mixer2; //xy=1159,329
AudioOutputI2S           i2s1;           //xy=1276.5714111328125,329.8571472167969
AudioConnection          patchCord1(voice1waveform1, voice1envelope1);
AudioConnection          patchCord2(voice0waveform1, voice0envelope1);
AudioConnection          patchCord3(voice1envelope1, 0, mixer1, 1);
AudioConnection          patchCord4(voice0envelope1, 0, mixer1, 0);
AudioConnection          patchCord5(mixer1, 0, filter1, 0);
AudioConnection          patchCord6(filter1, 0, mixer2, 0);
AudioConnection          patchCord7(filter1, 1, mixer2, 1);
AudioConnection          patchCord8(filter1, 2, mixer2, 2);
AudioConnection          patchCord9(mixer2, 0, i2s1, 0);
AudioConnection          patchCord10(mixer2, 0, i2s1, 1);
// GUItool: end automatically generated code

AudioControlSGTL5000     audioShield;     //xy=586,175

// GUItool: end automatically generated code




//void *voiceList[]={  };
VoiceParameters voiceParams;
//VoiceParameter osc1_type = {"osc1_type","osc1_type",0,0,0,127,0}; 
//VoiceParameter filt1_freq = {"Freq","filt1_freq",0,0,0,127,0};

VoiceItems voiceItems0= {-1, &voice0waveform1, &voice0envelope1, &filter1, &mixer2};
VoiceItems voiceItems1= {-1, &voice1waveform1, &voice1envelope1, &filter1, &mixer2};

//VoiceItems voiceItems;


/////////START: PARAM PROPERTIES /////////
const int currVal = 0;
const int minVal = 1;
const int maxVal = 2;

/////////STOP: PARAM PROPERTIES /////////

/////////START: VOICE PROPERTIES /////////

typedef struct{
  int osc1_type = 0;     
  int osc1_wform = 1;     
  int osc1_amp = 2;     
  int osc1_offset = 3;     
  int osc1_phase = 4;     
  int osc1_pw = 5;     
  int osc1_noise = 6;     
  
  int osc2_type = 10;     
  int osc2_wform = 11;     
  int osc2_amp = 12;     
  int osc2_offset = 13;     
  int osc2_phase = 14;     
  int osc2_pw = 15;     
  int osc2_noise = 16;      
  
  int flt_type = 20;     
  int flt_reso = 21;     
  int flt_vel = 22;         
  int flt_env_mod = 23;     
  int flt_env_a = 24;     
  int flt_env_d = 25;     
  int flt_env_s = 26;     
  int flt_env_r = 27;     
     
  int amp_vel = 30;         
  int amp_env_mod = 31;     
  int amp_env_a = 32;     
  int amp_env_d = 33;     
  int amp_env_s = 34;     
  int amp_env_r = 35;    
  
  int lfo_dest = 40;     
  int lfo_freq = 41;    
  int lfo_wform = 42;      
  int lfo_amp = 43;     
  int lfo_offset = 44;     
  int lfo_phase = 45;     
  int lfo_pw = 46;     
  int lfo_keyon = 47;   
} VoiceParamIdxs;

int defaultPatch[50][3]={};
int defaultPatch2[50][3]={};
int patchLibrary[64][50][3]={};
int currentPatchIdx = 0;

int uiMode=0;

/////////END: VOICE PROPERTIES /////////

VoiceHandler voiceHandler(&voiceParams);

void timerIsr() { 
  for (int encIdx = 0; encIdx < maxEncNum; encIdx++) {
    encoderList[encIdx]->setAccelerationEnabled(false);     
    encoderList[encIdx]->service();     
  }
}

void handleNoteOn(byte inChannel, byte inNumber, byte inVelocity)
{
    Serial.print("NoteOn  ");
    Serial.print(inNumber);
    Serial.print("\tvelocity: ");
    Serial.println(inVelocity);
    voiceHandler.noteOn( inNumber, inVelocity);
   

}
void handleNoteOff(byte inChannel, byte inNumber, byte inVelocity)
{
    Serial.print("NoteOff ");
    Serial.print(inNumber);
    Serial.print("\tvelocity: ");
    Serial.println(inVelocity);
    voiceHandler.noteOff( inNumber);
}

void initGUI(){
    gauge0.drawFrame(40,40);    
    gauge1.drawFrame(120,40);
    gauge2.drawFrame(200,40);
    gauge3.drawFrame(280,40);
    gauge4.drawFrame(40,160);
    gauge5.drawFrame(120,160); 
    gauge6.drawFrame(200,160);
    gauge7.drawFrame(280,160);

    switch0.drawFrame(40,90);
    switch1.drawFrame(120,90);
//    switch2.setPosition(200);
//    switch3.setPosition(280);
//    switch4.setPosition(40);
//    switch5.setPosition(120);
//    switch6.setPosition(200);
//    switch7.setPosition(280);

    
    refreshGUI();
}

void refreshGUI(){
    for (int encIdx = 0; encIdx < maxEncNum; encIdx++) {
      VoiceParameter *param = voiceParams.getByModeEncType(uiMode, encIdx, 0);  
      if(param != NULL){
        gauges[encIdx]->drawLabel(param->label);  
        gauges[encIdx]->drawValue(param->val, 
                                  param->minVal, 
                                  param->maxVal);
      }
      param = voiceParams.getByModeEncType(uiMode, encIdx, 1);  
      if(param != NULL){
        uiSwitches[encIdx]->draw(param->label, param->valueLabel[param->val], false, true, true);
      }
    }  
}

void handleEncoderEvent( ){
    for (int encIdx = 0; encIdx < maxEncNum; encIdx++) {
      
      //JOG EVENT
      encCurrValList[encIdx] += encoderList[encIdx]->getValue();
      int currVal = encCurrValList[encIdx]; 
      int prevVal = encPrevValList[encIdx];

      if (currVal != prevVal) {
          
          Serial.print("Encoder "+(String)encIdx+" Value: ");
          Serial.println(currVal); 
          VoiceParameter *param = voiceParams.getByModeEncType(uiMode, encIdx, 0);
          
          if(param->type == 0){
            if(currVal > prevVal && param->val < param->maxVal){
              param->val++;
            }
            if(currVal < prevVal && param->val > param->minVal){
              param->val--;
            }
            Serial.println((String) param->id);
            Serial.println((String) param->val);
            int val;
            int minVal;
            int maxVal;
            voiceHandler.update();
            gauges[encIdx]->drawValue(param->val, 
                                      param->minVal, 
                                      param->maxVal);
          }
//          gauge1.drawValue(currVal,0,127);

//          Serial.print("filterFreq: ");
//          //filterFreq = log10(1+((float) last)/32);
//          filterFreq = pow((float) last, 1.5);
//          Serial.print((float)filterFreq); 
//          Serial.print(" ");
          
//          voice1filter.frequency(filterFreq);
          encPrevValList[encIdx] = currVal;
        } else {
          //CLICK EVENT
          encoderList[encIdx]->setDoubleClickEnabled(true);
          ClickEncoder::Button b = encoderList[encIdx]->getButton();
          
          if (b != ClickEncoder::Open) {
            Serial.print("Button "+(String)encIdx+": ");
            #define VERBOSECASE(label) case label: Serial.println(#label); break;
            switch (b) {
              VERBOSECASE(ClickEncoder::Pressed);
              VERBOSECASE(ClickEncoder::Held)
              VERBOSECASE(ClickEncoder::Released)
              VERBOSECASE(ClickEncoder::Clicked)
              case ClickEncoder::DoubleClicked:
                  Serial.println("ClickEncoder::DoubleClicked"); 
                break;
            }
            VoiceParameter *param = voiceParams.getByModeEncType(uiMode, encIdx, 1);
            if(param != NULL){
              if( param->val <= param->maxVal){
                param->val++;
              }
              if(param->val > param->maxVal){
                param->val = param->minVal;
              }
              Serial.println((String) param->id);
              Serial.println((String) param->val);
    //          Serial.println((String) param->valLabels[0]);
              voiceHandler.update();
              uiSwitches[encIdx]->draw(param->label, param->valueLabel[param->val], false, true, true);
            }
          }
        }
    }
}


unsigned long last_time = millis();

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    while (!Serial);
    usbMIDI.setHandleNoteOn(handleNoteOn);
    usbMIDI.setHandleNoteOff(handleNoteOff);

    tft.begin();
    tft.setRotation(3); 
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(10 , 110);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(5);
    tft.println("OliouSynth");
    delay(3000);
    tft.fillScreen(ILI9341_BLACK);
    
      // Set up
    AudioMemory(8);
    audioShield.enable();
    audioShield.volume(0.64);

    voiceHandler.addVoice(&voiceItems0,0);  
    voiceHandler.addVoice(&voiceItems1,1);  
    voiceHandler.setup();
    
    //voiceItems.waveform1->pulseWidth(0.5);
  
//    waveform1.pulseWidth(0.5);
//    waveform1.begin(0.4, 220, WAVEFORM_PULSE);
//  
//    envelope1.attack(50);
//    envelope1.decay(50);
//    envelope1.release(250);
//    envelope1.noteOn();
//    int buttonState = 0;         // current state of the button
//    int lastButtonState = 0;     // previous state of the button

    //voice1.setup();
    // Initialize patches


//    defaultPatch[osc1_type][currVal]=18;
//    defaultPatch[osc1_type][minVal]=0;
//    defaultPatch[osc1_type][maxVal]=127;

//     voiceParams.p[10].val = 121;
     
     //memcpy(patchLibrary[0], defaultPatch, sizeof(defaultPatch));
//    memcpy(patchLibrary[0], defaultPatch, sizeof(defaultPatch));
//
  
//    memcpy(patchLibrary[1], voiceParams, sizeof(voiceParams));

//      paramIdxs indexes;
//     EEPROM_writeAnything(0, voiceParams);
//     VoiceParameters testParam2;
    // Load Patch Library in memory 
//      Serial.println(defaultPatch[indexes.osc1_type][0]);
//    Serial.println((String) testParam2.filt1_freq.val);

//      EEPROM_readAnything(0, voiceParams);
      voiceHandler.update();
//      Serial.println((String) defaultPatch[indexes.osc1_type][0]);
//     VoiceParameter *tp = voiceParams.getById("filt1_freq");
//    Serial.println((String) voiceParams.getByModeEncType(0,1,1)->id);
        Serial.println((String) voiceParams.p[2].label);

          Serial.println("RRR");

//    VoiceParameter *param = voiceParams.getByModeEncType(0, 0, 1);
//    Serial.println((String) param->label);

    // Initialise Encoders
    encoderList[0] = new ClickEncoder(1,2,3);
    encoderList[1] = new ClickEncoder(4,5,6);
//    encoderList[2] = new ClickEncoder(7,8,10);
//    encoderList[3] = new ClickEncoder(14,15,16);
//    encoderList[4] = new ClickEncoder(21,22,23);
//    encoderList[5] = new ClickEncoder(33,34,35);
//    encoderList[6] = new ClickEncoder(36,37,38);
//    encoderList[7] = new ClickEncoder(27,28,29);
    Timer1.initialize(1000);
    Timer1.attachInterrupt(timerIsr); 
    last = -1;
  
     // Initialize processor and memory measurements
    AudioProcessorUsageMaxReset();
    AudioMemoryUsageMaxReset();

    initGUI();

    Serial.println("##### OliouSynth #####");
}

void loop() {
    
    // put your main code here, to run repeatedly:  
    while (usbMIDI.read())
    {
      // ignoring incoming messages, so don't do anything here.
         //Serial.print(patchLibrary[1][osc1_type]);

    }


//
//     value += encoder1->getValue();
//
//  if (value != last) {
////      last = value;
////      Serial.print("Encoder Value: ");
////      Serial.println(last/4); 
////      Serial.print("Encoder2: ");
////      //filterFreq = log10(1+((float) last)/32);
////      filterFreq = pow((float) last, 1.59);
////      Serial.print((float)filterFreq); 
////      Serial.print(" ");
////      
////      voice1filter.frequency(filterFreq);
//    }
  handleEncoderEvent();
  //sensorValue = analogRead(sensorPin);
  if(1) {
    if(millis() - last_time >= 3000) {
//      envelope1.noteOn();
//      voice1.noteOff();
////      Serial.println((String) voice1.getCurrNote());
////      Serial.println((String) voice1.isBusy());
//      voice1.noteOn(32,127);
//      Serial.println((String) voiceParams.osc1_type.id);

//      ClickEncoder::Button b = encoderList[0]->getButton();
//        Serial.print((String) b);
//      Serial.print("Enc = ");
////        encoderList[0] = encoder0;
////        ClickEncoder *tempEnc = encoderList[0];
//      //Serial.println(encoderList[0]->getValue()); 

//      EEPROM_writeAnything(0, voiceParams);
      
//      EEPROM_readAnything(0, voiceParams);
//      
      Serial.print("Proc = ");
      Serial.print(AudioProcessorUsage());
      Serial.print(" (");    
      Serial.print(AudioProcessorUsageMax());
      Serial.print("),  Mem = ");
      Serial.print(AudioMemoryUsage());
      Serial.print(" (");    
      Serial.print(AudioMemoryUsageMax());
      Serial.println(")");
      last_time = millis();
    }
  }    
}











