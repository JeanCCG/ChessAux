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
This is the command I use to build the release build type.
```bash
cmake -S . -B ./build && cmake -DCMAKE_BUILD_TYPE=Debug ./build && cd ./build && make -j $(nproc) && ./UCSP_Chess
## OR, build like any other project, it builds in Debug by default
cmake -S . -B ./build && cmake --build ./build && cd ./build && make -j $(nproc) && ./UCSP_Chess
```

some aliases that might improve your life quality. Add these in your `.bashrc` or `.zshrc`
```bash
alias gbuild='cd ./build || cd ../build || cd ../../build'
alias cmors='rm -rf build && cmake -S . -B ./build && cmake --build ./build && gbuild && make -j $(nproc)'
alias cmors_debug='rm -rf build && cmake -S . -B ./build && cmake -DCMAKE_BUILD_TYPE=Debug ./build && gbuild && make -j $(nproc)'
alias cmors_release='rm -rf build && cmake -S . -B ./build && cmake -DCMAKE_BUILD_TYPE=Release ./build && gbuild all && make -j $(nproc)'
```

if you added the aliases to your profile you can build the project as follows:
```bash
cmors && ./UCSP_Chess # to default debug built type
cmors_debug && ./UCSP_Chess # to build the debug built type
cmors_release && ./UCSP_Chess # to build the release built type
```

     <!-- Stalemate - if not in check but no legal moves
    Correct threefold repetition claim
    Correct fifty-move rule claim
    Insufficient material
    Draw offer accepted -->
