# webc `</c>`

```
   d88P        d88P          Y88b          
  d88P        d88P            Y88b         
 d88P        d88P              Y88b        
d88P        d88P     .d8888b    Y88b       
Y88b       d88P     d88P"       d88P       
 Y88b     d88P      888        d88P        
  Y88b   d88P       Y88b.     d88P         
   Y88b d88P         "Y8888P d88P          
```

Write and deploy websites using the C Programming Language.

<!--toc:start-->
- [webc](#webc)
  - [Usage](#usage)
    - [Build the library for Linux](#build-the-library-for-linux)
    - [Link the library to your project](#link-the-library-to-your-project)
  - [Examples](#examples)
  - [Documentation](#documentation)
  - [LICENSE](#license)
<!--toc:end-->


> [!IMPORTANT]
> WARNING: This C library is in the early stages of development and is not yet ready for production use. 
> The API, functionality, and overall stability of the library are subject to change without notice. 
> Use this library at your own risk. We strongly recommend against using this library in any mission-critical or production systems. 
> Please report any issues or feedback to the project maintainers. Further development and stabilization of 
> this library is ongoing, and a stable release will be announced when it is ready.


## Usage

### Build the library for Linux

```console
git clone https://github.com/KDesp73/webc
cd webc
make
```

### Link the library to your project

Add `-Lpath/to/library -lwebc` to your LDFLAGS

Make sure you have the LD_LIBRARY_PATH environment variable set correctly by running `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:path/to/library` before running

## Examples

See [docs/examples/](./docs/examples/core.md)

## Templates

- Single Page Portfolio `spp.h`
  
![image](https://github.com/KDesp73/webc/assets/63654361/60561368-e2fd-42c2-9151-f9dac91cfea2)



## Documentation

The documentation for this library is written in the header files

See [webc-core.h](./include/webc-core.h)

Autogenerated documentation using Doxygen [here](https://kdesp73.github.io/webc-docs/)

## LICENSE

[MIT](./LICENSE)
