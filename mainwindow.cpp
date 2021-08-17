/* Pairs GUI
 *
 * Desc:
 * In the instructions.txt
 * */
#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <iostream>
#include <algorithm>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer_->setInterval(1000);
    randomEng_.seed(time(0));
    // Make UI components
    current_player_name_ = new QLabel("Player 1", this);
    current_player_name_->setStyleSheet("QLabel { color : #007F0E;}");
    winner_player_name_ = new QLabel("", this);
    timer_label_ = new QLabel("00:00", this);
    restart_button_ = new QPushButton("Restart", this);
    turn_button_ = new QPushButton("Change turn", this);
    turn_button_->setStyleSheet(" QPushButton {background-color: #2DED63;}");
    restart_button_->setStyleSheet(" QPushButton {background-color: #EA2A43;}");
    player_names_ = new QTextEdit(this);
    player_names_instructions_ = new QLabel("Set player names.\nEach new line is a player.\n(max " + QString::number(MAX_PLAYER_NAME_LENGTH) + " chars per name)", this);
    player_names_instructions_->setAlignment(Qt::AlignCenter);
    score_board_ = new QTextBrowser(this);
    score_board_instructions_ = new QLabel("Scoreboard", this);
    score_board_instructions_->setAlignment(Qt::AlignCenter);
    all_time_score_board_ = new QTextBrowser(this);
    all_time_score_board_instructions_ = new QLabel("All time scoreboard", this);
    all_time_score_board_instructions_->setAlignment(Qt::AlignCenter);
    game_size_selector_ = new QComboBox(this);
    auto_scroll_ = new QCheckBox(this);
    auto_scroll_label_ = new QLabel("Enable autoscroll", this);
    // Get the font from any component
    QFont font = current_player_name_->font();
    // Set font size for all components
    font.setPointSize(FONT_SIZE);
    for (auto component: QApplication::allWidgets()) {
        component->setFont(font);
    }
    set_game_sizes();
    player_names_->setPlainText("Player 1\nPlayer 2\nPlayer 3");
    // Connect timer to timeout
    connect(timer_, &QTimer::timeout, this, &MainWindow::on_timeout);
    // Connect buttons to functions
    connect(restart_button_, &QPushButton::clicked, this, &MainWindow::restart);
    connect(turn_button_, &QPushButton::clicked, this, &MainWindow::change_turn);
    read_all_time_players();
    restart();
    // Set the size of the window
    this->resize(1200,700);
}

MainWindow::~MainWindow()
{
    delete ui;
    remove_cards();
    remove_players();
    for (Player* player: list_of_all_time_players_) {
        delete player;
    }
    for (auto component: QApplication::allWidgets()) {
        delete component;
    }
    delete timer_;
}

void MainWindow::restart()
{
    // Get game dimensions
    QString game_size = game_size_selector_->currentText();
    column_amount_ = game_size.split("x").at(0).toInt();
    row_amount_ = game_size.split("x").at(1).toInt();
    list_of_cards_left_.clear();
    if (cards_.size() != 0) {
        remove_cards();
    }
    init_cards();
    remove_players();
    create_players();
    player_in_turn_ = list_of_players_.at(0);
    elapsed_seconds_ = 0;
    turn_number_ = 0;
    current_turn_cards_ = {nullptr, nullptr};
    set_geometry();
    turn_button_->setDisabled(true);
    winner_player_name_->setHidden(true);
    current_player_name_->setHidden(false);
    set_times();
    set_all_time_player_scores();
    refresh_ui();
    timer_->start();
}

