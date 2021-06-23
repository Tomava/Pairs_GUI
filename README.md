# Pairs_GUI
 Pairs game made with C++.

### Updating cards (requires python)
To update card colours run **RedoSVG.py**.

### Instructions
This is a card game where the goal is to get a pair of 2 cards. Players take turn in the game.
The player who's currently in turn is shown on top left corner. Players can be added and removed by
using the rightmost textbrowser. Each new line is a player and the maximum number of characters in
a name is 16 to fit the ui. Players names, scores and collected cards are shown in the left panel
(current player is shown in green). Automatic scrolling of this panel can be enabled so that it tracks
the current player. The middle panel shows all time score and win ratio of players who have played the game.
It's only updated on launch and after each game ends. The game size can be chosen from the combobox's dropdown
menu which shows sizes in "columns x rows" or by using S and W keys. The game can be restarted at any time
using the "restart" button or by pressing "delete" key (restarting also applies the chosen game size).

The game is played by revealing 2 cards. After this the player clicks on "Change turn" button or presses
C, CTRL or Enter key. If the 2 cards have the same element in them, the player in turn gets another turn.
If cards were not the same, it's the next player's turn. When all cards are removed from the game, the player
with the most points win. If there are multiple players with the same score, it's a draw.
