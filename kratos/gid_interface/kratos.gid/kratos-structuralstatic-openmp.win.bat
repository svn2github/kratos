REM @ECHO OFF
REM Identification for arguments
REM basename                          = %1
REM Project directory                 = %2
REM Problem directory                 = %3
 
REM OutputFile: "%2\%1.info"
REM ErrorFile: "%2\%1.err"
 
DEL "%2\%1.info"
DEL "%2\%1.err"

REM Set the number of threads for OpenMP
REM export OMP_NUM_THREADS=%5
set OMP_NUM_THREADS=%5

REM Run Python using the script KratosOpenMPStructuralStatic.py
%3\\kratos\\runkratos.exe KratosOpenMPStructuralStatic.py > "%2\%1.info" 2> "%2\%1.err"
