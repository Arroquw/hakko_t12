#include "config.h"
#include "dspl_0802.h"
#include "iron_tips.h"

#ifdef DISPLAY_0802

#ifdef DISPLAY_1602
// Custom LCD Characters
// Special Characters
// Temperature Degree Character
byte degreeChar[8] = { 0b00100, 0b01010, 0b00100, 0b00000, 0b00000, 0b00000,
                       0b00000, 0b00000 };

// Head Character
byte headChar[8] = { 0b00100, 0b01110, 0b11111, 0b00100, 0b00100, 0b00100,
                     0b00100, 0b00100 };

// PWM Character
byte pwmChar[8] = { 0b10111, 0b10101, 0b10101, 0b10101, 0b10101, 0b10101,
                    0b11101, 0b00000 };

// SET Char
byte setChar[8] = { 0b00100, 0b01010, 0b01010, 0b01010, 0b10001, 0b10001,
                    0b01110, 0b00000 };

// TIP Char
byte tipChar[8] = { 0b01110, 0b01010, 0b01010, 0b01010, 0b11011, 0b01110,
                    0b00100, 0b00000 };

#define LCD_PRINT_PRE_SPACE()
#define LCD_PRINT_POST_SPACE() LiquidCrystal::print(F(" "))
#define LCD_PRINT_SPACING() LiquidCrystal::print(F("       "))
#define MSG_CURSOR_X 12
#define MAX_COLUMNS 16
#else
#define MAX_COLUMNS 8
#define MSG_CURSOR_X 4
#define LCD_PRINT_PRE_SPACE() LiquidCrystal::print(F(" "))
#define LCD_PRINT_POST_SPACE()
#define LCD_PRINT_SPACING()
#endif

#define MAX_ROWS 2


void DSPL::init(void) {
  LiquidCrystal::begin(MAX_COLUMNS, MAX_ROWS);
  LiquidCrystal::clear();
#ifdef DISPLAY_1602
  LiquidCrystal::createChar(PWM_CHAR, pwmChar);
  LiquidCrystal::createChar(DEGREE_CHAR, degreeChar);
  LiquidCrystal::createChar(SET_CHAR, setChar);
  LiquidCrystal::createChar(TIP_CHAR, tipChar);
#endif
  full_second_line = false;
}

void DSPL::tip(const char *tip_name, bool top) {
  uint8_t y = 1;
  if (top) y = 0;
#ifdef DISPLAY_1602
  LiquidCrystal::setCursor(8, 0);
#else
  LiquidCrystal::setCursor(0, y);
#endif
  LiquidCrystal::print(tip_name);
  for (uint8_t i = strlen(tip_name); i < 8; ++i)
    LiquidCrystal::print(' ');
}

void DSPL::msgSelectTip(void) {
  LiquidCrystal::setCursor(0, 0);
  LiquidCrystal::print(F("   TIP:"));
}

void DSPL::msgActivateTip(void) {
  LiquidCrystal::setCursor(0, 0);
  LiquidCrystal::print(F("act. tip"));
}

void DSPL::tSet(uint16_t t, bool celsius) {
  char buff[10];
  char units = 'C';
  if (!celsius) units = 'F';
  LiquidCrystal::setCursor(0, 0);
  sprintf(buff, "%c%3d%c%c", ((uint8_t)SET_CHAR), t, ((uint8_t)DEGREE_CHAR), units);
  LiquidCrystal::print(buff);
}

#ifdef DISPLAY_1602
void DSPL::tCurr(uint16_t t, bool celsius) {
  char buff[10];
  char units = 'C';
  if (!celsius) units = 'F';
  LiquidCrystal::setCursor(0, 1);
  if (t < 1000) {
    sprintf(buff, "%c%3d%c%c", ((uint8_t)TIP_CHAR), t, ((uint8_t)DEGREE_CHAR), units);
  } else {
    LiquidCrystal::print(F("xxx"));
    return;
  }
  LiquidCrystal::print(buff);
  if (full_second_line) {
    LiquidCrystal::print(F("            "));
    full_second_line = false;
  }
}

void DSPL::tCurrTune(uint16_t t, bool celsius) {
  char buff[10];
  char units = 'C';
  if (!celsius) units = 'F';
  LiquidCrystal::setCursor(10, 0);
  if (t < 1000) {
    sprintf(buff, "%c%3d%c%c", ((uint8_t)TIP_CHAR), t, ((uint8_t)DEGREE_CHAR), units);
  } else {
    LiquidCrystal::print(F("xxx"));
    return;
  }
  LiquidCrystal::print(buff);
  if (full_second_line) {
    LiquidCrystal::print(F("            "));
    full_second_line = false;
  }
}

