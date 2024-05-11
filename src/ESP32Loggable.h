#ifndef ESP32_LOGGABLE_H
#define ESP32_LOGGABLE_H

#include <logging.hpp>

using namespace esp32m;

class ESP32Loggable : public Loggable
{
  public:
    ESP32Loggable(const char *name) : _name(name) {
    }

    void debug(const char *format, ...) {
      va_list args;
      va_start(args,format);
      logD(format,args);
      va_end(args);
    }
    void info(const char *format, ...) {
      va_list args;
      va_start(args,format);
      logI(format,args);
      va_end(args);
    }
    void warn(const char *format, ...) {
      va_list args;
      va_start(args,format);
      logW(format,args);
      va_end(args);
    }
    void error(const char *format, ...) {
      va_list args;
      va_start(args,format);
      logE(format,args);
      va_end(args);
    }


  protected:
    virtual const char *logName() const { return _name; };


  private:
    const char *_name;
};


#endif