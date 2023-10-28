@echo off

if "%~1" neq "" (
	set RepositoryDir=%~1
) else (
    set RepositoryDir=C:\Projects\Nand2Tetris
)

echo "RepositoryDir: " %RepositoryDir%


echo "Tests for project 10: "
call %RepositoryDir%\projects\10\RunTests.bat || goto :error
echo "Tests for project 10 passed"


:error
exit /b %errorlevel%