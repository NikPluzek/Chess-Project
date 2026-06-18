<h1>Chess Engine</h1>

This is a project I made to create a chess engine with a playable GUI, built from scratch using C++ for the engine and SFML for the GUI.

<h3>Features</h3>
<ul>
  <li>Bitboard based board representation</li>
  <li>Alpha-beta pruning with MVV-LVA move ordering</li>
  <li>Quiescence search</li>
  <li>With an estimated playing strength of ~1600 Elo as of 18/06/2026</li>
</ul>  


<h2>How To Run</h2>
to run it you must: 

find the ChessGame_Deploy folder, within it there is a chessGUI.exe file. Running this file (chessGUI.exe) will load up the Chess game.

if you want to run it though a terminal, then you will need to run the following:

g++ main.cpp gui.cpp board.cpp movegen.cpp engine.cpp -IC:/SFML-2.6.1/include -LC:/SFML-2.6.1/lib -o ChessGUI -lsfml-graphics -lsfml-window -lsfml-system
./ChessGUI


<h2>Screenshots</h2>
<h3>Side Selection Screen</h3>
<img width="638" height="641" alt="image" src="https://github.com/user-attachments/assets/ec137773-7e5f-47f8-b09c-880551d27ea0" />

<h3>Starting Positions</h3>
<img width="637" height="637" alt="image" src="https://github.com/user-attachments/assets/e37d7d94-15f1-4e52-84b4-b518f4d70eb2" />

<h3>Move Highlighting Example</h3>
<img width="636" height="638" alt="image" src="https://github.com/user-attachments/assets/5f278dd5-1322-41f7-96a1-ea8351939996" />

