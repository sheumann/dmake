echo off
cls
rem  *** This is the make command file that is used under OS/2 to make the
rem  *** first version of dmake.  It isn't pretty but it does work, assuming
rem  *** the compilers have been correctly setup.
rem
echo Running make.cmd script to make a %1 copy of dmake.

if %0%1 == %0 goto error
if %1 == msc40 goto mkmsc40
if %1 == msc50 goto mkmsc50
if %1 == msc51 goto mkmsc51
if %1 == msc60 goto mkmsc60
if %1 == ibm   goto mkibm

rem label the possible DOS variations for dmake here.
:error
echo OS/2 INDEX:  You must specify one of:
echo ------------------
echo    msc40    - Microsoft C 4.0 compile.
echo    msc50    - Microsoft C 5.0 compile.
echo    msc51    - Microsoft C 5.1 compile.
echo    msc60    - Microsoft C 6.0 compile.
3cho    ibm      - IBM C 2.0 compile.
goto end

rem This is the script that makes dmake using Microsoft C 4.0
:mkmsc40
os2\mscdos\mk40.cmd
goto end

rem This is the script that makes dmake using Microsoft C 5.0
:mkmsc50
os2\mscdos\mk50.cmd
goto end

rem This is the script that makes dmake using Microsoft C 5.1
:mkmsc51
os2\mscdos\mk51.cmd
goto end

rem This is the script that makes dmake using Microsoft C 6.0
:mkmsc60
os2\mscdos\mk60.cmd
goto end

rem This is the script that makes dmake using Microsoft C 6.0
:ibm
os2\ibm\mkc2.cmd
goto end

rem All done!
:end
