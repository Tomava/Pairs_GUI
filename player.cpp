/* Class: Player
 * -----------
 * Represents a player in the game.
 * */
#include "player.hh"
#include <iostream>

Player::Player(QString name):
    name_(name)
{

}

void Player::add_card(Card* card)
{
    add_points(1);
    list_of_cards.push_back(card);
}

int Player::get_points() const
{
    return points_;
}

void Player::add_points(int points)
{
    points_ += points;
}

QString Player::get_name()
{
    return name_;
}

QString Player::get_cards()
{
    // Constructs a string from player's cards
    QString card_string;
    for (Card* card: list_of_cards) {
        // Adds two of each character as technically there are two of each card
        // Do it in two lines as doing arithmetics would change the char in question
        card_string += card->get_element() + " ";
    }
    return card_string.trimmed();
}

void Player::add_games(int games)
{
    games_ += games;
}

void Player::add_wins(int wins)
{
    wins_ += wins;
}

int Player::get_games()
{
    return games_;
}

int Player::get_wins()
{
    return wins_;
}
