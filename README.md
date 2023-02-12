# GeoConverter

This program takes a list of `Degree, Minites, Seconds` coordinates and returns `Decimal Degree` conversion result. The source code in this repo is my first attemt to write software in `C`.

List of dependencies : 

- String parsing library : [String_View](https://github.com/tsoding/sv)
- Build tool : [nobuild](https://github.com/tsoding/nobuild)

## How to use ?

```console
$ gcc -o nobuild nobuild.c
$ ./nobuild
$ ./converter /path/to/data.txt
```