#endif

void DSPL::tCurr(uint16_t t) {
  char buff[4];
  LiquidCrystal::setCursor(0, 1);
  if (t < 1000) {
    sprintf(buff, "%3d", t);
  } else {
    LiquidCrystal::print(F("xxx"));
    return;
  }
  LiquidCrystal::print(buff);
  if (full_second_line) {
    LiquidCrystal::print(F("    "));
    full_second_line = false;
  }
}

void DSPL::pSet(uint8_t p) {
  char buff[6];
  sprintf(buff, "P:%3d", p);
  LiquidCrystal::setCursor(0, 0);
  LiquidCrystal::print(buff);
}

void DSPL::tRef(uint8_t ref) {
  char buff[9];
  LiquidCrystal::setCursor(0, 0);
  sprintf(buff, "Ref. #%1d ", ref + 1);
  LiquidCrystal::print(buff);
}

void DSPL::timeToOff(uint8_t sec) {
  char buff[5];
  sprintf(buff, " %3d", sec);
  LiquidCrystal::setCursor(MSG_CURSOR_X, 0);
  LiquidCrystal::print(buff);

#ifdef DISPLAY_1602
  LiquidCrystal::setCursor(7, 0);
  LiquidCrystal::print(F("AutoOFF"));
#endif
}

void DSPL::msgReady(void) {
  LiquidCrystal::setCursor(MSG_CURSOR_X, 0);
  LCD_PRINT_PRE_SPACE();
  LiquidCrystal::print(F("RDY"));
  LCD_PRINT_POST_SPACE();
}

void DSPL::msgWorking(void) {
  LiquidCrystal::setCursor(MSG_CURSOR_X, 0);
  LCD_PRINT_PRE_SPACE();
  LiquidCrystal::print(F("WRK"));
}

void DSPL::msgOn(void) {
  LiquidCrystal::setCursor(MSG_CURSOR_X, 0);
  LCD_PRINT_PRE_SPACE();
  LiquidCrystal::print(F("ON"));
  LCD_PRINT_POST_SPACE();
}

void DSPL::msgOff(void) {
  LiquidCrystal::setCursor(MSG_CURSOR_X, 0);
  LCD_PRINT_PRE_SPACE();
  LiquidCrystal::print(F("OFF"));
  LCD_PRINT_POST_SPACE();
}

void DSPL::msgStandby(void) {
  LiquidCrystal::setCursor(MSG_CURSOR_X, 0);
  LiquidCrystal::print(F(" STB"));
}

void DSPL::msgCold(void) {
#ifdef DISPLAY_1602
  uint8_t x = 2;
#else
  uint8_t x = 0;
#endif
  LiquidCrystal::setCursor(x, 1);
  LiquidCrystal::print(F("  Cold  "));
  full_second_line = true;
}

void DSPL::msgFail(void) {
  LiquidCrystal::setCursor(0, 1);
  LiquidCrystal::print(F(" FAILED "));
}

void DSPL::msgTune(void) {
  LiquidCrystal::setCursor(0, 0);
  LiquidCrystal::print(F("Tune"));
}

void DSPL::msgCelsius(void) {
  LiquidCrystal::setCursor(0, 1);
  LiquidCrystal::print(F("Celsius "));
}

void DSPL::msgFarneheit(void) {
  LiquidCrystal::setCursor(0, 1);
  LiquidCrystal::print(F("Fahrenh. "));
  LCD_PRINT_SPACING();
}

void DSPL::msgDefault() {
  LiquidCrystal::setCursor(0, 1);
  LiquidCrystal::print(F(" Default"));
  LCD_PRINT_SPACING();
}

