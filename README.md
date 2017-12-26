# adb-bufferSystem

adb-bufferSystem is a C++-written demo system for adb lesson in Fall 2017-2018 semester, which can simply show how the storage & buffer control part in a database works

## System Requirement

You should install CMake with the version at least 3.9

You should install a reasonable C++11-supported compiler such as clang, g++-5 and so on

## Config

There are some parameters which can change the way of what this program running. You can find and modify them in the src/config.h

## Installation (For Unix-Like OS)

1. Clone this project to your local machine and move to the root directory of this project

```
cd [path]/[to]/adb-BufferSystem
```

2. Create a build directory

```
mkdir build

cd build
```

3. Execute cmake

```
cmake ..
```

4. Compile this project

```
make
```

After that, two executables will be generated, one called "IOCount", and the other called "dataGen"

## Usage

Before you run the main program "IOCount", you should first create the storage file called "data.dbf" by executing "dataGen"

After that, you can execute "IOCount" for the main experiment

## Parts finished

Data Storage Manager part

Buffer Manager part

## Todo List

No idea
