@echo off
FOR /F "tokens=*" %%G IN ('dir /b Debug\*.exe') DO (
echo Running %%G...
call Debug\%%G > output.txt
)
del output.txt

