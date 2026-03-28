# Protoforge
Protoforge is a prototype of wordforge, a program designed to make revising for English easier and more fun! Protoforge is the minimum viable product of wordforge, please use wordforge for a smoother and more stable experience.

> [!WARNING]
> Protoforge will not be actively maintained. Please use the wordforge GUI app.

## Compatibility
Protoforge is not compatible with Windows. If you wish to use protoforge on Windows, please use WSL. Protoforge supports any POSIX system (MacOS, Linux, BSD, etc).

## Build Instructions
Protoforge uses the CMake build system.

```bash
mkdir build
cd build
cmake ..
make
```

## Home Directory
Protoforge will require a home directory to run. Use the included `wordforge.zip` file to create one.

```bash
mv wordforge.zip ~
unzip ~/wordforge.zip -d ~/wordforge
```