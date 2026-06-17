// StreetLight OOP
// Using a DIGITAL-ONLY pin (no PWM/analogWrite).
// "Fading" is simulated with software PWM: update() rapidly toggles the
// pin HIGH/LOW within a fixed-length time slot, where the HIGH portion of
// the slot ("duty cycle") represents brightness (0-255 -> 0-100% of slot).
//
// Timeline per 20s cycle:
//   0s       - begin fade IN
//   ~2s      - fully ON
//   5s       - flicker burst
//   ~5.5s    - resume ON
//   10s      - begin fade OUT
//   ~12s     - fully OFF
//   20s      - repeat

class StreetLight {
public:
  StreetLight(uint8_t pin)
    : _pin(pin),
      _cycleStart(0),
      _slotStart(0),
      _brightness(0),
      _state(FADING_IN),
      _pinState(LOW) {}

  void begin() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    _cycleStart = millis();
    _slotStart  = _cycleStart;
    _brightness = 0;
    _state = FADING_IN;
  }

  void update() {
    unsigned long now = millis();
    unsigned long elapsed = now - _cycleStart;

    // Restart cycle every 20 seconds
    if (elapsed >= CYCLE_MS) {
      _cycleStart = now;
      elapsed = 0;
      _brightness = 0;
      _state = FADING_IN;
    }

    switch (_state) {

      // 0 – 2000 ms: fade in from 0 -> 255
      case FADING_IN: {
        _brightness = map(elapsed, 0, FADE_IN_MS, 0, 255);
        _brightness = constrain(_brightness, 0, 255);
        if (elapsed >= FADE_IN_MS) {
          _brightness = 255;
          _state = ON;
        }
        break;
      }

      // 2000 – 5000 ms: fully on
      case ON: {
        _brightness = 255;
        if (elapsed >= FLICKER_START_MS) {
          _flickerEnd  = now + FLICKER_DURATION_MS;
          _nextFlicker = now;
          _state = FLICKERING;
        }
        break;
      }

      // 5000 – 5500 ms: rapid random flicker
      case FLICKERING: {
        if (now >= _nextFlicker) {
          _brightness = (random(2) == 0) ? 255 : random(60, 180);
          _nextFlicker = now + random(40, 120);  // flicker every 40-120 ms
        }
        if (now >= _flickerEnd) {
          _brightness = 255;
          _state = ON_AFTER_FLICKER;
        }
        break;
      }

      // 5500 – 10000 ms: back to fully on
      case ON_AFTER_FLICKER: {
        _brightness = 255;
        if (elapsed >= FADE_OUT_START_MS) {
          _state = FADING_OUT;
        }
        break;
      }

      // 10000 – 12000 ms: fade out from 255 -> 0
      case FADING_OUT: {
        unsigned long fadeElapsed = elapsed - FADE_OUT_START_MS;
        _brightness = map(fadeElapsed, 0, FADE_OUT_MS, 255, 0);
        _brightness = constrain(_brightness, 0, 255);
        if (elapsed >= FADE_OUT_START_MS + FADE_OUT_MS) {
          _brightness = 0;
          _state = OFF;
        }
        break;
      }

      // 12000 – 20000 ms: fully off, wait for cycle reset
      case OFF: {
        _brightness = 0;
        break;
      }
    }

    applySoftwarePWM(now);
  }

private:
  // Timing constants (ms)
  static const unsigned long CYCLE_MS            = 20000UL;
  static const unsigned long FADE_IN_MS          =  2000UL;
  static const unsigned long FLICKER_START_MS    =  5000UL;
  static const unsigned long FLICKER_DURATION_MS =   500UL;
  static const unsigned long FADE_OUT_START_MS   = 10000UL;
  static const unsigned long FADE_OUT_MS         =  2000UL;

  // Software-PWM slot length in ms. Brightness 0-255 maps to an ON
  // duration of 0-PWM_SLOT_MS within each slot. Shorter slot = less
  // visible flicker but more digitalWrite() calls.
  static const unsigned long PWM_SLOT_MS = 20UL;

  enum State {
    FADING_IN,
    ON,
    FLICKERING,
    ON_AFTER_FLICKER,
    FADING_OUT,
    OFF
  };

  void applySoftwarePWM(unsigned long now) {
    unsigned long inSlot = now - _slotStart;

    if (inSlot >= PWM_SLOT_MS) {
      // start a new slot
      _slotStart = now;
      inSlot = 0;
    }

    if (_brightness <= 0) {
      setPin(LOW);
      return;
    }
    if (_brightness >= 255) {
      setPin(HIGH);
      return;
    }

    unsigned long onTime = (PWM_SLOT_MS * (unsigned long)_brightness) / 255UL;
    setPin(inSlot < onTime ? HIGH : LOW);
  }

  void setPin(int level) {
    if (level != _pinState) {
      digitalWrite(_pin, level);
      _pinState = level;
    }
  }

  uint8_t       _pin;
  unsigned long _cycleStart;
  unsigned long _slotStart;
  unsigned long _flickerEnd;
  unsigned long _nextFlicker;
  int           _brightness;
  State         _state;
  int           _pinState;
};