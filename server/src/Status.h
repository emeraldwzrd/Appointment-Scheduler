#pragma once
#include <unordered_map>
#include <string>

const std::string HTTP_VERSION = "HTTP/1.1";
const std::string HTTP_CONTENT_TYPE_PLAIN = "Content-Type: text/plain";
const std::string HTTP_CONTENT_TYPE_JSON = "Content-Type: application/json";
const std::string HTTP_OK = "200 OK";
const std::string HTTP_BAD_REQUEST = "400 Bad Request";
const std::string HTTP_NO_CONTENT = "204 No Content";

const int HTTP_OK_STANDARD_CODE = 1;
const int HTTP_BAD_REQUEST_STANDARD_CODE = 10;
const int HTTP_NO_CONTENT_STANDARD_CODE = 20;

struct HTTPCodeMessage {
   int code;
   std::string message;
};

// .             General HTTP Statuses
const std::unordered_map<std::string, HTTPCodeMessage> HTTP_CODE_MESSAGES = {
    {HTTP_BAD_REQUEST, {HTTP_BAD_REQUEST_STANDARD_CODE, "Unrecognized request"}},
    {HTTP_NO_CONTENT, {HTTP_NO_CONTENT_STANDARD_CODE, "There is no content"}},
    {HTTP_OK, {HTTP_OK_STANDARD_CODE, ""}},
};

// .             Appointment Statuses
enum AppointmentStatus {
    WRONG_DATE_FORMAT = 0,
    WRONG_TIME_FORMAT = 1,
    WRONG_USERID_FORMAT = 2,
    APPOINTMENT_SCHEDULED = 3,
    APPOINTMENT_ALREADY_SCHEDULED = 4,
    MISSING_PARAMETER = 5,
    STATUS_OK = 6,
    WRONG_USER_ID_FORMAT = 7,
    GET_SCHEDULED_APPOINTMENTS_SUCCESS = 8
};

struct AppointmentStatusMessage {
   std::string responseCode;
   HTTPCodeMessage codeMessage;
   std::string description;
};

const std::unordered_map<AppointmentStatus, AppointmentStatusMessage> STATUS_CODES = {
    {WRONG_DATE_FORMAT, {HTTP_BAD_REQUEST, HTTP_CODE_MESSAGES.at(HTTP_BAD_REQUEST), "Date format is incorrect. Please enter a valid YYYY-MM-DD"}},
    {WRONG_TIME_FORMAT, {HTTP_BAD_REQUEST, HTTP_CODE_MESSAGES.at(HTTP_BAD_REQUEST), "Time format is incorrect. Please enter a valid HH:MM"}},
    {WRONG_USER_ID_FORMAT, {HTTP_BAD_REQUEST, HTTP_CODE_MESSAGES.at(HTTP_BAD_REQUEST), "User ID format is incorrect. Please provide your user id as an unsigned integer"}},
    {MISSING_PARAMETER, {HTTP_BAD_REQUEST, HTTP_CODE_MESSAGES.at(HTTP_BAD_REQUEST), "Cannot complete the request due to a missing parameter."}},
    {APPOINTMENT_SCHEDULED, {HTTP_OK, HTTP_CODE_MESSAGES.at(HTTP_OK), "Your appointment has been scheduled."}},
    {APPOINTMENT_ALREADY_SCHEDULED, {HTTP_OK, HTTP_CODE_MESSAGES.at(HTTP_OK), "An appointment has already been scheduled for that day."}},
    {GET_SCHEDULED_APPOINTMENTS_SUCCESS, {HTTP_OK, HTTP_CODE_MESSAGES.at(HTTP_OK), "Here are your scheduled appointments:"}},
    {STATUS_OK, {HTTP_OK, HTTP_CODE_MESSAGES.at(HTTP_OK), ""}}
};