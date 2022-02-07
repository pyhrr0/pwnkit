# CVE-2021-4034
## Introduction

This is an exploit for the `CVE-2021-4034` vulnerability, aka [pwnkit](https://www.qualys.com/2022/01/25/cve-2021-4034/pwnkit.txt), which was discovered by Qualys.

## Usage
### Compilation
```
$ make
```
or
```
$ gcc -o pwnkit.so -fPIC -shared pwnkit.c -Wl,-e,main
```

### Testing
```
$ make test
```
or
```
$ ./pwnkit.so
```
