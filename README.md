# Appointment Scheduler

The appointment scheduler allows user's to schedule appointments and get their scheduled appointments.

## Requirements
In order to compile and run this program you need to meet the following requirements: 
1. Operating System - Linux or macOS
2. Compiler: g++ or clang++
3. C++ Version: 17

## Compiling

To compile this program from the source code:
1. Downloaded the file repository from Github.
2. Navigate to the AppointmentScheduler directory in a terminal
3. Run the following from the command line:
For Mac or Linux:
```
g++ -std=c++17 -stdlib=libc++ -g server/src/*.cpp -o server/src/main 
```

For Mac using clang:
```
clang++ -std=c++17 -stdlib=libc++ -g server/src/*.cpp -o server/src/main 
```

## Running the executable

In order to run to run the executable, navigate to AppointmentScheduler/server/src directory in the terminal and run

```
./main
```
From the command line.

## Appointment Scheduler API

The appointment scheduler API provides one GET endpoint to get scheduled appointments and one POST endpoint to schedule appointments.

To hit the API you can use Postman or your favorite HTTP sender.

Here is the GET endpoint and the required JSON parameters:

```
GET localhost:8080/schedule
```

| Field |       Type       | Format | Constraints | Description                      |
| :---- | :--------------: | :----: | ----------- | -------------------------------- |
| uid   | unsigned integer |  N/A   | 0-INT_MAX   | An unsigned integer for the user |

Some notes:

Any excess fields are ignored.

Here is the POST endpoint and the required JSON parameters:

```
POST localhost:8080/schedule
```

| Field |    Type    | Format | Constraints | Description |
| :----------------------------- | :----------: | :-----------: | ------------------------------ | ------------------------------ |
| uid                            |  unsigned integer  | N/A | 0-INT_MAX | An unsigned integer for the user |
| date                           | string | YYYY-MM-DD | Y: 2000-2999 M: 1-12 D: 1-28/30/31 (depending on month) | Date of the appointment |
| time | string | HH:MM | H: 0-23 M: 0, 30 | Time of the appointment |

Some notes:

A real time library was not used, so February 29th is not a valid day.

Appointments can only be made on the hour or half hour.

Any excess fields are ignored.

## Error Handling

All invalid requests will receive a Bad Request response.

For a favicon.ico request I return a No Content response

| HTTP Response |    Code    | Message |
| :----------------------------- | :----------: | :-----------: |
| 400 Bad Request | 10 | "Unrecognized request" |
| 204 No Content | 20 | "There is no content" |

For an OK request but invalid input for the fields, an appropriate message is returned to help the user enter the information in the correct format.


## Future Considerations
1. Use an actual time library to better handle time constraints. Currently you can't schedule anything for February 29 because I kept the time keeping as simple as possbile.
2. User authentication
3. Handle multiple incoming requests at a time


## Acknowledgements

I borrowed code from the following tutorial to set up the server:
https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa

And code from here to parse some of the incoming message:
https://stackoverflow.com/questions/28268236/parse-http-request-without-using-regexp

And thanks to the Tencent for the rapidjson library