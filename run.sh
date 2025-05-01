#! /bin/sh
TIMEFORMAT='Total time taken to run cpp executable : %R seconds.'
# Color variables
red='\033[0;31m'
green='\033[0;32m'
# Clear the color after that
clear='\033[0m'

time {
DIR="./build/"
i=0
if [ -d "$DIR" ]; then
    echo -e "${green}RUN[$[i = i+1]]${clear} : build directory exists"
    echo -e "${green}RUN[$[i = i+1]]${clear} : deleting build directory"
    rm -rf ./build
    echo -e "${green}RUN[$[i = i+1]]${clear} : creating new build directory"
    mkdir build
else
    echo -e "${green}RUN[$[i = i+1]]${clear} : build directory does not exists"
    echo -e "${green}RUN[$[i = i+1]]${clear} : creating directory"
    mkdir build
fi

echo -e "${green}RUN[$[i = i+1]]${clear} : Go to build"
cd build

echo -e "${green}RUN[$[i = i+1]]${clear} : running cmake"
arch -arm64 cmake ..

FILE="./Makefile"
if [ -f "$FILE" ]; then
    echo -e "${green}RUN[$[i = i+1]]${clear} : running make"
    arch -arm64 make -j 4
fi

FILE="./MyExecutable"
if [ -f "$FILE" ]; then
    echo -e "${green}RUN[$[i = i+1]]${clear} : running executable"
    echo -e "${red}--------------------------------------------------------${clear}"
    ./MyExecutable
    echo -e "${red}--------------------------------------------------------${clear}"
    echo -e "${green}RUN[$[i = i+1]]${clear} : ran successfully"
fi
}