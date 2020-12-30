# CppND-System-Monitor

This repo contains the second project of the [C++ Nanodegree](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213) from Udacity.

A system monitor shows you what’s happening on your computer: processes, id, CPU, memory usage, and much more. In this project, we’ll write object-oriented C++ to build a Linux system monitor similar to the widely used [htop](https://en.wikipedia.org/wiki/Htop) application. This is the equivalent of the [Process Explorer](https://docs.microsoft.com/en-us/sysinternals/downloads/process-explorer) or [Process Monitor](https://docs.microsoft.com/en-us/sysinternals/downloads/procmon) if you are a Windows user, or to the [Activity Monitor](https://support.apple.com/guide/activity-monitor/welcome/mac) if you are a Mac user.

The starter code can be found [here](https://github.com/udacity/CppND-System-Monitor-Project-Updated).

## Dependencies for Running Locally

### ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output. Install ncurses within your own Linux environment:</br>
```
sudo apt install libncurses5-dev libncursesw5-dev
```

### Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository:</br>
```
git clone https://github.com/blelevier/CppND-System-Monitor-Project-Updated.git
```

2. Build the project:</br>
```
make build
```

3. Run the resulting executable:</br>
```
./build/monitor
```

## Results
</br>

## Future Work 

* Calculate CPU utilization dynamically, based on recent utilization
* Sort processes based on CPU or memory utilization
* Make the display interactive
* Restructure the program to use abstract classes (interfaces) and pure virtual functions
* Port the program to another operating system
