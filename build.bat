::Carefull, it clears build folder
::For testing. This is the way server builds the code

@echo off
if exist build rmdir /q /s build
mkdir build

for /r %%a in (*.cpp) do (
	call set SRC=%%SRC%% "%%a"
)

@echo on

g++ -std=c++1y -static -Isrc -O2 -lm -o build/bob.exe%SRC%