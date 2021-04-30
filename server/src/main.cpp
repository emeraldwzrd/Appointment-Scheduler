// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <iostream>
#include <exception>
#include "AppointmentManager.h"

// Stand port definition
#define PORT 8080

void sendMessage(int socket, std::string response) {
    write(socket , response.c_str() , response.length());
    printf("------------------message sent-------------------\n");
    close(socket);
}

std::string createStandardJSONResponseFromStatusCode(std::string statusCode) {
    std::string response = HTTP_VERSION + " " + statusCode + "\n"; 
    response += HTTP_CONTENT_TYPE_JSON + "\n\n";
    response += "{";
    response += "\"code\":" + std::to_string(HTTP_CODE_MESSAGES.at(statusCode).code) + ",";
    response += "\"message\":\"" + HTTP_CODE_MESSAGES.at(statusCode).message + "\"";
    response += "}";
    return response;
}

int main(int argc, char const *argv[])
{
    // Most of the server code I found from a tutorial
    // that I will link in the README
    int server_fd, new_socket; 
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    const int option_value = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int)) < 0) { 
        perror("setsockopt(SO_REUSEADDR) failed");
    }
  
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    // Create a new Appointment Manager that lasts the whole session
    AppointmentManager am;
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , &buffer, 30000);
        std::string query(buffer);
        // This method parsing code was borrowed from StackOverflow
        // that I will link in the README
        std::string method, path, action, response;
        // First check the method at the beginning 
        if (query.substr(0, 5) == "GET /") {  
            method = "GET";
        } else if (query.substr(0, 6) == "POST /") {
            method = "POST";
        } else {
            response = createStandardJSONResponseFromStatusCode(HTTP_BAD_REQUEST);
            sendMessage(new_socket, response);
            continue;
        }

        // Take the rest, ignoring whitespace and slash
        path = query.substr(method.length() + 2);
        // Find the end of the url
        size_t ph = path.find(" HTTP"); 
        // If it's not found return bad request
        if (ph == std::string::npos) {           
           response = createStandardJSONResponseFromStatusCode(HTTP_BAD_REQUEST);
           sendMessage(new_socket, response);
           continue;
        } else {
            // Otherwise get rid of the end of the string
            path.resize(ph);              
        }

        // Look for the ?
        size_t pq = path.find("?");
        // If it's absent, action is the whole string
        if (pq == std::string::npos) {        
            action = path;
        } else {                           
            // Users can provide additional parameters, but they will be ignored
            action = path.substr(0, pq);
        }
        AppointmentEndpointResponse apptResponse;
        std::string jsonContent = "";
        try {
            jsonContent = query.substr(query.find("{"), query.find_last_of("}"));
        } catch (...) {
           response = createStandardJSONResponseFromStatusCode(HTTP_BAD_REQUEST);
           sendMessage(new_socket, response);
           continue;
        }
        if(method == "POST" && action == "schedule") {
            apptResponse = am.scheduleAppointmentForUser(jsonContent);
            response = am.createAppointmentWebPageResponse(apptResponse);
        } else if(method == "GET" && action == "getschedule") {
            apptResponse = am.getAppointmentsForUser(jsonContent);
            response = am.createAppointmentWebPageResponse(apptResponse);
        } else if(action == "favicon.ico") {
            response = createStandardJSONResponseFromStatusCode(HTTP_NO_CONTENT);
        } else {
            response = createStandardJSONResponseFromStatusCode(HTTP_BAD_REQUEST);
        }
        sendMessage(new_socket, response);
    }
    return 0;
}