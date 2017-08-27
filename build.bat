::For testing. This is the way server builds the code

@echo off
if not exist build mkdir build

set SRC= main.cpp
cd src
for /r %%a in (*.cpp) do (
	call set SRC=%%SRC%% "%%a"
)
cd ..

@echo on

g++ -std=c++1y -static -Isrc -O2 -lm -o build/Bobik.exe%SRC%