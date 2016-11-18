#ifndef SERIAL_H_
#define SERIAL_H_

#include <windows.h>

class serial{
      public:
             serial(void);
             void begin(char comport[], int _baud);
             void close(void);
             void write(char data);
             void send(char data[]);
      private:
              int baud;
              int error;
              HANDLE serialHandle;
};

#endif /* SERIAL_H_ */
