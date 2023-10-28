echo "One:" %RepositoryDir%

if "%~1" neq "" (
	set RepositoryDir=%~1
) else (
    set RepositoryDir=C:\Projects\Nand2Tetris
)

echo "Two:" %RepositoryDir%

%RepositoryDir%\projects\10\RunTests.bat