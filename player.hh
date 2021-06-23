/* Class: Player
 * -----------
 * Represents a player in the game.
 * */
#ifndef PLAYER_HH
#define PLAYER_HH
#include <string>
#include <vector>
#include <QString>
#include "card.hh"


class Player
{
public:
    // Name is given when constructing
    Player(QString name);
    // Adds a card to the player
    void add_card(Card* card);
    // Obvious getters and setters
    int get_points() const;
    void add_points(int points);
    QString get_name();
    QString get_cards();
    void add_games(int games);
    void add_wins(int wins);
    int get_games();
    int get_wins();

private:
    // Player name
    QString name_;
    int points_ = 0;
    int games_ = 0;
    int wins_ = 0;
    // List of cards player has earned
    std::vector<Card*> list_of_cards;
};

#endif // PLAYER_HH
