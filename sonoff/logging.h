#ifndef SONOFF_LOGGING_H
#define SONOFF_LOGGING_H

#ifdef DEBUG
#define DEBUG_PRINT(message) Serial.print(message)
#define DEBUG_PRINTLN(message) Serial.println(message)
#define DEBUG_PRINT_VAR(message, value) \
  DEBUG_PRINT(message); \
  DEBUG_PRINTLN(value)
#else
#define DEBUG_PRINT(message)
#define DEBUG_PRINTLN(message)
#define DEBUG_PRINT_VAR(message, value)
#endif

#define INFO_PRINT(message) Serial.print(message)
#define INFO_PRINTLN(message) Serial.println(message)
#define INFO_PRINT_VAR(message, value) \
  INFO_PRINT(message); \
  INFO_PRINTLN(value)

#endif