void MainWindow::set_geometry()
{
    // Set all components geometry
    // (x, y, x-size, y-size)
    score_board_->setGeometry(2 * BORDER_MARGIN + max_column_amount_ * (DEFAULT_BUTTON_WIDTH) + MARGIN,
                              2 * BORDER_MARGIN + DEFAULT_BUTTON_HEIGTH,
                              2 * DEFAULT_NAME_TEXT_WIDTH,
                              max_row_amount_ * DEFAULT_BUTTON_HEIGTH);
    score_board_instructions_->setGeometry(2 * BORDER_MARGIN + max_column_amount_ * (DEFAULT_BUTTON_WIDTH) + MARGIN,
                                           0 - MARGIN,
                                           2 * DEFAULT_NAME_TEXT_WIDTH,
                                           3 * (DEFAULT_BUTTON_HEIGTH));
    all_time_score_board_->setGeometry(2 * BORDER_MARGIN + max_column_amount_ * (DEFAULT_BUTTON_WIDTH) + 2 * DEFAULT_NAME_TEXT_WIDTH + 2 * MARGIN,
                                       2 * BORDER_MARGIN + DEFAULT_BUTTON_HEIGTH,
                                       2 * DEFAULT_NAME_TEXT_WIDTH,
                                       max_row_amount_ * DEFAULT_BUTTON_HEIGTH);
    all_time_score_board_instructions_->setGeometry(2 * BORDER_MARGIN + max_column_amount_ * (DEFAULT_BUTTON_WIDTH) + 2 * DEFAULT_NAME_TEXT_WIDTH + 2 * MARGIN,
                                                    0 - MARGIN,
                                                    2 * DEFAULT_NAME_TEXT_WIDTH,
                                                    3 * (DEFAULT_BUTTON_HEIGTH));
    current_player_name_->setGeometry(2 * BORDER_MARGIN,
                                     2 * BORDER_MARGIN,
                                     max_column_amount_ * (DEFAULT_BUTTON_WIDTH),
                                     DEFAULT_BUTTON_HEIGTH);
    winner_player_name_->setGeometry(2 * BORDER_MARGIN,
                                    2 * BORDER_MARGIN,
                                    max_column_amount_ * (DEFAULT_BUTTON_WIDTH),
                                    DEFAULT_BUTTON_HEIGTH);
    timer_label_->setGeometry(2 * BORDER_MARGIN,
                             2 * BORDER_MARGIN + (max_row_amount_ + 1) * (DEFAULT_BUTTON_HEIGTH),
                             DEFAULT_NAME_TEXT_WIDTH,
                             DEFAULT_BUTTON_HEIGTH);
    restart_button_->setGeometry(2 * BORDER_MARGIN + DEFAULT_NAME_TEXT_WIDTH + MARGIN,
                                2 * BORDER_MARGIN + (max_row_amount_ + 2) * (DEFAULT_BUTTON_HEIGTH),
                                DEFAULT_NAME_TEXT_WIDTH,
                                DEFAULT_BUTTON_HEIGTH);
    turn_button_->setGeometry(2 * BORDER_MARGIN + 2 * (DEFAULT_NAME_TEXT_WIDTH + MARGIN),
                             2 * BORDER_MARGIN + (max_row_amount_ + 2) * (DEFAULT_BUTTON_HEIGTH),
                             DEFAULT_NAME_TEXT_WIDTH,
                             DEFAULT_BUTTON_HEIGTH);
    player_names_->setGeometry(2 * BORDER_MARGIN + max_column_amount_ * (DEFAULT_BUTTON_WIDTH) + 4 * DEFAULT_NAME_TEXT_WIDTH + 3 * MARGIN,
                              2 * BORDER_MARGIN + DEFAULT_BUTTON_HEIGTH,
                              2 * DEFAULT_NAME_TEXT_WIDTH,
                             max_row_amount_ * DEFAULT_BUTTON_HEIGTH);
    player_names_instructions_->setGeometry(2 * BORDER_MARGIN + max_column_amount_ * (DEFAULT_BUTTON_WIDTH) + 4 * DEFAULT_NAME_TEXT_WIDTH  + 3 * MARGIN,
                                           0 - MARGIN,
                                           2 * DEFAULT_NAME_TEXT_WIDTH,
                                           3 * (DEFAULT_BUTTON_HEIGTH));
    game_size_selector_->setGeometry(2 * BORDER_MARGIN,
                                     2 * BORDER_MARGIN + (max_row_amount_ + 2) * (DEFAULT_BUTTON_HEIGTH),
                                     DEFAULT_NAME_TEXT_WIDTH,
                                     DEFAULT_BUTTON_HEIGTH);
    auto_scroll_->setGeometry(2 * BORDER_MARGIN + max_column_amount_ * (DEFAULT_BUTTON_WIDTH) + MARGIN,
                              2 * BORDER_MARGIN + DEFAULT_BUTTON_HEIGTH + max_row_amount_ * DEFAULT_BUTTON_HEIGTH,
                              DEFAULT_BUTTON_WIDTH,
                              DEFAULT_BUTTON_HEIGTH);
    auto_scroll_label_->setGeometry(2 * BORDER_MARGIN + max_column_amount_ * (DEFAULT_BUTTON_WIDTH) + MARGIN + DEFAULT_BUTTON_WIDTH / 2,
                              2 * BORDER_MARGIN + DEFAULT_BUTTON_HEIGTH + max_row_amount_ * DEFAULT_BUTTON_HEIGTH,
                              2 * DEFAULT_NAME_TEXT_WIDTH,
                              DEFAULT_BUTTON_HEIGTH);
}

