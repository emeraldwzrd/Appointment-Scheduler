#pragma once
#include <string>
#include <time.h>
struct DateTime {
  int year;
  int month;
  int day;
  int hour;
  int minute;
};
class Appointment {
private:
  uint64_t userid;
  DateTime date;
public:
  Appointment(uint64_t userid, DateTime date) : userid(userid), date(date){};
  Appointment(){};
  uint64_t getUserID() {
       return userid;
  }
  int getYear() {
       return date.year;
  }
  int getMonth() {
       return date.month;
  }
  int getDay() {
       return date.day;
  }
  int getHour() {
       return date.hour;
  }
  int getMinute() {
       return date.minute;
  }
};