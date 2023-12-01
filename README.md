# Meet Boost.Interprocess

Some examples of Boost.Interprocess usage made for in company lighting talk.

## Requirements

Install Boost. On [Windows](https://www.boost.org/doc/libs/1_83_0/more/getting_started/windows.html) or [Unix](https://www.boost.org/doc/libs/1_83_0/more/getting_started/unix-variants.html).

## Setup and build

```shell
cmake -S . -B build -G Ninja
ninja -C build
```

## Run examples

```shell
ninja -C build run-<example_name>
```
