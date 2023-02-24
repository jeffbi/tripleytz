#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**************************************************************************
* Copyright (c) 2023 by Jeff Bienstadt                                    *
*                                                                         *
* This file is part of the tripleytz project.                             *
*                                                                         *
* tripleytz is free software: you can redistribute it and/or modify       *
* it under the terms of the GNU General Public License as published by    *
* the Free Software Foundation, either version 3 of the License, or       *
* (at your option) any later version.                                     *
*                                                                         *
* tripleytz is distributed in the hope that it will be useful, but        *
* WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU        *
* General Public License for more details.                                *
*                                                                         *
* You should have received a copy of the GNU General Public License along *
* with tripleytz. If not, see <https://www.gnu.org/licenses/>.            *
**************************************************************************/

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
    MainWindow(Config &config, QWidget *parent = nullptr);
    ~MainWindow();

private:
    void new_game();
    void end_game();
    void update_roll_button();
    int get_score_value(const Score *score);
    void show_high_scores_list();
    void update_grand_total(ScoreRow *row);

public slots:
    void score_entered(Score *score);
    void score_exited(Score *score);
    void score_clicked(Score *score);
    void row_changed(ScoreRow *row);
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
    void die_changed(int index, int value);
    void roll_clicked(bool checked);

private slots:
    void on_action_New_game_triggered();
    void on_action_Exit_triggered();
    void on_action_High_Scores_triggered();

private:
    static constexpr int    _max_rolls{3};
    static constexpr int    _max_plays{39};

    Ui::MainWindow *_ui;

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

    ScoreRow       *_total;         //TODO: This sould be a separate type, uncolored, for singled/doubled/tripled scores
    GrandTotalRow  *_grand_total;

    std::array<QPixmap *, 6>        _dice_pix;
    std::array<QPushButton *, 5>    _dice_btn;
    std::array<QCheckBox *, 5>      _dice_chk;

    QPushButton    *_btn_roll;
    int             _rolls_left{_max_rolls};
    int             _plays_left{_max_plays};

    ScoreColumn    *_column_single{nullptr};
    ScoreColumn    *_column_double{nullptr};
    ScoreColumn    *_column_triple{nullptr};

    Score          *_current_score_widget{nullptr};

    Config         &_config;
};

#endif // MAINWINDOW_H
