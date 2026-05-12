This is a project I am making to create a chess game 
to run it you must run: 

find the ChessGame_Deploy folder, within it there is a chessGUI.exe file. Running this file (chessGUI.exe) will load up the Chess game.


if you want to run it though a terminal, then you will need to run the following:

g++ main.cpp gui.cpp board.cpp movegen.cpp engine.cpp -IC:/SFML-2.6.1/include -LC:/SFML-2.6.1/lib -o ChessGUI -lsfml-graphics -lsfml-window -lsfml-system

./ChessGUI
