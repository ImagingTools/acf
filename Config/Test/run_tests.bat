@echo off
if NOT [%1]==[] set format=-o -,%1

for /r %%i in (*Test.exe) do %%i %format%

EXIT /B %ERRORLEVEL%
