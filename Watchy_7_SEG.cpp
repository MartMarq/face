#include "Watchy_7_SEG.h"


void Watchy7SEG::drawWatchFace() {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);

    drawTime();
    // reset step counter at midnight
    if (currentTime.Hour == 00 && currentTime.Minute == 00) {
        sensor.resetStepCounter();
        //turn off radios
        WiFi.mode(WIFI_OFF);
        btStop();
    }

    //time adjustment start
    //if you don't need a time adjustment, just delete all lines between time adjustment start and end
    //my watchy runs too fast (15 seconds and 500 ms a day), this code set back time each day by 15 seconds and 500 ms

    if (currentTime.Hour == 00 && currentTime.Minute == 30) {

        RTC.read(currentTime);
        int8_t sekunde = currentTime.Second;
        int8_t minute = currentTime.Minute;
        int8_t hour = currentTime.Hour;
        int8_t day = currentTime.Day;
        int8_t month = currentTime.Month;
        int8_t year = tmYearToY2k(currentTime.Year);

        delay(15500);

        tmElements_t tm;
        tm.Month = month;
        tm.Day = day;
        tm.Year = y2kYearToTm(year);
        tm.Hour = hour;
        tm.Minute = minute;
        tm.Second = sekunde;

        RTC.set(tm);
    }

    // time adjustment end
    
          
}


void Watchy7SEG::drawTime() {
    int8_t stundeA = currentTime.Hour;
    int minuteA = currentTime.Minute;
    int  s1 = sensor.getCounter();
    int tagA = currentTime.Day;
    int monatA = currentTime.Month;
    int n, hour;
    int16_t  xc, yc;
    uint16_t wc, hc;
    
    float VBAT = getBatteryVoltage();
    uint8_t Protz = min<int>(100 - ((4.16 - VBAT) * 100), 100);
    
    
    display.drawBitmap(0, 0, xface, 200, 200, GxEPD_BLACK);
    display.drawCircle(75, 70, 30, GxEPD_BLACK);
    display.drawCircle(135, 60, 30, GxEPD_BLACK);
    display.drawCircle(75, 70, 31, GxEPD_BLACK);
    display.drawCircle(135, 60, 31, GxEPD_BLACK);
    display.drawCircle(75, 70, 32, GxEPD_BLACK);
    display.drawCircle(135, 60, 32, GxEPD_BLACK);
    display.setFont(&FreeSansBold9pt7b);

    display.getTextBounds(String(monatA), 100, 105, &xc, &yc, &wc, &hc);
    display.setCursor(106 - (wc / 2), 180);
    display.println(monatA);
    
    display.getTextBounds(String(tagA), 100, 105, &xc, &yc, &wc, &hc);
    display.setCursor(80 - (wc / 2), 180);
    display.println(tagA);
    
    double winke = (stundeA * 30) + minuteA * 0.5;
    double lange = ((winke + 180) * 71) / 4068.0;
    double cx1 = 75 - (sin(lange) * 20);
    double cy1 = 70 + (cos(lange) * 20);
    display.fillCircle(cx1, cy1,  10, GxEPD_BLACK);
    display.fillCircle(cx1, cy1, 1,  GxEPD_WHITE);

    winke = minuteA *6;
    lange = ((winke + 180) * 71) / 4068.0;
    cx1 = 135 - (sin(lange) * 20);
    cy1 = 60 + (cos(lange) * 20);
    display.fillCircle(cx1, cy1, 10, GxEPD_BLACK);
    display.fillCircle(cx1, cy1, 1, GxEPD_WHITE);

    cx1 = Protz / 7;
    display.fillCircle(105, 115, cx1, GxEPD_BLACK);
}

