@echo off

if not defined RepositoryDir (
    set RepositoryDir=C:\Projects\Nand2Tetris
)

set JackAnalyzer=%RepositoryDir%\x64\Release\JackAnalyzer.exe
set TextComparer=%RepositoryDir%\tools\TextComparer.bat

set ArrayTestDir=%RepositoryDir%\projects\10\ArrayTest
set ExpressionLessSquareDir=%RepositoryDir%\projects\10\ExpressionLessSquare
set SquareDir=%RepositoryDir%\projects\10\Square

set Suffix=_New
set TokenSuffix=_NewT


echo. && echo Compiling %ArrayTestDir% || goto :error
%JackAnalyzer% %ArrayTestDir%

echo. && echo Compiling %ExpressionLessSquareDir% || goto :error
%JackAnalyzer% %ExpressionLessSquareDir%

echo. && echo Compiling %SquareDir% || goto :error
%JackAnalyzer% %SquareDir%

echo. && Comparing %ArrayTestDir%\MainT.xml
call %TextComparer% %ArrayTestDir%\MainT.xml %ArrayTestDir%\Main%TokenSuffix%.xml || goto :error

echo. && echo Comparing %ArrayTestDir%\Main.xml
call %TextComparer% %ArrayTestDir%\Main.xml %ArrayTestDir%\Main%Suffix%.xml || goto :error

echo. && echo Comparing %ExpressionLessSquareDir%\MainT.xml
call %TextComparer% %ExpressionLessSquareDir%\MainT.xml %ExpressionLessSquareDir%\Main%TokenSuffix%.xml || goto :error

echo. && echo Comparing %ExpressionLessSquareDir%\SquareGameT.xml
call %TextComparer% %ExpressionLessSquareDir%\SquareGameT.xml %ExpressionLessSquareDir%\SquareGame%TokenSuffix%.xml || goto :error

echo. && echo Comparing %ExpressionLessSquareDir%\SquareT.xml
call %TextComparer% %ExpressionLessSquareDir%\SquareT.xml %ExpressionLessSquareDir%\Square%TokenSuffix%.xml || goto :error

echo. && echo Comparing %ExpressionLessSquareDir%\Main.xml
call %TextComparer% %ExpressionLessSquareDir%\Main.xml %ExpressionLessSquareDir%\Main%Suffix%.xml || goto :error

echo. && echo Comparing %ExpressionLessSquareDir%\SquareGame.xml
call %TextComparer% %ExpressionLessSquareDir%\SquareGame.xml %ExpressionLessSquareDir%\SquareGame%Suffix%.xml || goto :error

echo. && echo Comparing %ExpressionLessSquareDir%\Square.xml
call %TextComparer% %ExpressionLessSquareDir%\Square.xml %ExpressionLessSquareDir%\Square%Suffix%.xml || goto :error

echo. && echo Comparing %SquareDir%\MainT.xml
call %TextComparer% %SquareDir%\MainT.xml %SquareDir%\Main%TokenSuffix%.xml || goto :error

echo. && echo Comparing %SquareDir%\SquareGameT.xml
call %TextComparer% %SquareDir%\SquareGameT.xml %SquareDir%\SquareGame%TokenSuffix%.xml || goto :error

echo. && echo Comparing %SquareDir%\SquareT.xml
call %TextComparer% %SquareDir%\SquareT.xml %SquareDir%\Square%TokenSuffix%.xml || goto :error

echo. && echo Comparing %SquareDir%\Main.xml
call %TextComparer% %SquareDir%\Main.xml %SquareDir%\Main%Suffix%.xml || goto :error

echo. && echo Comparing %SquareDir%\SquareGame.xml
call %TextComparer% %SquareDir%\SquareGame.xml %SquareDir%\SquareGame%Suffix%.xml || goto :error

echo. && echo Comparing %SquareDir%\Square.xml
call %TextComparer% %SquareDir%\Square.xml %SquareDir%\Square%Suffix%.xml || goto :error


:error
exit /b %errorlevel%