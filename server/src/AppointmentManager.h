#pragma once
#include "User.h"
#include "Appointment.h"
#include "Status.h"
#include <vector>
#include <unordered_map>
#include <ctime>

struct AppointmentEndpointResponse {
  AppointmentStatus status;
  std::vector<Appointment*> appointments;
};

class AppointmentManager {
private:
  std::vector<Appointment> appointments;
  std::unordered_map<uint64_t, User*> userCache;
public:
  AppointmentEndpointResponse getAppointments(uint64_t userid);
  AppointmentEndpointResponse getAppointmentsForUser(std::string jsonContent);
  AppointmentEndpointResponse scheduleAppointmentForUser(std::string jsonContent);
  AppointmentEndpointResponse addAppointment(uint64_t userid, DateTime date);
  std::string createAppointmentWebPageResponse(AppointmentEndpointResponse response);
};