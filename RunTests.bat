echo %RepositoryDir%

if not defined RepositoryDir (
	set RepositoryDir=C:\Projects\Nand2Tetris
)

::%RepositoryDir%\projects\10\RunTests.bat