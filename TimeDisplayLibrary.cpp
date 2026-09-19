#include "TimeDisplayLibrary.h"

static const uint8_t DIGIT_SEGMENTS[10] = {
  0b00111111,0b00000110,0b01011011,0b01001111,0b01100110,
  0b01101101,0b01111101,0b00000111,0b01111111,0b01101111
};
static const uint8_t SEGMENT_TO_PIXEL[7] = {6,5,4,3,2,1,0};

TimeDisplayLibrary::TimeDisplayLibrary(Adafruit_NeoPixel &d)
: _time_display(d), _color(d.Color(255,0,0)), _brightness(255), _currentNumber(0),
  _currentLeadingZeros(false), _hasCurrentNumber(false), _effect(EFFECT_NONE),
  _intensity(NORMAL), _effectStartMs(0), _effectDurationMs(0), _nextFrameMs(0),
  _effectResult(0), _effectLeadingZeros(false), _effectIntervalMs(60),
  _flashColor(0), _flashCount(0), _flashTransitions(0), _flashOn(false) {
  for(uint8_t i=0;i<4;i++){_digitColors[i]=0;_digitColorEnabled[i]=false;_shuffleDigits[i]=0;_shuffleStopped[i]=false;}
}

uint32_t TimeDisplayLibrary::namedColor(NamedColor c) const {
  switch(c){
    case GREEN:return _time_display.Color(0,255,0); case BLUE:return _time_display.Color(0,0,255);
    case WHITE:return _time_display.Color(255,255,255); case PURPLE:return _time_display.Color(180,0,255);
    case CYAN:return _time_display.Color(0,255,255); case ORANGE:return _time_display.Color(255,80,0);
    case YELLOW:return _time_display.Color(255,255,0); case PINK:return _time_display.Color(255,20,120);
    case ELECTRIC_BLUE:return _time_display.Color(30,120,255); default:return _time_display.Color(255,0,0);
  }
}
uint32_t TimeDisplayLibrary::wheel(uint8_t p) const {
  p=255-p; if(p<85)return _time_display.Color(255-p*3,0,p*3);
  if(p<170){p-=85;return _time_display.Color(0,p*3,255-p*3);}
  p-=170;return _time_display.Color(p*3,255-p*3,0);
}
void TimeDisplayLibrary::setColor(uint8_t r,uint8_t g,uint8_t b){_color=_time_display.Color(r,g,b);}
void TimeDisplayLibrary::setColor(uint32_t c){_color=c;}
void TimeDisplayLibrary::setColor(NamedColor c){_color=namedColor(c);}
void TimeDisplayLibrary::setDigitColor(uint8_t p,uint8_t r,uint8_t g,uint8_t b){if(p<4){_digitColors[p]=_time_display.Color(r,g,b);_digitColorEnabled[p]=true;}}
void TimeDisplayLibrary::setDigitColor(uint8_t p,NamedColor c){if(p<4){_digitColors[p]=namedColor(c);_digitColorEnabled[p]=true;}}
void TimeDisplayLibrary::clearDigitColors(){for(uint8_t i=0;i<4;i++)_digitColorEnabled[i]=false;}
void TimeDisplayLibrary::setBrightness(uint8_t b){_brightness=b;_time_display.setBrightness(b);_time_display.show();}

void TimeDisplayLibrary::clearDigit(uint8_t p){if(p>=4)return;for(uint8_t i=0;i<7;i++)_time_display.setPixelColor(p*7+i,0);}
void TimeDisplayLibrary::blankDigit(uint8_t p){stopEffect();clearDigit(p);_time_display.show();}
void TimeDisplayLibrary::drawDigitWithColor(uint8_t p,uint8_t d,uint32_t c){
  if(p>=4||d>9)return; clearDigit(p); uint8_t m=DIGIT_SEGMENTS[d];
  for(uint8_t s=0;s<7;s++)if(m&(1U<<s))_time_display.setPixelColor(p*7+SEGMENT_TO_PIXEL[s],c);
}
void TimeDisplayLibrary::drawDigit(uint8_t p,uint8_t d){drawDigitWithColor(p,d,_digitColorEnabled[p]?_digitColors[p]:_color);}
bool TimeDisplayLibrary::showDigit(uint8_t p,uint8_t d){if(p>=4||d>9)return false;stopEffect();drawDigit(p,d);_time_display.show();return true;}
void TimeDisplayLibrary::drawNumberInternal(uint16_t n,bool zeros,bool remember){
  uint8_t d[4]={(uint8_t)(n/1000%10),(uint8_t)(n/100%10),(uint8_t)(n/10%10),(uint8_t)(n%10)};
  bool started=zeros; for(uint8_t p=0;p<4;p++){if(!started&&d[p]==0&&p<3)clearDigit(p);else{started=true;drawDigit(p,d[p]);}}
  _time_display.show(); if(remember){_currentNumber=n;_currentLeadingZeros=zeros;_hasCurrentNumber=true;}
}
bool TimeDisplayLibrary::showNumber(uint16_t n,bool z){if(n>9999)return false;stopEffect();drawNumberInternal(n,z,true);return true;}
void TimeDisplayLibrary::drawStoredNumber(){if(_hasCurrentNumber)drawNumberInternal(_currentNumber,_currentLeadingZeros,false);else{_time_display.clear();_time_display.show();}}
void TimeDisplayLibrary::clear(){stopEffect();_hasCurrentNumber=false;_time_display.clear();_time_display.show();}

