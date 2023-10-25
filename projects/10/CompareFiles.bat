@echo off
set RepositoryDir=C:\Projects\Nand2Tetris

set JackAnalyzer=%RepositoryDir%\x64\Debug\JackAnalyzer.exe
set TextComparer=%RepositoryDir%\tools\TextComparer.bat

set ArrayTestDir=%RepositoryDir%\projects\10\ArrayTest
set ExpressionLessSquareDir=%RepositoryDir%\projects\10\ExpressionLessSquare
set SquareDir=%RepositoryDir%\projects\10\Square

set Suffix=_VS
set TokenSuffix=_VST

@echo on
%JackAnalyzer% %ArrayTestDir% &&^
%JackAnalyzer% %ExpressionLessSquareDir% &&^
%JackAnalyzer% %SquareDir% &&^
%TextComparer% %ArrayTestDir%\MainT.xml %ArrayTestDir%\Main%TokenSuffix%.xml &&^
%TextComparer% %ArrayTestDir%\Main.xml %ArrayTestDir%\Main%Suffix%.xml &&^
%TextComparer% %ExpressionLessSquareDir%\MainT.xml %ExpressionLessSquareDir%\Main%TokenSuffix%.xml &&^
%TextComparer% %ExpressionLessSquareDir%\SquareGameT.xml %ExpressionLessSquareDir%\SquareGame%TokenSuffix%.xml &&^
%TextComparer% %ExpressionLessSquareDir%\SquareT.xml %ExpressionLessSquareDir%\Square%TokenSuffix%.xml &&^
%TextComparer% %ExpressionLessSquareDir%\Main.xml %ExpressionLessSquareDir%\Main%Suffix%.xml &&^
%TextComparer% %ExpressionLessSquareDir%\SquareGame.xml %ExpressionLessSquareDir%\SquareGame%Suffix%.xml &&^
%TextComparer% %ExpressionLessSquareDir%\Square.xml %ExpressionLessSquareDir%\Square%Suffix%.xml &&^
%TextComparer% %SquareDir%\MainT.xml %SquareDir%\Main%TokenSuffix%.xml &&^
%TextComparer% %SquareDir%\SquareGameT.xml %SquareDir%\SquareGame%TokenSuffix%.xml &&^
%TextComparer% %SquareDir%\SquareT.xml %SquareDir%\Square%TokenSuffix%.xml &&^
%TextComparer% %SquareDir%\Main.xml %SquareDir%\Main%Suffix%.xml &&^
%TextComparer% %SquareDir%\SquareGame.xml %SquareDir%\SquareGame%Suffix%.xml &&^
%TextComparer% %SquareDir%\Square.xml %SquareDir%\Square%Suffix%.xml
