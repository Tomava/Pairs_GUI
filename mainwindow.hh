/* Pairs GUI
 *
 * Desc:
 * In the instructions.txt
 * */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QPushButton>
#include <QTextBrowser>
#include <QLabel>
#include <QTextBrowser>
#include <QTimer>
#include <QTextEdit>
#include <QComboBox>
#include <QKeyEvent>
#include <QScrollBar>
#include <QCheckBox>
#include <vector>
#include <map>
#include <random>
#include <string>
#include <set>
#include <time.h>
#include "card.hh"
#include "player.hh"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handle_card_click();
    void on_timeout();
    void change_turn();
    void keyPressEvent(QKeyEvent* event);

private:
    Ui::MainWindow *ui;
    // Space between elements, both horizontally and vertically
    const int MARGIN = 10;
    const int BUTTON_MARGIN = 0;
    const int BORDER_MARGIN = 30;

    int max_row_amount_ = -1;
    int max_column_amount_ = -1;
    // Default row and column amounts
    int row_amount_ = 2;
    int column_amount_ = 2;
    // Constants for push buttons
    const int DEFAULT_BUTTON_WIDTH = 50;
    const int DEFAULT_BUTTON_HEIGTH = 50;

    const int DEFAULT_NAME_TEXT_WIDTH = column_amount_ * DEFAULT_BUTTON_WIDTH;
    // Limit player names to certain number of characters
    const int MAX_PLAYER_NAME_LENGTH = 16;
    // If using alphabet, use 2 * 26
    const int MAX_NUMBER_OF_CARDS = 2 * 33;
    // List of disallowed characters in a player name
    const vector<char> LIST_OF_ILLEGAL_CHARACTERS = {':', ';', '/'};
    const int FONT_SIZE = 12;

    const string STATS_FILE_NAME = "stats";
    // Maps each button to a card
    map<QPushButton*, Card*> cards_;
    // Current turn's cards
    pair<Card*, Card*> current_turn_cards_;
    // List of cards still in the game
    set<Card*> list_of_cards_left_;

    QLabel* current_player_name_;
    QLabel* winner_player_name_;
    vector<Player*> list_of_players_;
    // Used to know which player already exists
    map<QString, Player*> map_of_all_time_players_;
    // Only used to sort players
    vector<Player*> list_of_all_time_players_;

    QTextBrowser* score_board_;
    QLabel* score_board_instructions_;
    QTextBrowser* all_time_score_board_;
    QLabel* all_time_score_board_instructions_;
    QTimer* timer_ = new QTimer();;
    QLabel* timer_label_;
    QTextEdit* player_names_;
    QLabel* player_names_instructions_;
    QPushButton* restart_button_;
    QPushButton* turn_button_;
    QComboBox* game_size_selector_;
    QCheckBox* auto_scroll_;
    QLabel* auto_scroll_label_;
    int game_sizes_;
    // Current player in turn
    Player* player_in_turn_;
    // How many turn have passed
    unsigned int turn_number_;
    std::default_random_engine randomEng_;


    // Initialize cards
    void init_cards();
    void remove_cards();
    void restart();
    void set_geometry();
    // Set characters for each card
    void set_card_characters();
    // Shuffle characters
    std::string get_shuffled_characters();    
    // Shuffle elements
    void get_shuffled_elements(std::vector<QString>& elements);
    // Handles a match
    void match();
    // Handles a turn where no match was made
    void no_match();
    // Checks if there is match in this turn
    bool check_turn();
    // Creates players
    void create_players();
    void remove_players();
    // Checks if game has ended
    void end_game();
    // Saves scores to a file
    void save_scores(vector<Player*>& winners);
    // Returns a list of winners
    vector<Player*> list_winners();
    // Refreshes all text in the UI
    void refresh_ui();
    // Time elapsed
    unsigned int elapsed_seconds_;
    // Refresh clock UI
    void set_times();
    // Calculates all game sizes and sets them
    void set_game_sizes();
    // Reads all time players from a file
    void read_all_time_players();
    void set_all_time_player_scores();
};
#endif // MAINWINDOW_HH