void TimeDisplayLibrary::beginEffect(EffectType e,uint32_t d){stopEffect();_effect=e;_effectStartMs=millis();_effectDurationMs=d?d:1;_nextFrameMs=0;}
void TimeDisplayLibrary::startElectricBuzz(uint32_t d,Intensity i){_intensity=i;beginEffect(EFFECT_ELECTRIC_BUZZ,d);}
void TimeDisplayLibrary::startFlicker(uint32_t d,Intensity i){_intensity=i;beginEffect(EFFECT_FLICKER,d);}
void TimeDisplayLibrary::startGlitch(uint32_t d,Intensity i){_intensity=i;beginEffect(EFFECT_GLITCH,d);}
void TimeDisplayLibrary::startBrownout(uint32_t d){beginEffect(EFFECT_BROWNOUT,d);}
void TimeDisplayLibrary::startStrobe(uint32_t d,uint16_t ms){_effectIntervalMs=max((uint16_t)20,ms);beginEffect(EFFECT_STROBE,d);}
void TimeDisplayLibrary::startBreathe(uint32_t d,uint16_t cycle){_effectIntervalMs=max((uint16_t)200,cycle);beginEffect(EFFECT_BREATHE,d);}
void TimeDisplayLibrary::startPulse(uint32_t d,uint16_t cycle){_effectIntervalMs=max((uint16_t)100,cycle);beginEffect(EFFECT_PULSE,d);}
void TimeDisplayLibrary::startRainbow(uint32_t d,uint16_t f){_effectIntervalMs=max((uint16_t)15,f);beginEffect(EFFECT_RAINBOW,d);}
void TimeDisplayLibrary::startColorCycle(uint32_t d,uint16_t f){_effectIntervalMs=max((uint16_t)20,f);beginEffect(EFFECT_COLOR_CYCLE,d);}
void TimeDisplayLibrary::startShimmer(uint32_t d){beginEffect(EFFECT_SHIMMER,d);}
void TimeDisplayLibrary::startSegmentChase(uint32_t d,uint16_t f){_effectIntervalMs=max((uint16_t)20,f);beginEffect(EFFECT_SEGMENT_CHASE,d);}
void TimeDisplayLibrary::startFadeIn(uint32_t d){beginEffect(EFFECT_FADE_IN,d);}
void TimeDisplayLibrary::startFadeOut(uint32_t d){beginEffect(EFFECT_FADE_OUT,d);}
void TimeDisplayLibrary::startPowerDown(uint32_t d){beginEffect(EFFECT_POWER_DOWN,d);}