void MainWindow::on_timeout()
{
    elapsed_seconds_ += 1;
    // Only update time every second, not the whole ui
    set_times();
}

void MainWindow::change_turn()
{
    // If turn button isn't enabled (meaning space was pressed in the middle of a turn), return
    if (!turn_button_->isEnabled()) {
        return;
    }
    turn_button_->setDisabled(true);
    // If there was a match this turn, remove cards and don't change the turn
    if (check_turn()) {
        match();
    } // If there was no match, just turn the cards back and change the turn
    else {
        no_match();
        turn_number_++;
        player_in_turn_ = list_of_players_.at(turn_number_ % list_of_players_.size());
    }
    // Empty current turn pair
    current_turn_cards_ = {nullptr, nullptr};
    refresh_ui();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // If C, CTRL or Enter is pressed, change the turn
    if(event->key() == Qt::Key_C || event->key() == Qt::Key_Control || event->key() == Qt::Key_Return )
        {
            change_turn();
        }
    // Change game size to smaller one
    else if (event->key() == Qt::Key_S) {
        // If it's not already on the last index
        if (game_size_selector_->currentIndex() != game_sizes_ - 1) {
            game_size_selector_->setCurrentIndex(game_size_selector_->currentIndex() + 1);
        }
    }
    // Change game size to bigger one
    else if (event->key() == Qt::Key_W) {
        // If it's not already on the first index
        if (game_size_selector_->currentIndex() != 0) {
            game_size_selector_->setCurrentIndex(game_size_selector_->currentIndex() - 1);
        }
    }
    // Restart game
    else if (event->key() == Qt::Key_Delete) {
        restart();
    }
}

void MainWindow::set_times()
{
    int minutes = elapsed_seconds_ / 60;
    int seconds = elapsed_seconds_ - minutes * 60;
    std::string minutes_string = std::to_string(minutes);
    std::string seconds_string = std::to_string(seconds);
    // Add leading zero
    if (minutes - 10 < 0) {
        minutes_string = "0" + minutes_string;
    }
    if (seconds - 10 < 0) {
        seconds_string = "0" + seconds_string;
    }
    timer_label_->setText(QString::fromStdString(minutes_string + ":" + seconds_string));
}