void DSPL::setupMode(uint8_t mode, bool tune, uint16_t p) {
  char buff[5];
  LiquidCrystal::clear();
  if (!tune) {
    LiquidCrystal::print(F("Setup"));
    LiquidCrystal::setCursor(1, 1);
  }
  switch (mode) {
    case 0:  // C/F. In-line editing
      LiquidCrystal::print(F("Units"));
      LiquidCrystal::setCursor(7, 1);
      if (p)
        LiquidCrystal::print("C");
      else
        LiquidCrystal::print("F");
      break;
    case 1:  // Buzzer
      LiquidCrystal::print(F("Buzzer"));
      if (tune) {
        LiquidCrystal::setCursor(1, 1);
        if (p)
          LiquidCrystal::print(F("ON"));
        else
          LiquidCrystal::print(F("OFF"));
      }
      break;
    case 2:  // Switch type
#ifdef DISPLAY_1602
      LiquidCrystal::print(F("Switch Type"));
#else
      LiquidCrystal::print(F("switch"));
#endif
      if (tune) {
        LiquidCrystal::setCursor(1, 1);
        if (p)
          LiquidCrystal::print(F("REED"));
        else
          LiquidCrystal::print(F("TILT"));
      }
      break;
    case 3:  // ambient temperature sensor
#ifdef DISPLAY_1602
      LiquidCrystal::print(F("Ambient Temp"));
#else
      LiquidCrystal::print(F("ambient"));
#endif
      if (tune) {
        LiquidCrystal::setCursor(1, 1);
        if (p)
          LiquidCrystal::print(F("ON"));
        else
          LiquidCrystal::print(F("OFF"));
      }
      break;
    case 4:  // standby temperature
#ifdef DISPLAY_1602
      LiquidCrystal::print(F("ST-BY TEMP"));
#else
      LiquidCrystal::print(F("stby T."));
#endif
      if (tune) {
        LiquidCrystal::setCursor(1, 1);
        if (p > 0) {
          sprintf(buff, "%3d", p);
          LiquidCrystal::print(buff);
        } else {
          LiquidCrystal::print(" NO");
        }
      }
      break;
    case 5:  // Standby Time
#ifdef DISPLAY_1602
      LiquidCrystal::print(F("ST-BY Time"));
#else
      LiquidCrystal::print(F("stby tm"));
#endif
      if (tune) {
        LiquidCrystal::setCursor(1, 1);
        sprintf(buff, "%3ds", p);
        LiquidCrystal::print(buff);
      }
      break;
    case 6:  // off-timeout
#ifdef DISPLAY_1602
      LiquidCrystal::print(F("AutoOff Time"));
#else
      LiquidCrystal::print(F("off tm"));
#endif
      if (tune) {
        LiquidCrystal::setCursor(1, 1);
        if (p > 0) {
          sprintf(buff, "%2dm", p);
          LiquidCrystal::print(buff);
        } else {
          LiquidCrystal::print(" NO");
        }
      }
      break;
    case 7:  // Tip calibration
#ifdef DISPLAY_1602
      LiquidCrystal::print(F("Tip Calibration"));
#else
      LiquidCrystal::print(F("tip cfg"));
#endif

      break;
    case 8:  // Activate tip
#ifdef DISPLAY_1602
      LiquidCrystal::print(F("Tip Activation"));
#else
      LiquidCrystal::print(F("activ."));
#endif
      break;
    case 9:  // Tune controller
      LiquidCrystal::print(F("Tune"));
      break;
    case 10:  // save
      LiquidCrystal::print(F("Apply"));
      break;
    case 11:  // cancel
      LiquidCrystal::print(F("Cancel"));
    default:
      break;
  }
}

void DSPL::percent(uint8_t Power) {
  char buff[6];
#ifdef DISPLAY_1602
  sprintf(buff, "%c%3d%c", ((uint8_t)PWM_CHAR), Power, '%');
#else
  sprintf(buff, " %3d%c", Power, '%');
#endif
  LiquidCrystal::setCursor(MSG_CURSOR_X - 1, 1);
  LiquidCrystal::print(buff);
}

void DSPL::mark(char sym, bool on) {
  char buff[5];
  for (uint8_t i = 0; i < 4; ++i) buff[i] = ' ';
  if (on) buff[3] = sym;
  buff[4] = '\0';
  LiquidCrystal::setCursor(4, 1);
  LiquidCrystal::print(buff);
}

#ifdef DISPLAY_1602
void DSPL::markCalibrated(char sym, bool calibrated) {
  char buff[5];
  for (uint8_t i = 0; i < 4; ++i) buff[i] = ' ';
  if (calibrated) buff[3] = sym;
  //LiquidCrystal::setCursor(0, 1);
  //LiquidCrystal::print (buff);
  LiquidCrystal::setCursor(0, 0);
  LiquidCrystal::print(F("   TIP:"));
  LiquidCrystal::setCursor(0, 1);
  if (calibrated) {
    LiquidCrystal::print("  *Calibrated* ");
  } else {
    LiquidCrystal::print(" *Uncalibrated*");
  }
}

void DSPL::markActivated(char sym, bool on) {
  char buff[5];
  for (uint8_t i = 0; i < 4; ++i) buff[i] = ' ';
  if (on) buff[3] = sym;
  buff[4] = '\0';
  //LiquidCrystal::setCursor(0, 1);
  //LiquidCrystal::print (buff);
  LiquidCrystal::setCursor(0, 0);
  LiquidCrystal::print(F("   Tip:"));
  LiquidCrystal::setCursor(0, 1);
  if (on) {
    LiquidCrystal::print("  *Activated* ");
  } else {
    LiquidCrystal::print("  *Deactivated*");
  }
}
#endif
#endif
