#include <WiFi.h>
#include "time.h"

const char* ssid       = "CCIT";
const char* password   = "rkdtjdus";
const char* ntpServer  = "pool.ntp.org";
uint8_t timeZone = 9;
uint8_t summerTime = 0; // 3600

int s_hh = 12;      // 시간 설정 변수 < 0 조건 위해 자료형 int
int s_mm = 59;
uint8_t s_ss = 45;
uint16_t s_yy = 2019;
uint8_t s_MM = 10;
uint8_t s_dd = 19;

time_t now;
time_t prevEpoch;
struct tm timeinfo;

bool ntp_failed = false;

void check_ntp_sync() {
  s_dd = timeinfo.tm_mday;
  s_MM = timeinfo.tm_mon + 1;
  s_yy = timeinfo.tm_year +1900;
  s_ss = timeinfo.tm_sec;
  s_mm = timeinfo.tm_min;
  s_hh = timeinfo.tm_hour;
  Serial.println("check NTP sync");
  struct timeval now = { .tv_sec = 0 };
  settimeofday(&now, NULL);
}

void get_NTP() {
  check_ntp_sync();
  configTime(3600 * timeZone, 3600 * summerTime, ntpServer);
  prevEpoch = time(&now);
  Serial.println(prevEpoch);  // NTP sync value
  while(prevEpoch < 5) {      // NTP sync가 안된동안
    if (time(&now) > 5) return;         // NTP sync가 성공했으면 빠져 나간다
    else if (time(&now) != prevEpoch) { // 1초마다 3번 NTP sync 시도
      if (time(&now) > 5) return;       // NTP sync가 성공했으면 빠져 나간다
      else {
        Serial.println(time(&now));
        if (time(&now) == 1) configTime(3600 * timeZone, 3600 * summerTime, ntpServer);
        else if (time(&now) == 2) configTime(3600 * timeZone, 3600 * summerTime, ntpServer);
        else if (time(&now) == 3) configTime(3600 * timeZone, 3600 * summerTime, ntpServer); 
        else if (time(&now) == 4) {             // NTP sync 3번 실패할 경우 이전 시간으로 복귀
          Serial.println("Failed ntp sync");    
          ntp_failed = true;
          set_time(); 
          return;
        }
      }
      prevEpoch = time(&now);
    }
  }
}

void set_time() {
  struct tm tm;
  tm.tm_year = s_yy - 1900;
  tm.tm_mon = s_MM - 1;
  tm.tm_mday = s_dd;
  tm.tm_hour = s_hh;
  tm.tm_min = s_mm;
  tm.tm_sec = s_ss;
  time_t t = mktime(&tm);
  printf("Setting time: %s", asctime(&tm));
  if (!ntp_failed) {
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);
  }
  else {
    struct timeval now = { .tv_sec = t + 5};  // NTP 시도에 걸린 시간 가산
    settimeofday(&now, NULL);
    ntp_failed = false;
  }
}

void printLocalTime() {
  if (time(&now) != prevEpoch) {
    Serial.println(time(&now));  // UTC 시간 값 출력
    getLocalTime(&timeinfo);
    int dd = timeinfo.tm_mday;
    int MM = timeinfo.tm_mon + 1;
    int yy = timeinfo.tm_year +1900;
    int ss = timeinfo.tm_sec;
    int mm = timeinfo.tm_min;
    int hh = timeinfo.tm_hour;
    int week = timeinfo.tm_wday;
    Serial.print(week); Serial.print(". ");
    Serial.print(yy); Serial.print(". ");
    Serial.print(MM); Serial.print(". ");
    Serial.print(dd); Serial.print(" ");
    Serial.print(hh); Serial.print(": ");
    Serial.print(mm); Serial.print(": ");
    Serial.println(ss);
    prevEpoch = time(&now);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  uint8_t wifi_check = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifi_check ++;
    if (wifi_check == 20) {
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      break;                  // while 루프 빠져나감
    }
  }
  if (wifi_check == 20) {
    Serial.println("Failed wifi connection");
    set_time();
  }
  else {
    Serial.println(" CONNECTED");
    configTime(3600 * timeZone, 3600 * summerTime, ntpServer); // NTP sync
    while(!getLocalTime(&timeinfo)){                           // 5초 동안 딜레이
      Serial.println("Failed to obtain time");
      set_time();                                              // set time
      return;
    }
  }
}

void loop() {
  if(Serial.available() > 0){
    String temp = Serial.readStringUntil('\n');
    if (temp == "1") set_time();   // set time
    else if (temp == "2") get_NTP(); //NTP Sync
  }
  printLocalTime();
}
