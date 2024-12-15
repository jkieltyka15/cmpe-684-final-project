/**
* @brief: Contains macros to simplify logging
* @file: Log.h
*
* @author: jkieltyka15
*/

#ifndef _LOG_H_
#define _LOG_H_

#include <Arduino.h>
#include <string.h>

#define INFO(str)  Serial.println("INFO: " + (String)str);
#define WARN(str)  Serial.println("WARN: " + (String)str);
#define ERROR(str) Serial.println("ERROR: " + (String)str);

#endif // _LOG_H_