void MainWindow::set_game_sizes()
{
    QList<QString> list_of_items;
    // min 4 as 1x2 is not really a game, max letters: 2 * 27
    for (int game_size = 4; game_size < MAX_NUMBER_OF_CARDS; game_size += 2) {
        int rows = sqrt(game_size);
        int columns = game_size / rows;
        // If there are an uneven amount of tiles, continue
        if (columns * rows % 2 != 0) {
            continue;
        }
        QString item = QString::number(columns) + "x" + QString::number(rows);
        // Only add unique sizes
        if (std::find(list_of_items.begin(), list_of_items.end(), item) == list_of_items.end()) {
            list_of_items.push_back(item);
        }
        if (rows > max_row_amount_) {
            max_row_amount_ = rows;
        }
        if (columns > max_column_amount_) {
            max_column_amount_ = columns;
        }
    }
    // Add items in inverse order so that they intuitively get bigger when going up
    for (QString item: list_of_items) {
        game_size_selector_->insertItem(-1, item);
    }
    game_sizes_ = list_of_items.size();
}

// Sort funcion
bool sort_by_points(const Player* player1, const Player* player2) {
    return player1->get_points() > player2->get_points();
}

void MainWindow::read_all_time_players()
{
    ifstream file(STATS_FILE_NAME);
    if (!file) {
        return;
    }
    string line;
    while (getline(file, line)) {
        // Structure:
        // NAME:POINTS;WINS/GAMES
        int colon_index = line.find(":");
        int semicolon_index = line.find(";");
        int slash_index = line.find("/");
        // Read to colon
        QString name = QString::fromStdString(line.substr(0, colon_index));
        // Read from colon to semicolon, etc
        int points = stoi(line.substr(++colon_index, semicolon_index));
        int wins = stoi(line.substr(++semicolon_index, slash_index));
        int games = stoi(line.substr(++slash_index));
        Player* player = new Player(name);
        player->add_points(points);
        player->add_games(games);
        player->add_wins(wins);
        map_of_all_time_players_.insert({player->get_name(), player});
        list_of_all_time_players_.push_back(player);
    }
}

void MainWindow::set_all_time_player_scores()
{
    std::sort(list_of_all_time_players_.begin(), list_of_all_time_players_.end(), sort_by_points);
    QString text;
    for (Player* player: list_of_all_time_players_) {
        // Multiply by 1.0 to change to double
        double win_percentage = 1.0 * player->get_wins() / player->get_games() * 100;
        text += player->get_name() + ": " + QString::number(player->get_points()) + " (" + QString::number(round(win_percentage)) + "%)\n";
    }
    all_time_score_board_->setText(text);
}

void MainWindow::create_players()
{
    QString names_string = player_names_->toPlainText();
    QStringList list_of_names = names_string.split("\n");
    int i = 0;
    for (QString name: list_of_names) {
        i++;
        // Remove illegal characters from name to not break reading from a file
        for (char c: LIST_OF_ILLEGAL_CHARACTERS) {
            name.replace(c, "");
        }
        // Limit player names to certain number of characters
        name = name.left(MAX_PLAYER_NAME_LENGTH);
        Player* player = new Player(name);
        list_of_players_.push_back(player);
    }
}
void MainWindow::remove_players()
{
    // Do nothing if there are no players
    if (list_of_players_.size() == 0) {
        return;
    }
    for (Player* player: list_of_players_) {
        delete player;
    }
    list_of_players_.clear();
}

void MainWindow::end_game()
{
    // If there are only 2 cards left, it has to be the last turn
    if (list_of_cards_left_.size() <= 2) {
        // Match the last 2 cards
        match();
        timer_->stop();
        vector<Player*> winners = list_winners();
        // If there are more than 1 winner, it's a draw
        if (winners.size() > 1) {
            winner_player_name_->setText("It's a draw!");
        } else {
            winner_player_name_->setText(list_of_players_.at(0)->get_name() + " has won!");
        }
        winner_player_name_->setHidden(false);
        current_player_name_->setHidden(true);
        save_scores(winners);
        turn_button_->setDisabled(true);
    }
}

