echo "RepositoryDir check"
if not defined RepositoryDir (
	set RepositoryDir=C:\Projects\Nand2Tetris
)

echo "Running %RepositoryDir%\projects\10\RunTests.bat"
%RepositoryDir%\projects\10\RunTests.bat