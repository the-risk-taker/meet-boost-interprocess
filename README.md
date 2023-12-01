# Meet Boost.Interprocess

Examples of Boost.Interprocess usage created for an in-company lightning talk.

## Requirements

Install Boost. On [Windows](https://www.boost.org/doc/libs/1_83_0/more/getting_started/windows.html) or [Unix](https://www.boost.org/doc/libs/1_83_0/more/getting_started/unix-variants.html).

## Setup and build

```shell
cmake -S . -B build -G Ninja
ninja -C build
```

## Run examples

There are a few examples. To run example, use the following command:

```shell
ninja -C build run-shared-memory
```
