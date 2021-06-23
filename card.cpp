/* Class: Card
 * -----------
 * Represents a card in the game.
 * */
#include "card.hh"

Card::Card(QPushButton* button, int card_height, int card_width):
    button_(button), card_height_(card_height), card_width_(card_width)
{
    QFont font = button_->font();
    font.setPointSize(30);
    button_->setFont(font);
    button_->setStyleSheet(" QPushButton {background-color: lightblue;}");
    QPixmap back(CARD_BACK_IMAGE);
    scaled_back_ = back.scaled(QSize(card_width_,card_height_), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    button_->setIcon(scaled_back_);
    button_->setIconSize(QSize(card_width_,card_height_));
}

Card::~Card()
{
    delete button_;
}

QString Card::turn()
{
    is_revealed_ = !is_revealed_;
    if (is_revealed_) {
//        button_->setText(QString(character_));
        button_->setStyleSheet(" QPushButton { background-color: lightgrey; }");
        button_->setIcon(scaled_front_);
//        button_->setIcon(QIcon());
    } else {
        button_->setText("");
        // Enable button, if it was disabled as the first card in a turn
        button_->setDisabled(false);
        button_->setStyleSheet(" QPushButton { background-color: lightblue; }");
        button_->setIcon(scaled_back_);
    }

    return element_;
}

void Card::disable()
{
    is_revealed_ = true;
    button_->setDisabled(true);
}

void Card::remove()
{
    button_->setDisabled(true);
    button_->setHidden(true);
}

void Card::revive()
{
    // Undisable and unhide button
    button_->setDisabled(false);
    button_->setHidden(false);
    // Turn card if it's revealed
    if (is_revealed_) {
        turn();
    }
}

bool Card::is_revealed()
{
    return is_revealed_;
}

void Card::set_element(QString c)
{
    element_ = c;
    QPixmap front(":/icons/" + element_ + ".svg");
    scaled_front_ = front.scaled(QSize(card_width_,card_height_), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

QString Card::get_element()
{
    return element_;
}
