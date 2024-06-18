# webc

<!--toc:start-->
- [webc](#webc)
  - [Usage](#usage)
    - [Build the library for Linux](#build-the-library-for-linux)
    - [Link the library to your project](#link-the-library-to-your-project)
  - [Example](#example)
  - [Documentation](#documentation)
  - [LICENSE](#license)
<!--toc:end-->

Write websites using the far superior (and actual programming language) C. 


## Usage

### Build the library for Linux

```console
git clone https://github.com/KDesp73/webc
cd webc
make
```

### Link the library to your project

Add `-Lpath/to/library -lwebc` to your LDFLAGS

Make sure run `export LD_LIBRARY_PATH=D_LIBRARY_PATH:/lib/webc` before compiling


## Example

See [Examples.md](./docs/Examples.md)

## Documentation

See [webc.h](./include/webc.h)

## LICENSE

[MIT](./LICENSE)
