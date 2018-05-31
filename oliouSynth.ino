#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <EEPROMAnything.h>
#include <ClickEncoder.h>
#include <TimerOne.h>

struct VoiceItems{
  int note = -1;
  AudioSynthWaveform *waveform1;
  AudioEffectEnvelope *envelope1;
} ;

///////////////////OBJECTS/////////////////////////
class VoiceHandler {
  int note;
//    AudioSynthWaveform       waveform1;  
   int maxVoices = 10;
   VoiceItems *voices[10];
 // struct VoiceItems *audioObjects;
    
  public:
    VoiceHandler(){}

    void addVoice(struct VoiceItems *voiceItems, int idx){
      voices[idx]=voiceItems;
    }
    
    void setup() {
      for (int idx = 0; idx < 2; idx++) {
        voices[idx]->waveform1->pulseWidth(0.5);
        voices[idx]->waveform1->begin(0.4, 220, WAVEFORM_PULSE);
      
        voices[idx]->envelope1->attack(50);
        voices[idx]->envelope1->decay(50);
        voices[idx]->envelope1->release(250);
      }
      //voices[0]->envelope1->noteOn();
  
    }

    void noteOn(int noteVal, int veloVal ) {
      note = noteVal;
      Serial.println("Voice.noteOn()");
      int freq = 440.0 * powf(2.0, (float)(noteVal - 69) * 0.08333333);

      for (int idx = 0; idx < maxVoices; idx++) {
        Serial.println("Voice "+(String)idx+":");
//        Serial.println((String) voices[idx]->note);
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
// GUItool: begin automatically generated code
// GUItool: begin automatically generated code
AudioSynthWaveform       voice1waveform1; //xy=379.14284896850586,437.428560256958
AudioSynthWaveform       voice0waveform1; //xy=552.0000171661377,301.71429443359375
AudioEffectEnvelope      voice1envelope1; //xy=729.2857131958008,474.428560256958
AudioEffectEnvelope      voice0envelope1; //xy=735.0000171661377,298.71429443359375
AudioMixer4              mixer1;         //xy=928.0000171661377,441.71429443359375
AudioOutputI2S           i2s1;           //xy=1202.5714359283447,325.8571581840515
AudioConnection          patchCord1(voice1waveform1, voice1envelope1);
AudioConnection          patchCord2(voice0waveform1, voice0envelope1);
AudioConnection          patchCord3(voice1envelope1, 0, mixer1, 1);
AudioConnection          patchCord4(voice0envelope1, 0, mixer1, 0);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);

//// GUItool: end automatically generated code
//
//AudioOutputI2S           i2s1;           //xy=565,241
//
//AudioSynthWaveform       voice0waveform1;      //xy=188,240
//AudioEffectEnvelope      voice0envelope1;      //xy=371,237
//AudioConnection          patchCord1(voice0waveform1, voice0envelope1);
//AudioConnection          patchCord2(voice0envelope1, 0, i2s1, 0);
//AudioConnection          patchCord3(voice0envelope1, 0, i2s1, 1);
//
//AudioSynthWaveform       voice1waveform1;      //xy=188,240
//AudioEffectEnvelope      voice1envelope1;      //xy=371,237
//AudioConnection          patchCord4(voice1waveform1, voice1envelope1);
//AudioConnection          patchCord5(voice1envelope1, 0, i2s1, 0);
//AudioConnection          patchCord6(voice1envelope1, 0, i2s1, 1);

AudioControlSGTL5000     audioShield;     //xy=586,175

// GUItool: end automatically generated code

typedef struct{
  String id;
  int initVal;
  int val;
  int minVal;
  int maxVal;
} VoiceParameter;


class VoiceParameters{
  public:
    VoiceParameter osc1_type = {"osc1_type",0,0,0,127};
};

void *voiceList[]={  };
VoiceItems voiceItems0= {-1, &voice0waveform1, &voice0envelope1};
VoiceItems voiceItems1= {-1, &voice1waveform1, &voice1envelope1};

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

VoiceParameters voiceParams;

/////////END: VOICE PROPERTIES /////////


VoiceHandler voiceHandler;

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

void handleEncoderEvent( ){
    for (int encIdx = 0; encIdx < maxEncNum; encIdx++) {
      
      //JOG EVENT
      encCurrValList[encIdx] += encoderList[encIdx]->getValue();
      int currVal = encCurrValList[encIdx]; 
      if (currVal != encPrevValList[encIdx]) {
          encPrevValList[encIdx] = currVal;
          Serial.print("Encoder "+(String)encIdx+" Value: ");
          Serial.println(currVal); 
//          Serial.print("filterFreq: ");
//          //filterFreq = log10(1+((float) last)/32);
//          filterFreq = pow((float) last, 1.5);
//          Serial.print((float)filterFreq); 
//          Serial.print(" ");
          
//          voice1filter.frequency(filterFreq);
        } 

        //CLICK EVENT
//        encoderList[encIdx]->setDoubleClickEnabled(true);
//        ClickEncoder::Button b = encoderList[encIdx]->getButton();
//        
//        if (b != ClickEncoder::Open) {
//        Serial.print("Button "+(String)encIdx+": ");
//        #define VERBOSECASE(label) case label: Serial.println(#label); break;
//        switch (b) {
//          VERBOSECASE(ClickEncoder::Pressed);
//          VERBOSECASE(ClickEncoder::Held)
//          VERBOSECASE(ClickEncoder::Released)
//          VERBOSECASE(ClickEncoder::Clicked)
//          case ClickEncoder::DoubleClicked:
//              Serial.println("ClickEncoder::DoubleClicked"); 
//            break;
//        }
//      }
    }
}

unsigned long last_time = millis();

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    while (!Serial);
    usbMIDI.setHandleNoteOn(handleNoteOn);
    usbMIDI.setHandleNoteOff(handleNoteOff);

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

     //voiceParams.osc1_type.id = "bimbol18";
     
     //memcpy(patchLibrary[0], defaultPatch, sizeof(defaultPatch));
//    memcpy(patchLibrary[0], defaultPatch, sizeof(defaultPatch));
//
  
//    memcpy(patchLibrary[1], defaultPatch, sizeof(defaultPatch));

//      paramIdxs indexes;
//     EEPROM_writeAnything(0, defaultPatch);
//      VoiceParameter testParam2;
    // Load Patch Library in memory 
//      Serial.println(defaultPatch[indexes.osc1_type][0]);

//      EEPROM_readAnything(0, defaultPatch);

//      Serial.println((String) defaultPatch[indexes.osc1_type][0]);


    // Initialise Encoders
    encoderList[0] = new ClickEncoder(1,2,3);
    encoderList[1] = new ClickEncoder(4  ,5,6);
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











