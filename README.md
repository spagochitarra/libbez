# libbez
libc system() with timeout

A little process execution library to use instead of system()

The advantages is :

  * If the invoked command is not responding it can be terminated based on
    the timeout specified as parameter

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

  * Time out if run more than 10 seconds, that's the default timeout

```
$ ./runme sleep 15
$ system_timeout: Timer expired
echo $?
255
```

Thanks,
Spago
