#ifndef __DEBUG__THING__
#define __DEBUG__THING__

#define DEBUG

#ifdef DEBUG
#define Serial_print(...) do { Serial.print(__VA_ARGS__); } while (0)
#define Serial_println(...) do { Serial.println(__VA_ARGS__); } while (0)
#else
#define Serial_print(...) do {} while (0)
#define Serial_println(...) do {} while (0)
#endif // END DEBUG

#endif