void TimeDisplayLibrary::shuffleTo(uint16_t n,uint32_t d,bool z){
  if(n>9999)n=9999;_effectResult=n;_effectLeadingZeros=z;for(uint8_t i=0;i<4;i++){_shuffleDigits[i]=random(10);_shuffleStopped[i]=false;}beginEffect(EFFECT_SHUFFLE,max((uint32_t)250,d));
}
void TimeDisplayLibrary::shuffleRandom(uint32_t d,bool z){shuffleTo((uint16_t)random(10000),d,z);}
void TimeDisplayLibrary::startSlotMachine(uint16_t n,uint32_t d,bool z){shuffleTo(n,d,z);}
void TimeDisplayLibrary::startOdometer(uint16_t n,uint32_t d,bool z){_effectResult=min((uint16_t)9999,n);_effectLeadingZeros=z;beginEffect(EFFECT_ODOMETER,d);}
void TimeDisplayLibrary::startCountTo(uint16_t n,uint32_t d,bool z){_effectResult=min((uint16_t)9999,n);_effectLeadingZeros=z;beginEffect(EFFECT_COUNT_TO,d);}
void TimeDisplayLibrary::startPowerUp(uint16_t n,uint32_t d,bool z){_effectResult=min((uint16_t)9999,n);_effectLeadingZeros=z;beginEffect(EFFECT_POWER_UP,d);}
void TimeDisplayLibrary::startRevealLeft(uint16_t n,uint32_t d,bool z){_effectResult=min((uint16_t)9999,n);_effectLeadingZeros=z;beginEffect(EFFECT_REVEAL_LEFT,d);}
void TimeDisplayLibrary::startRevealRight(uint16_t n,uint32_t d,bool z){_effectResult=min((uint16_t)9999,n);_effectLeadingZeros=z;beginEffect(EFFECT_REVEAL_RIGHT,d);}
void TimeDisplayLibrary::startFlash(uint8_t r,uint8_t g,uint8_t b,uint8_t flashes,uint16_t ms){
  if(!flashes)return;_flashColor=_time_display.Color(r,g,b);_flashCount=flashes;_flashTransitions=0;_flashOn=false;_effectIntervalMs=max((uint16_t)20,ms);beginEffect(EFFECT_FLASH,(uint32_t)flashes*2*_effectIntervalMs+1);
}
bool TimeDisplayLibrary::isEffectRunning()const{return _effect!=EFFECT_NONE;}
uint16_t TimeDisplayLibrary::getEffectResult()const{return _effectResult;}
void TimeDisplayLibrary::stopEffect(){if(_effect==EFFECT_NONE)return;_effect=EFFECT_NONE;_time_display.setBrightness(_brightness);drawStoredNumber();}
void TimeDisplayLibrary::finishEffect(){_effect=EFFECT_NONE;_time_display.setBrightness(_brightness);drawStoredNumber();}

void TimeDisplayLibrary::update(){
  if(_effect==EFFECT_NONE)return;uint32_t now=millis();
  if(now-_effectStartMs>=_effectDurationMs){
    if(_effect==EFFECT_SHUFFLE||_effect==EFFECT_ODOMETER||_effect==EFFECT_COUNT_TO||_effect==EFFECT_POWER_UP||_effect==EFFECT_REVEAL_LEFT||_effect==EFFECT_REVEAL_RIGHT){
      _currentNumber=_effectResult;_currentLeadingZeros=_effectLeadingZeros;_hasCurrentNumber=true;
    }
    finishEffect();return;
  }
  updateEffect(now);
}