void MainWindow::save_scores(vector<Player*>& winners)
{
    for (Player* player: list_of_players_) {
        // If player already exists in all time players
        if (map_of_all_time_players_.find(player->get_name()) != map_of_all_time_players_.end()) {
            // Add a point
            map_of_all_time_players_.find(player->get_name())->second->add_points(player->get_points());
            // Add a game
            map_of_all_time_players_.find(player->get_name())->second->add_games(1);
            // If player is a winner, add a win
            if (std::find(winners.begin(), winners.end(), player) != winners.end()) {
                map_of_all_time_players_.find(player->get_name())->second->add_wins(1);
            }
        } else {
            // Make a new player object to not mess the game scores
            Player* temp_player = new Player(player->get_name());
            // Add a point
            temp_player->add_points(player->get_points());
            // Add a game
            temp_player->add_games(1);
            // If player is a winner, add a win
            if (std::find(winners.begin(), winners.end(), player) != winners.end()) {
                temp_player->add_wins(1);
            }
            map_of_all_time_players_.insert({temp_player->get_name(), temp_player});
            list_of_all_time_players_.push_back(temp_player);
        }
    }
    // Save to file
    ofstream file(STATS_FILE_NAME);
    for (auto pair: map_of_all_time_players_) {
        // NAME:POINTS;WINS/GAMES
        file << pair.first.toStdString() << ":" << pair.second->get_points() << ";" << pair.second->get_wins()  << "/" << pair.second->get_games() << std::endl;
    }
}

vector<Player*> MainWindow::list_winners()
{
    std::sort(list_of_players_.begin(), list_of_players_.end(), sort_by_points);
    vector<Player*> winners;
    for (Player* player: list_of_players_) {
        if (player->get_points() == list_of_players_.at(0)->get_points()) {
            winners.push_back(player);
        }
    }
    return winners;
}

void MainWindow::refresh_ui()
{
    // Get initial scoll bar location
    int scroll_to = score_board_->verticalScrollBar()->value();
    current_player_name_->setText("TURN: " + player_in_turn_->get_name());
    score_board_->setText("");
    int player_number = 1;
    for (Player* player: list_of_players_) {
        QString score_board_text;
        if (player == player_in_turn_) {
            // If autoscoll is enabled, calculate where to scroll)
            if (auto_scroll_->isChecked()) {
                // Calculate if the name is off-screen (good enough algorithm)
                if (4 * player_number * FONT_SIZE > score_board_->height()) {
                    scroll_to = score_board_->verticalScrollBar()->value() + (score_board_->verticalScrollBar()->height() / 2);
                } else {
                    scroll_to = 0;
                }
            }
            score_board_->setTextColor(QColor("#007F0E"));
        } else {
            score_board_->setTextColor(QColor("black"));
        }
        score_board_text += player->get_name() + ": " + QString::number(player->get_points());
        score_board_text += "\n" + player->get_cards();
        score_board_->append(score_board_text);
        player_number++;
    }
    score_board_->verticalScrollBar()->setValue(scroll_to);
}

void MainWindow::match()
{
    // Remove this turn's cards from pair
    list_of_cards_left_.erase(current_turn_cards_.first);
    list_of_cards_left_.erase(current_turn_cards_.second);
    // Add a point to player
    player_in_turn_->add_card(current_turn_cards_.first);
    // Remove this turn's cards
    current_turn_cards_.first->remove();
    current_turn_cards_.second->remove();
}

void MainWindow::no_match()
{
    // Turn this turn's cards back
    current_turn_cards_.first->turn();
    current_turn_cards_.second->turn();
}

bool MainWindow::check_turn() {
    // If it's currently the first turn or second card from a turn, return
    if (current_turn_cards_.second == nullptr) {
        return false;
    }
    // If there was a match
    if (current_turn_cards_.first->get_element() == current_turn_cards_.second->get_element()) {
        return true;
    } else {
        return false;
    }
}

