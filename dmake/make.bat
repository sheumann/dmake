echo off
cls
rem  *** This is the make batchfile that is used under MSDOS to make the
rem  *** first version of dmake.  It isn't pretty but it does work, assuming
rem  *** the compilers have been correctly setup.  See the warning below
rem  *** concerning tlink, if you are making the Turbo C version.
rem

if %0%1 == %0 goto error
if %1 == tcc goto mktcc
if %1 == tccswp goto mktccswp
if %1 == bcc goto mkbcc
if %1 == bccswp goto mkbccswp
if %1 == msc40 goto mkmsc40
if %1 == msc40swp goto mkmsc40swp
if %1 == msc50 goto mkmsc50
if %1 == msc50swp goto mkmsc50swp
if %1 == msc51 goto mkmsc51
if %1 == msc51swp goto mkmsc51swp
if %1 == msc60 goto mkmsc60
if %1 == msc60swp goto mkmsc60swp
if %1 == ztc goto mkztc
if %1 == ztcswp goto mkztcswp

rem label the possible DOS variations for dmake here.
:error
echo MSDOS:  You must specify 'make target' where target is one of:
echo -------------
echo    tcc       - Turbo C 2.0 compile.
echo    tccswp    - Turbo C 2.0 compile of swapping dmake.
echo    bcc       - Borland C++ 2.0,3.0 compile.
echo    bccswp    - Borland C++ 2.0,3.0 compile of swapping dmake.
echo    msc40     - Microsoft C 4.0 compile.
echo    msc50     - Microsoft C 5.0 compile.
echo    msc51     - Microsoft C 5.1 compile.
echo    msc60     - Microsoft C 6.0 compile.
echo    msc40swp  - Microsoft C 4.0, MASM 5.1 compile of swapping dmake.
echo    msc50swp  - Microsoft C 5.0, MASM 5.1 compile of swapping dmake.
echo    msc51swp  - Microsoft C 5.1, MASM 5.1 compile of swapping dmake.
echo    msc60swp  - Microsoft C 6.0, MASM 5.1 compile of swapping dmake.
echo    ztc       - Zortech C++ 2.1 compile.
echo    ztcswp    - Zortech C++ 2.1 compile of swapping dmake.
goto end

rem This is the script that makes dmake using Microsoft C 4.0
:mkmsc40
msdos\mscdos\mk40.bat
goto end

:mkmsc40swp
msdos\mscdos\mk40swp.bat
goto end

rem This is the script that makes dmake using Microsoft C 5.0
:mkmsc50
msdos\mscdos\mk50.bat
goto end

:mkmsc50swp
msdos\mscdos\mk50swp.bat
goto end

rem This is the script that makes dmake using Microsoft C 5.1
:mkmsc51
msdos\mscdos\mk51.bat
goto end

:mkmsc51swp
msdos\mscdos\mk51swp.bat
goto end

rem This is the script that makes dmake using Microsoft C 6.0
:mkmsc60
msdos\mscdos\mk60.bat
goto end

:mkmsc60swp
msdos\mscdos\mk60swp.bat
goto end

rem This is the script that makes dmake using Zortech C++ 2.1
:mkztc
msdos\ztcdos\mk.bat
goto end

:mkztcswp
msdos\ztcdos\mkswp.bat
goto end

rem This is the script that makes dmake using Turbo C 2.0 or higher.
:mktcc
cls
echo WARNING:
echo    The default response files:
echo        msdos\tccdos\obj.rsp
echo        msdos\tccdos\lib.rsp
echo    contain absolute paths to TURBO-C runtime startup objects, and to
echo    the standard libraries.  You should check that these files contain
echo    the correct path names for your installation of Turbo-C before
echo    proceeding further.  Also check that the mkdir command at the start
echo    of the response file and the copy command at the end of the response
echo    file will work on your system.
echo --
echo Continue if ok, or abort and edit the response files.
pause
msdos\tccdos\mk.bat
goto end

:mktccswp
cls
echo WARNING:
echo    The default response files:
echo        msdos\tccdos\objswp.rsp
echo        msdos\tccdos\libswp.rsp
echo    contain absolute paths to TURBO-C runtime startup objects, and to
echo    the standard libraries.  You should check that these files contain
echo    the correct path names for your installation of Turbo-C before
echo    proceeding further.  Also check that the mkdir command at the start
echo    of the response file and the copy command at the end of the response
echo    file will work on your system.
echo --
echo Continue if ok, or abort and edit the response files.
pause
msdos\tccdos\mkswp.bat
goto end

rem This is the script that makes dmake using Turbo C++ 2.0.
:mkbcc
cls
echo WARNING:
echo    The default response files:
echo        msdos\bccdos\obj.rsp
echo        msdos\bccdos\lib.rsp
echo    contain absolute paths to Borland C++ runtime startup objects, and to
echo    the standard libraries.  You should check that these files contain
echo    the correct path names for your installation of Borland C++ before
echo    proceeding further.  Also check that the mkdir command at the start
echo    of the response file and the copy command at the end of the response
echo    file will work on your system.
echo --
echo Continue if ok, or abort and edit the response files.
pause
msdos\bccdos\mk.bat
goto end

:mkbccswp
cls
echo WARNING:
echo    The default response files:
echo        msdos\bccdos\objswp.rsp
echo        msdos\bccdos\libswp.rsp
echo    contain absolute paths to Borland C++ runtime startup objects, and to
echo    the standard libraries.  You should check that these files contain
echo    the correct path names for your installation of Borland C++ before
echo    proceeding further.  Also check that the mkdir command at the start
echo    of the response file and the copy command at the end of the response
echo    file will work on your system.
echo --
echo Continue if ok, or abort and edit the response files.
pause
msdos\bccdos\mkswp.bat
goto end

rem All done!
:end