void TimeDisplayLibrary::updateEffect(uint32_t now){
  uint32_t elapsed=now-_effectStartMs;
  if(_effect==EFFECT_ELECTRIC_BUZZ){
    if(now<_nextFrameMs)return;_nextFrameMs=now+random(_intensity==WILD?10:20,_intensity==SUBTLE?100:65);
    if(random(12)<_intensity){_time_display.clear();_time_display.show();return;}
    uint32_t save=_color;uint8_t c=random(3);_color=c==0?namedColor(ELECTRIC_BLUE):(c==1?_time_display.Color(160,220,255):namedColor(WHITE));drawStoredNumber();_color=save;return;
  }
  if(_effect==EFFECT_FLICKER||_effect==EFFECT_SHIMMER||_effect==EFFECT_BROWNOUT){
    if(now<_nextFrameMs)return;_nextFrameMs=now+random(25,110);
    uint8_t low=_effect==EFFECT_BROWNOUT?2:(_effect==EFFECT_SHIMMER?max(1,(int)_brightness-35):15);
    uint8_t hi=_effect==EFFECT_BROWNOUT?max(8,(int)_brightness/3):_brightness;
    _time_display.setBrightness(random(low,hi+1));drawStoredNumber();return;
  }
  if(_effect==EFFECT_GLITCH){
    if(now<_nextFrameMs)return;_nextFrameMs=now+random(25,100);drawStoredNumber();
    uint8_t hits=_intensity;while(hits--){uint16_t p=random(min((uint16_t)28,_time_display.numPixels()));_time_display.setPixelColor(p,random(2)?_color:0);}_time_display.show();return;
  }
  if(_effect==EFFECT_FLASH||_effect==EFFECT_STROBE){
    if(now<_nextFrameMs)return;_nextFrameMs=now+_effectIntervalMs;_flashOn=!_flashOn;
    if(_effect==EFFECT_FLASH&&++_flashTransitions>_flashCount*2){finishEffect();return;}
    if(_flashOn){uint32_t c=_effect==EFFECT_FLASH?_flashColor:namedColor(WHITE);for(uint16_t i=0;i<min((uint16_t)28,_time_display.numPixels());i++)_time_display.setPixelColor(i,c);_time_display.show();}else drawStoredNumber();return;
  }
  if(_effect==EFFECT_BREATHE||_effect==EFFECT_PULSE){
    uint32_t phase=elapsed%_effectIntervalMs;uint32_t half=_effectIntervalMs/2;uint8_t b=(phase<half)?(uint8_t)(phase*_brightness/half):(uint8_t)((_effectIntervalMs-phase)*_brightness/half);
    _time_display.setBrightness(b);drawStoredNumber();return;
  }
  if(_effect==EFFECT_FADE_IN||_effect==EFFECT_FADE_OUT||_effect==EFFECT_POWER_DOWN){
    uint8_t b=(uint8_t)(elapsed*(uint32_t)_brightness/_effectDurationMs);if(_effect!=EFFECT_FADE_IN)b=_brightness-b;_time_display.setBrightness(b);drawStoredNumber();return;
  }
  if(_effect==EFFECT_RAINBOW||_effect==EFFECT_COLOR_CYCLE){
    if(now<_nextFrameMs)return;_nextFrameMs=now+_effectIntervalMs;uint32_t save=_color;uint8_t base=(uint8_t)(elapsed/_effectIntervalMs);
    if(_effect==EFFECT_COLOR_CYCLE){_color=wheel(base*8);drawStoredNumber();}
    else if(_hasCurrentNumber){uint8_t d[4]={(uint8_t)(_currentNumber/1000%10),(uint8_t)(_currentNumber/100%10),(uint8_t)(_currentNumber/10%10),(uint8_t)(_currentNumber%10)};bool st=_currentLeadingZeros;for(uint8_t p=0;p<4;p++){if(!st&&d[p]==0&&p<3)clearDigit(p);else{st=true;drawDigitWithColor(p,d[p],wheel(base+p*55));}}_time_display.show();}
    _color=save;return;
  }
  if(_effect==EFFECT_SEGMENT_CHASE){
    if(now<_nextFrameMs)return;_nextFrameMs=now+_effectIntervalMs;_time_display.clear();uint8_t s=(elapsed/_effectIntervalMs)%7;for(uint8_t p=0;p<4;p++)_time_display.setPixelColor(p*7+SEGMENT_TO_PIXEL[s],_digitColorEnabled[p]?_digitColors[p]:_color);_time_display.show();return;
  }
  if(_effect==EFFECT_SHUFFLE){
    if(now<_nextFrameMs)return;uint32_t interval=30+elapsed*150UL/_effectDurationMs;_nextFrameMs=now+interval;
    uint8_t target[4]={(uint8_t)(_effectResult/1000%10),(uint8_t)(_effectResult/100%10),(uint8_t)(_effectResult/10%10),(uint8_t)(_effectResult%10)};
    uint32_t lockStart=_effectDurationMs*55UL/100UL,window=_effectDurationMs-lockStart,per=window/4;
    for(uint8_t i=0;i<4;i++){if(elapsed>=lockStart+per*(i+1)){_shuffleDigits[i]=target[i];_shuffleStopped[i]=true;}else if(!_shuffleStopped[i])_shuffleDigits[i]=random(10);drawDigit(i,_shuffleDigits[i]);}_time_display.show();return;
  }
  if(_effect==EFFECT_ODOMETER||_effect==EFFECT_COUNT_TO){
    uint16_t n=(uint16_t)((uint32_t)_effectResult*elapsed/_effectDurationMs);drawNumberInternal(n,_effectLeadingZeros,false);return;
  }
  if(_effect==EFFECT_POWER_UP){
    uint8_t stage=(uint8_t)(elapsed*5UL/_effectDurationMs);if(stage<4){_time_display.clear();for(uint8_t p=0;p<=stage&&p<4;p++)for(uint8_t s=0;s<7;s++)_time_display.setPixelColor(p*7+s,_color);_time_display.show();}else drawNumberInternal(_effectResult,_effectLeadingZeros,false);return;
  }
  if(_effect==EFFECT_REVEAL_LEFT||_effect==EFFECT_REVEAL_RIGHT){
    uint8_t count=min((uint8_t)4,(uint8_t)(elapsed*5UL/_effectDurationMs));uint8_t d[4]={(uint8_t)(_effectResult/1000%10),(uint8_t)(_effectResult/100%10),(uint8_t)(_effectResult/10%10),(uint8_t)(_effectResult%10)};_time_display.clear();
    for(uint8_t i=0;i<count;i++){uint8_t p=_effect==EFFECT_REVEAL_LEFT?i:3-i;drawDigit(p,d[p]);}_time_display.show();return;
  }
}

void TimeDisplayLibrary::twentyfive(){showNumber(25);}
void TimeDisplayLibrary::five_ten(){showNumber(510);}
void TimeDisplayLibrary::zero(){showNumber(0,true);}
void TimeDisplayLibrary::ninefourtwo(){showNumber(942);}
void TimeDisplayLibrary::oneeightfourtwo(){showNumber(1842);}
