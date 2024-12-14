#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <Arduino.h>
#include <string.h>

#define INFO(str)  Serial.println("INFO: " + (String)str);
#define WARN(str)  Serial.println("WARN: " + (String)str);
#define ERROR(str) Serial.println("ERROR: " + (String)str);

#endif // _LOG_HPP_
