if "%file%"=="" goto ERROR
if "%ftmp%"=="" goto ERROR

cd %1

echo making... %1
echo making... %1>> %file%
echo making... %1>> %file2%

psymake for_menu > %ftmp%
type %ftmp%
type %ftmp% >> %file2%
echo:
echo:>> %file2%

psymake > %ftmp%
type %ftmp%
type %ftmp% >> %file%
echo:
echo:>> %file%

cd %root%

goto EXIT

:ERROR
    echo ERROR

:EXIT
