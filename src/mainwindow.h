#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


#include <QCheckBox>
#include <QPixmap>
#include <QPushButton>

#include <array>

#include "config.h"
#include "dice.h"
#include "score.h"
#include "scorecolumn.h"
#include "scorerow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void new_game();
    void end_game();
    void update_dice_widgets();
    void update_roll_button();
    int get_score_value(const Score *score);

public slots:
    void score_entered(Score *score);
    void score_exited(Score *score);
    void score_clicked(Score *score);
    void die_0_clicked();
    void die_1_clicked();
    void die_2_clicked();
    void die_3_clicked();
    void die_4_clicked();
    void keep_0_toggled(bool checked);
    void keep_1_toggled(bool checked);
    void keep_2_toggled(bool checked);
    void keep_3_toggled(bool checked);
    void keep_4_toggled(bool checked);
    void roll_released();

private slots:
    void on_action_New_game_triggered();

    void on_action_Exit_triggered();

private:
    static constexpr int    _max_rolls{3};
    static constexpr int    _max_plays{39};

    Ui::MainWindow *ui;

    Dice            _dice;

    ScoreRow       *_aces;
    ScoreRow       *_twos;
    ScoreRow       *_threes;
    ScoreRow       *_fours;
    ScoreRow       *_fives;
    ScoreRow       *_sixes;
    ScoreRow       *_upper_sub_total;
    ScoreRow       *_bonus;
    ScoreRow       *_upper_total;

    ScoreRow       *_three_of_a_kind;
    ScoreRow       *_four_of_a_kind;
    ScoreRow       *_full_house;
    ScoreRow       *_small_straight;
    ScoreRow       *_large_straight;
    ScoreRow       *_yahtzee;
    ScoreRow       *_chance;
    ScoreRow       *_lower_section_total;
    ScoreRow       *_upper_section_total;
    ScoreRow       *_combined_total;

    ScoreRow       *_total;     //TODO: This sould be a separate type, uncolored, for singled/douibled/tripled scores
    ScoreRow       *_grand_total{nullptr};  //TODO: need separate type for this one-score field

    std::array<QPixmap *, 6>        _dice_pix;
    std::array<QPushButton *, 5>    _dice_btn;
    std::array<QCheckBox *, 5>      _dice_chk;

    QPushButton    *_btn_roll;
    int             _rolls_left{_max_rolls};
    int             _plays_left{_max_plays};
    int             _grand_total_score{0};

    ScoreColumn    *_column_single{nullptr};
    ScoreColumn    *_column_double{nullptr};
    ScoreColumn    *_column_triple{nullptr};

    Score          *_current_score_widget{nullptr};

    Config          _config;
};

#endif // MAINWINDOW_H
