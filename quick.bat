@echo off
echo Building project with Makefile...
make

IF %ERRORLEVEL% NEQ 0 (
    echo Build failed. Exiting.
    pause
    exit /b %ERRORLEVEL%
)

echo Build succeeded. Running dict1.exe...
dict2.exe 2 tests/dataset_test.csv output.txt < tests/test.in > output.stdout.out
SET EXITCODE=%ERRORLEVEL%

echo Program exited with code: %EXITCODE%

REM Detect access violation (segfault-like crash)
IF "%EXITCODE%"== "-1073741819" (
    echo ERROR: dict1.exe crashed with an access violation (possible segmentation fault).
    pause
    exit /b %EXITCODE%
)

echo Done