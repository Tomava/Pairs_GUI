/* Class: Card
 * -----------
 * Represents a card in the game.
 * */
#ifndef CARD_HH
#define CARD_HH
#include <QPushButton>

class Card
{
public:
    Card(QPushButton* button, int card_height, int card_width);
    ~Card();
    // Turning a card returns the character of the card
    QString turn();
    // Disables card's button
    void disable();
    // Removes card from game
    void remove();
    // Unremoves, enables and unreveals card
    void revive();
    // Obvious setters and getters
    bool is_revealed();
    void set_element(QString c);
    QString get_element();

private:
    // Each card has a button
    QPushButton* button_;
    // The character that this card has
    QString element_;
    // Is the card revealed
    bool is_revealed_ = false;
    // Path to image used on the back of the card
    const QString CARD_BACK_IMAGE = ":/icons/card_back_fade.svg";
    QPixmap scaled_back_;
    QPixmap scaled_front_;
    int card_height_;
    int card_width_;
};

#endif // CARD_HH
