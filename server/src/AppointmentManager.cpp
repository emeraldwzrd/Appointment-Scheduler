#include <string>
#include "../../dependencies/rapidjson/document.h"
#include "../../dependencies/rapidjson/writer.h"
#include "../../dependencies/rapidjson/stringbuffer.h"
#include "../../dependencies/rapidjson/pointer.h"
#include "AppointmentManager.h"
#include "Status.h"
#include <unordered_map>
#include <iostream>

namespace rj = rapidjson;

#define YEAR_MAX 2999
#define YEAR_MIN 2000
#define MONTH_MAX 12
#define MONTH_MIN 1
#define HOUR_MAX 23
#define HOUR_MIN 0
#define MINUTE_MAX 30
#define MINUTE_MIN 0
#define DAY_MIN 1


std::unordered_map<int,int> DAYS_IN_MONTH = {
       {1, 31},
       {2, 28},
       {3, 31},
       {4, 30},
       {5, 31},
       {6, 30},
       {7, 31},
       {8, 31},
       {9, 30},
       {10, 31},
       {11, 30},
       {12, 31},
};

AppointmentStatus checkDateTimeBounds(DateTime datetime) {
     if(datetime.year < YEAR_MIN || datetime.year > YEAR_MAX) {
        return WRONG_DATE_FORMAT;
     }
     if(datetime.month < MONTH_MIN || datetime.month > MONTH_MAX) {
        return WRONG_DATE_FORMAT;
     }
     if(datetime.day < DAY_MIN || datetime.day > DAYS_IN_MONTH[datetime.month]) {
        return WRONG_DATE_FORMAT;
     }
     if(datetime.hour < HOUR_MIN || datetime.hour > HOUR_MAX) {
        return WRONG_TIME_FORMAT;
     }
     if(datetime.minute != MINUTE_MIN && datetime.minute != MINUTE_MAX) {
         std::cout << "MINUTE: " << datetime.minute << std::endl;
        return WRONG_TIME_FORMAT;
     }
     return STATUS_OK;
}

AppointmentEndpointResponse AppointmentManager::getAppointments(uint64_t userid) {
    // An in memory cache is used to simulate caching database results and
    // here to speed up lookup times in general
    // Anytime we get an appointment we first check to see if the user is in the cache
    if(userCache.find(userid) != userCache.end()) {
        return {STATUS_OK, userCache[userid]->getAppointments()};
    }
    User *user = new User(userid); 
    // If not we search the the simulated database (the vector) to
    // find appointments for the given user id
    for(int i = 0; i < appointments.size(); i++){ 
       if(appointments[i].getUserID() == userid) {
           user->setUserID(userid);
           user->addAppointment(&appointments[i]);
       }
    }
    userCache[userid] = user;
    return {GET_SCHEDULED_APPOINTMENTS_SUCCESS, user->getAppointments()};
}

AppointmentEndpointResponse AppointmentManager::getAppointmentsForUser(std::string jsonContent) {
    rj::Document doc;
    doc.Parse(jsonContent.c_str());
    rj::Value& uid = doc["uid"];
    uint64_t userid = 0;
    if(rj::Value* uid = rj::Pointer("/uid").Get(doc)) {
        if(uid->IsUint64()) {
            userid = uid->GetUint64();
        } else {
            return {WRONG_USER_ID_FORMAT, {}};
        }
    }
    return getAppointments(userid);
} 

AppointmentEndpointResponse AppointmentManager::scheduleAppointmentForUser(std::string jsonContent) {
    rj::Document doc;
    doc.Parse(jsonContent.c_str());
    uint64_t userid = 0;
    std::string dateString = "";
    std::string timeString = "";
    AppointmentEndpointResponse response = {STATUS_OK, {}};
    if(rj::Value* uid = rj::Pointer("/uid").Get(doc)) {
        if(uid->IsUint64()) {
            userid = uid->GetUint64();
        } else {
            response.status = WRONG_USER_ID_FORMAT;
            return response;
        }
    }
    if(rj::Value* date = rj::Pointer("/date").Get(doc)) {
        if(date->IsString()) {
            dateString = date->GetString();
        }
    }
    if(rj::Value* time = rj::Pointer("/time").Get(doc)) {
        if(time->IsString()) {
            timeString = time->GetString();
        }
    }
    //// Parse Date
    DateTime datetime;
    std::string delimiter = "-";
    try {
        datetime.year = stoi(dateString.substr(0, dateString.find(delimiter)));
        dateString.erase(0, dateString.find(delimiter) + delimiter.length());
        datetime.month = stoi(dateString.substr(0, dateString.find(delimiter)));
        dateString.erase(0, dateString.find(delimiter) + delimiter.length());
        datetime.day = stoi(dateString.substr(0, dateString.find(delimiter)));
    } catch (...) {
       response.status = WRONG_DATE_FORMAT;
       return response;
    }
    //// Parse Time
    delimiter = ":";
    try {
        datetime.hour = stoi(timeString.substr(0, timeString.find(delimiter)));
        timeString.erase(0, timeString.find(delimiter) + delimiter.length());
        datetime.minute = stoi(timeString.substr(0, timeString.find(delimiter)));
    } catch (...) {
        response.status = WRONG_TIME_FORMAT;
        return response;
    }
    response.status = checkDateTimeBounds(datetime);
    if(response.status != STATUS_OK) {
        return response;
    }
    return addAppointment(userid, datetime);
}

AppointmentEndpointResponse AppointmentManager::addAppointment(uint64_t userid, DateTime date) {
    std::vector<Appointment*> userApps = getAppointments(userid).appointments;
    for(auto app : userApps) {
        if(app->getDay() == date.day &&
           app->getMonth() == date.month && 
           app->getYear() == date.year) {
            return {APPOINTMENT_ALREADY_SCHEDULED, {}};
        }
    }
    // Any time we add an appointment we clear the user cache for
    // the given user id
    // This allows every getAppointment call to always have the most
    // up to date information
    appointments.push_back(Appointment(userid, date));
    userCache.erase(userid);
    return {APPOINTMENT_SCHEDULED, {&appointments[appointments.size() - 1]}};
};

std::string AppointmentManager::createAppointmentWebPageResponse(AppointmentEndpointResponse response) {
    std::string webPage = HTTP_VERSION + " " + STATUS_CODES.at(response.status).responseCode + "\n";
    webPage += HTTP_CONTENT_TYPE_PLAIN + "\n";
    std::string appointmentsString = "";
    std::string appointmentSeparator = "------------------------------------\n";
    for(auto appointment : response.appointments) {
         appointmentsString += appointmentSeparator;
         appointmentsString += "User ID: " + std::to_string(appointment->getUserID()) + "\n";
         appointmentsString += "Date: " + std::to_string(appointment->getYear())
                                  + "-" + std::to_string(appointment->getMonth())
                                  + "-" + std::to_string(appointment->getDay()) + "\n";
         appointmentsString += "Time: " + std::to_string(appointment->getHour())
                                  + ":" + std::to_string(appointment->getMinute()) + "\n";
    }
    if(!response.appointments.empty()) {
        appointmentsString += appointmentSeparator;
    }
    webPage += "Content-Length: " + std::to_string(STATUS_CODES.at(response.status).description.length() 
                                  + appointmentsString.length()) + "\n\n";
    webPage += STATUS_CODES.at(response.status).description + "\n";
    webPage += appointmentsString;
    return webPage;
}