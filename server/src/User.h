#pragma once
#include <cstdint>
#include "Appointment.h"
#include <vector>
class User {
private:
  uint64_t userid;
  std::vector<Appointment*> appointments;
public:
  User(uint64_t id) : userid(id){}
  void setUserID(uint64_t id) {
      userid = id;
  } 
  uint64_t getUserID() {
    return userid;
  } 
  std::vector<Appointment*> getAppointments() {
    return appointments;
  };
  void addAppointment(Appointment* appointment) {
      appointments.push_back(appointment);
  };
};