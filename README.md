# libbez
Little popen()

A little process execution library to use instead of system() and popen() calls.
The advantage  are:


  * If the invoked command is not responding it can be terminated based on
    the timeout specified as parameter

  * The output from the command can be read using stdio

Currently only the reading mode is supported.

Thanks,
Spago
