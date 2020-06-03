# libbez
Little popen()

A little process execution library to use instead of system() and popen() calls.
The advantages  are:


  * If the invoked command is not responding it can be terminated based on
    the timeout specified as parameter

  * The output from the command can be read using stdio

Currently only the reading mode is supported.

Examples.

  * Run a simple command like ls
```
  $ ./runme ls
librun.c
librun.h
librun.o
Makefile
README.md
runme
runme.c
runme.o
```

  * Time out if run more than 10 secods, that's the default timeout

```
$ ./runme sleep 15
popen_read(): Timer expired
```

Thanks,
Spago
