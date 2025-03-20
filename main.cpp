#include <iostream>
#include <string>

#include "ntp_client.h"

#pragma warning(disable : 4996)  // disable CRT secure warnings

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#define IP "10.96.16.5"  // NTP server ip
// #define IP "195.113.144.201"  // NTP server ip

using namespace CTU::VIN::NTP_client;

BOOL WINAPI ConsoleHandler(DWORD dwCtrlType);  // console ctrl handler prototype

HNTP client;  // NTP client object opaque pointer

int main(int argc, char** argv)  // C style example
{
  enum Status s;                    // status enum as a result from query
  struct Result* result = nullptr;  // result struct with all data
  char status_str[100];             // status string from enum container
  char result_str[400];             // result print info char container
  char ip[20];                      // IP char container

  if (argc > 1)                // if there exists argument,
    strncpy(ip, argv[1], 20);  // copy it into IP array
  else
    strcpy(ip, IP);  // otherwise use predefined IP value

  SetConsoleCtrlHandler(ConsoleHandler,
                        true);  // assign console window onclose event
  client = Client__create();    // finally create NTP client

  try {
    s = Client__query_and_sync(client, ip, &result);  // main NTP server query

    Client__get_status_str(s, status_str);  // convert status enum to str
    // printf("\nStatus: %s\n", status_str);   // print status string

    if (s == Status::OK) {                          // if status ok, print info
      Client__format_info_str(result, result_str);  // format info into string
      Client__free_result(result);                  // delete dynamic object
      auto msg = std::string("OK -> ") + std::string(ip);
      MessageBoxA(NULL, result_str, msg.c_str(), MB_OK);
      return 0;
    } else {                        // if status not ok, quit
      Client__free_result(result);  // delete dynamic object
      auto msg = std::string("NG -> ") + std::string(ip);
      MessageBoxA(NULL, "Failed to update.", msg.c_str(), MB_OK | MB_ICONERROR);
      return -1;
    }
  } catch (const std::exception& exc)  // catch any exception
  {
    auto msg = std::string("NG -> ") + std::string(ip);
    MessageBoxA(NULL, exc.what(), msg.c_str(), MB_OK);
    return -1;
  }
}

BOOL WINAPI ConsoleHandler(DWORD dwCtrlType)  // console ctrl event handler
{
  if (CTRL_CLOSE_EVENT == dwCtrlType)  // close window event fired
  {
    if (client != NULL)  // if client exists,
    {
      Client__close(client);  // close / clean-up
    }
  }
  return TRUE;
}