void MainWindow::handle_card_click()
{
    // Get button that was clicked
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    // If both cards have been turned already in this turn and there is no match, do nothing
    if (current_turn_cards_.second != nullptr) {
        return;
    }
    // Get card
    Card* card = cards_.find(buttonSender)->second;
    // If the same button is pressed again, return
    if (current_turn_cards_.first == card) {
        return;
    }
    card->turn();
    refresh_ui();
    // If this is the first card of the turn, return
    if (current_turn_cards_.first == nullptr) {
        current_turn_cards_.first = card;
        return;
    }
    // Code below is only ran if it's the second card of a turn
    current_turn_cards_.second = card;
    turn_button_->setDisabled(false);
    // End game if it's the last turn
    end_game();
    refresh_ui();
}

void MainWindow::init_cards()
{
    double button_height = DEFAULT_BUTTON_HEIGTH;
    double button_width = DEFAULT_BUTTON_WIDTH;
    // To make button size scale with game size, uncomment two line below (it looks bad, but buttons are larger)
//    button_width = 1.0 * max_column_amount_ / column_amount_ * DEFAULT_BUTTON_WIDTH;
//    button_height = 1.0 * max_row_amount_ / row_amount_ * DEFAULT_BUTTON_HEIGTH;
    for (int row = 0; row < row_amount_; row++) {
        for (int column = 0; column < column_amount_; column++) {
            QPushButton* pushButton = new QPushButton("", this);
            Card* card = new Card(pushButton, button_height, button_width);
            cards_.insert({pushButton, card});
            pushButton->setGeometry(2 * BORDER_MARGIN + column * (button_width + BUTTON_MARGIN),
                                            2 * BORDER_MARGIN + row * (button_height + BUTTON_MARGIN) + DEFAULT_BUTTON_HEIGTH,
                                            button_width,
                                            button_height);
            connect(pushButton, &QPushButton::clicked, this, &MainWindow::handle_card_click);
        }
    }
    set_card_characters();
}

void MainWindow::remove_cards()
{
    // Do nothing if there are no cards
    if (cards_.size() == 0) {
        return;
    }
    for (auto pair: cards_) {
//        delete pair.first;
        delete pair.second;
    }
    list_of_cards_left_.clear();
    cards_.clear();
}

void MainWindow::set_card_characters()
{
    std::vector<QString> elements;
    get_shuffled_elements(elements);
    int i = 0;
    QString current_character;
    for (auto pair: cards_) {
        // Get a character from characters string
        current_character = elements.at(i);
        pair.second->set_element(current_character);
        pair.second->revive();
        list_of_cards_left_.insert(pair.second);
        i++;
    }

}

string MainWindow::get_shuffled_characters()
{
    string characters;
    // Count how many characters are needed
    int max_characters = row_amount_ * column_amount_ / 2;
    // Make a string from characters
    for (int i = 0, c = 'A'; i < max_characters; i++, c++) {
        // Clunky but cant use calculation as c is an integer
        characters += c;
        characters += c;
    }
    // Shuffle letters
    shuffle(characters.begin(), characters.end(), randomEng_);
    return characters;
}

void MainWindow::get_shuffled_elements(std::vector<QString>& elements)
{
    std::vector<QString> all_elements = {"H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si",
                                     "P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni",
                                     "Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb",
                                     "Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe",
                                     "Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho",
                                     "Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg",
                                     "Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np",
                                     "Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg",
                                     "Bh","Hs","Mt","Ds","Rg","Cn","Nh","Fl","Mc","Lv","Ts","Og"};
    // Shuffle all elements so the first ones arent the same always
    shuffle(all_elements.begin(), all_elements.end(), randomEng_);
    // Count how many characters are needed
    int max_characters = row_amount_ * column_amount_ / 2;
    for (int i = 0; i < max_characters; i++) {
        QString element = all_elements.at(i);
        elements.push_back(element);
        elements.push_back(element);
    }
    // Shuffle elements so pairs arent next to each other
    std::shuffle(elements.begin(), elements.end(), randomEng_);
}
