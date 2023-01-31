# Chess UCSP

C++ player vs player and player vs computer chess game.

## How to build the project

### Download dependencies
We use ncurses library to detect the keyboard input.
```bash
sudo apt install ncurses-dev
```

### Build the project

This is the command I use to build the release build type.
```bash
cmake -S . -B ./build && cmake -DCMAKE_BUILD_TYPE=Release ./build && cd ./build && make -j $(nproc) && ./UCSP_Chess
```


## How to contribute
[check this detailed guide](Contributing.md)
