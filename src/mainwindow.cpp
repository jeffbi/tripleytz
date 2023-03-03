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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>

#include <array>
#include <cassert>

#include "gamescorer.h"
#include "highscoresdialog.h"
#include "ace.xpm"
#include "two.xpm"
#include "three.xpm"
#include "four.xpm"
#include "five.xpm"
#include "six.xpm"

namespace {
void set_score_arrays(const ScoreRow *row,
                      std::array<Score *, 20>::size_type ndx,
                      std::array<Score *, 20> &singles,
                      std::array<Score *, 20> &doubles,
                      std::array<Score *, 20> &triples)
{
    const auto [s, d, t] = row->get_scores();
    singles[ndx] = s;
    doubles[ndx] = d;
    triples[ndx] = t;
}
}   // anonymous namespace

MainWindow::MainWindow(Config &config, QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , _aces{new ScoreRow{tr("Aces")}}
  , _twos{new ScoreRow{tr("Twos")}}
  , _threes{new ScoreRow{tr("Threes")}}
  , _fours{new ScoreRow{tr("Fours")}}
  , _fives{new ScoreRow{tr("Fives")}}
  , _sixes{new ScoreRow{tr("Sixes")}}
  , _upper_sub_total{new ScoreRow{tr("Total")}}
  , _bonus{new ScoreRow{tr("Bonus")}}
  , _upper_total{new ScoreRow{tr("Total")}}
  , _three_of_a_kind{new ScoreRow{tr("Three of a kind")}}
  , _four_of_a_kind{new ScoreRow{tr("Four of a kind")}}
  , _full_house{new ScoreRow{tr("Full House")}}
  , _small_straight{new ScoreRow{tr("Sm. Straight")}}
  , _large_straight{new ScoreRow{tr("Lg. Straight")}}
  , _yahtzee{new ScoreRow{tr("YAHTZEE")}}
  , _chance{new ScoreRow{tr("Chance")}}
  , _lower_section_total{new ScoreRow{tr("Lower Section Total")}}
  , _upper_section_total{new ScoreRow{tr("Upper Section Total")}}
  , _combined_total{new ScoreRow{tr("Combined Total")}}
  , _total{new ScoreRow{tr("Total Score")}}
  , _dice_pix{nullptr}
  , _dice_btn{nullptr}
  , _dice_chk{nullptr}
  , _config{config}
{
    ui->setupUi(this);

    QWidget        *cw{ui->centralwidget};
    QVBoxLayout    *layout{new QVBoxLayout{cw}};

    layout->setSpacing(0);
    layout->setContentsMargins(0, 6, 0, 0);

    QLabel *section_label{new QLabel{tr("Upper Section")}};
    section_label->setStyleSheet("font-weight: bold");
    section_label->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    layout->addWidget(section_label);
    layout->addItem(new QSpacerItem{0, 4});
    layout->addWidget(_aces);
    layout->addWidget(_twos);
    layout->addWidget(_threes);
    layout->addWidget(_fours);
    layout->addWidget(_fives);
    layout->addWidget(_sixes);

    layout->addItem(new QSpacerItem{0, 4});
    layout->addWidget(_upper_sub_total);
    layout->addWidget(_bonus);
    layout->addWidget(_upper_total);
    _upper_sub_total->ui_enabled(false);
    _bonus->ui_enabled(false);
    _upper_total->ui_enabled(false);

    layout->addItem(new QSpacerItem{0, 10});
    section_label = new QLabel{tr("Lower Section")};
    section_label->setStyleSheet("font-weight: bold");
    section_label->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    layout->addWidget(section_label);
    layout->addItem(new QSpacerItem{0, 4});
    layout->addWidget(_three_of_a_kind);
    layout->addWidget(_four_of_a_kind);
    layout->addWidget(_full_house);
    layout->addWidget(_small_straight);
    layout->addWidget(_large_straight);
    layout->addWidget(_yahtzee);
    layout->addWidget(_chance);

    layout->addItem(new QSpacerItem{0, 4});
    layout->addWidget(_lower_section_total);
    layout->addWidget(_upper_section_total);
    layout->addWidget(_combined_total);
    _lower_section_total->ui_enabled(false);
    _upper_section_total->ui_enabled(false);
    _combined_total->ui_enabled(false);

    QHBoxLayout *mult_layout{new QHBoxLayout{}};
    mult_layout->addStretch();

    // Lay out the multiplier indicators just above the total score boxes.
    auto [s, d, t] = _total->get_scores();
    for (int i{1}; i < 4; ++i)
    {
        QLabel *mult_label{new QLabel{tr("x%1").arg(i)}};
        mult_label->setFixedWidth(s->width());  // Make the label width the same as the scoring box.
        mult_label->setAlignment(Qt::AlignHCenter | Qt::AlignHCenter);
        mult_layout->addWidget(mult_label);
    }
    mult_layout->addSpacing(8);
    layout->addLayout(mult_layout);

    layout->addWidget(_total);
    _total->ui_enabled(false);

    layout->addItem(new QSpacerItem{0, 4});
    _grand_total = new GrandTotalRow{this};
    layout->addWidget(_grand_total);


    _dice_pix[0] = new QPixmap(ace_xpm);
    _dice_pix[1] = new QPixmap(two_xpm);
    _dice_pix[2] = new QPixmap(three_xpm);
    _dice_pix[3] = new QPixmap(four_xpm);
    _dice_pix[4] = new QPixmap(five_xpm);
    _dice_pix[5] = new QPixmap(six_xpm);

    QHBoxLayout *hbl{new QHBoxLayout{}};
    for (size_t i{0}; i < 5; ++i)
    {
        _dice_btn[i] = new QPushButton(*_dice_pix[i], "");
        _dice_btn[i]->setFlat(true);
        _dice_btn[i]->setIconSize(_dice_pix[i]->size());
        _dice_btn[i]->setFixedSize(_dice_pix[i]->size());
        _dice_chk[i] = new QCheckBox(tr("Keep"));
        _dice_chk[i]->setEnabled(false);
        QVBoxLayout *vbl{new QVBoxLayout{}};
        vbl->addWidget(_dice_btn[i]);
        vbl->addWidget(_dice_chk[i]);
        hbl->addLayout(vbl);
        if (i < 4)
            hbl->addSpacing(4);
    }
    layout->addLayout(hbl);

    _btn_roll = new QPushButton{};
    auto *hlayout = new QHBoxLayout{};
    hlayout->addWidget(_btn_roll);
    layout->addLayout(hlayout);

    enable_undo(false);
    update_roll_button();

    setFixedSize(sizeHint());
    setWindowTitle(tr("Triple Yahtzee"));


    connect(_aces, &ScoreRow::on_enter, this, &MainWindow::score_entered);
    connect(_aces, &ScoreRow::on_exit, this, &MainWindow::score_exited);
    connect(_aces, &ScoreRow::on_click, this, &MainWindow::score_clicked);
    connect(_twos, &ScoreRow::on_enter, this, &MainWindow::score_entered);
    connect(_twos, &ScoreRow::on_exit, this, &MainWindow::score_exited);
    connect(_twos, &ScoreRow::on_click, this, &MainWindow::score_clicked);
    connect(_threes, &ScoreRow::on_enter, this, &MainWindow::score_entered);
    connect(_threes, &ScoreRow::on_exit, this, &MainWindow::score_exited);
    connect(_threes, &ScoreRow::on_click, this, &MainWindow::score_clicked);
    connect(_fours, &ScoreRow::on_enter, this, &MainWindow::score_entered);
    connect(_fours, &ScoreRow::on_exit, this, &MainWindow::score_exited);
    connect(_fours, &ScoreRow::on_click, this, &MainWindow::score_clicked);
    connect(_fives, &ScoreRow::on_enter, this, &MainWindow::score_entered);
    connect(_fives, &ScoreRow::on_exit, this, &MainWindow::score_exited);
    connect(_fives, &ScoreRow::on_click, this, &MainWindow::score_clicked);
    connect(_sixes, &ScoreRow::on_enter, this, &MainWindow::score_entered);
    connect(_sixes, &ScoreRow::on_exit, this, &MainWindow::score_exited);
    connect(_sixes, &ScoreRow::on_click, this, &MainWindow::score_clicked);

    connect(_three_of_a_kind, &ScoreRow::on_enter, this, &MainWindow::score_entered);
    connect(_three_of_a_kind, &ScoreRow::on_exit, this, &MainWindow::score_exited);
    connect(_three_of_a_kind, &ScoreRow::on_click, this, &MainWindow::score_clicked);
    connect(_four_of_a_kind, &ScoreRow::on_enter, this, &MainWindow::score_entered);
    connect(_four_of_a_kind, &ScoreRow::on_exit, this, &MainWindow::score_exited);
    connect(_four_of_a_kind, &ScoreRow::on_click, this, &MainWindow::score_clicked);
    connect(_full_house, &ScoreRow::on_enter, this, &MainWindow::score_entered);
    connect(_full_house, &ScoreRow::on_exit, this, &MainWindow::score_exited);
    connect(_full_house, &ScoreRow::on_click, this, &MainWindow::score_clicked);
    connect(_small_straight, &ScoreRow::on_enter, this, &MainWindow::score_entered);
    connect(_small_straight, &ScoreRow::on_exit, this, &MainWindow::score_exited);
    connect(_small_straight, &ScoreRow::on_click, this, &MainWindow::score_clicked);
    connect(_large_straight, &ScoreRow::on_enter, this, &MainWindow::score_entered);
    connect(_large_straight, &ScoreRow::on_exit, this, &MainWindow::score_exited);
    connect(_large_straight, &ScoreRow::on_click, this, &MainWindow::score_clicked);
    connect(_yahtzee, &ScoreRow::on_enter, this, &MainWindow::score_entered);
    connect(_yahtzee, &ScoreRow::on_exit, this, &MainWindow::score_exited);
    connect(_yahtzee, &ScoreRow::on_click, this, &MainWindow::score_clicked);
    connect(_chance, &ScoreRow::on_enter, this, &MainWindow::score_entered);
    connect(_chance, &ScoreRow::on_exit, this, &MainWindow::score_exited);
    connect(_chance, &ScoreRow::on_click, this, &MainWindow::score_clicked);

    connect(_total, &ScoreRow::on_changed, this, &MainWindow::row_changed);

    connect(_dice_btn[0], &QPushButton::clicked, this, &MainWindow::die_0_clicked);
    connect(_dice_btn[1], &QPushButton::clicked, this, &MainWindow::die_1_clicked);
    connect(_dice_btn[2], &QPushButton::clicked, this, &MainWindow::die_2_clicked);
    connect(_dice_btn[3], &QPushButton::clicked, this, &MainWindow::die_3_clicked);
    connect(_dice_btn[4], &QPushButton::clicked, this, &MainWindow::die_4_clicked);
    connect(_dice_chk[0], &QAbstractButton::toggled, this, &MainWindow::keep_0_toggled);
    connect(_dice_chk[1], &QAbstractButton::toggled, this, &MainWindow::keep_1_toggled);
    connect(_dice_chk[2], &QAbstractButton::toggled, this, &MainWindow::keep_2_toggled);
    connect(_dice_chk[3], &QAbstractButton::toggled, this, &MainWindow::keep_3_toggled);
    connect(_dice_chk[4], &QAbstractButton::toggled, this, &MainWindow::keep_4_toggled);
    connect(&_dice, &Dice::on_die_changed, this, &MainWindow::die_changed);

    connect(_btn_roll, &QPushButton::clicked, this, &MainWindow::roll_clicked);

    //
    // The following code sets up the ScoreColumn objects.
    //
    std::array<Score *, 20> singles{nullptr};
    std::array<Score *, 20> doubles{nullptr};
    std::array<Score *, 20> triples{nullptr};

    set_score_arrays(_aces,                 0, singles, doubles, triples);
    set_score_arrays(_twos,                 1, singles, doubles, triples);
    set_score_arrays(_threes,               2, singles, doubles, triples);
    set_score_arrays(_fours,                3, singles, doubles, triples);
    set_score_arrays(_fives,                4, singles, doubles, triples);
    set_score_arrays(_sixes,                5, singles, doubles, triples);
    set_score_arrays(_upper_sub_total,      6, singles, doubles, triples);
    set_score_arrays(_bonus,                7, singles, doubles, triples);
    set_score_arrays(_upper_total,          8, singles, doubles, triples);
    set_score_arrays(_three_of_a_kind,      9, singles, doubles, triples);
    set_score_arrays(_four_of_a_kind,      10, singles, doubles, triples);
    set_score_arrays(_full_house,          11, singles, doubles, triples);
    set_score_arrays(_small_straight,      12, singles, doubles, triples);
    set_score_arrays(_large_straight,      13, singles, doubles, triples);
    set_score_arrays(_yahtzee,             14, singles, doubles, triples);
    set_score_arrays(_chance,              15, singles, doubles, triples);
    set_score_arrays(_lower_section_total, 16, singles, doubles, triples);
    set_score_arrays(_upper_section_total, 17, singles, doubles, triples);
    set_score_arrays(_combined_total,      18, singles, doubles, triples);
    set_score_arrays(_total,               19, singles, doubles, triples);
    _column_single = new ScoreColumn{1, singles};
    _column_double = new ScoreColumn{2, doubles};
    _column_triple = new ScoreColumn{3, triples};
}

MainWindow::~MainWindow()
{
    delete ui;

    delete _column_single;
    delete _column_double;
    delete _column_triple;

    for (auto dp : _dice_pix)
        delete dp;
}

void MainWindow::end_game()
{
    assert(_grand_total->total().has_value());
    int game_score = _grand_total->total().value_or(0);

    QString     msg{tr("Your final score is %1!").arg(game_score)};
    QMessageBox mb{QMessageBox::Icon::Information, "TripleYtz", msg, QMessageBox::StandardButton::Ok, this};

    mb.exec();

    // handle high scores
    if (_config.is_high_score(game_score))
    {
        bool    ok;
        QString name = QInputDialog::getText(this, tr("High Score!"),
                                             tr("You made it to the high score list.\nPlease enter your name:"),
                                             QLineEdit::Normal, _config.last_used_name(), &ok);
        if (ok)
        {
            _config.last_used_name(name);
            if (_config.add_high_score(game_score, name))
            {
                _config.save();
                show_high_scores_list();
            }
        }
    }

    new_game();
}

void MainWindow::new_game()
{
    _current_score_widget = nullptr;

    _aces->reset();
    _twos->reset();
    _threes->reset();
    _fours->reset();
    _fives->reset();
    _sixes->reset();
    _three_of_a_kind->reset();
    _four_of_a_kind->reset();
    _full_house->reset();
    _small_straight->reset();
    _large_straight->reset();
    _yahtzee->reset();
    _chance->reset();
    update_grand_total(_total);

    _dice.reset();

    _rolls_left = 3;
    _plays_left = 39;
    update_roll_button();
    _btn_roll->setEnabled(true);
    enable_undo(false);
    for (auto k : _dice_chk)
    {
        k->setChecked(false);
        k->setEnabled(false);
    }
}

void MainWindow::update_roll_button()
{
    _btn_roll->setText(tr("Roll! (%1 left)").arg(_rolls_left));
}

int MainWindow::get_score_value(const Score *score)
{
    ScoreRow    *sr = static_cast<ScoreRow *>(score->parent());
    if (sr == _aces)
        return sr->get_score_value(_dice, [](const GameScorer &gs){return gs.aces();});
    else if (sr == _twos)
        return sr->get_score_value(_dice, [](const GameScorer &gs){return gs.twos();});
    else if (sr == _threes)
        return sr->get_score_value(_dice, [](const GameScorer &gs){return gs.threes();});
    else if (sr == _fours)
        return sr->get_score_value(_dice, [](const GameScorer &gs){return gs.fours();});
    else if (sr == _fives)
        return sr->get_score_value(_dice, [](const GameScorer &gs){return gs.fives();});
    else if (sr == _sixes)
        return sr->get_score_value(_dice, [](const GameScorer &gs){return gs.sixes();});
    else if (sr == _three_of_a_kind)
        return sr->get_score_value(_dice, [](const GameScorer &gs){return gs.three_of_a_kind();});
    else if (sr == _four_of_a_kind)
        return sr->get_score_value(_dice, [](const GameScorer &gs){return gs.four_of_a_kind();});
    else if (sr == _full_house)
        return sr->get_score_value(_dice, [](const GameScorer &gs){return gs.full_house();});
    else if (sr == _small_straight)
        return sr->get_score_value(_dice, [](const GameScorer &gs){return gs.small_straight();});
    else if (sr == _large_straight)
        return sr->get_score_value(_dice, [](const GameScorer &gs){return gs.large_straight();});
    else if (sr == _yahtzee)
        return sr->get_score_value(_dice, [](const GameScorer &gs){return gs.yahtzee();});
    else if (sr == _chance)
        return sr->get_score_value(_dice, [](const GameScorer &gs){return gs.chance();});
    else
        return 0;
}

void MainWindow::show_high_scores_list()
{
    HighScoresDialog    dlg{_config, this};

    dlg.exec();
}

void MainWindow::update_grand_total(ScoreRow *row)
{
    std::optional<int>  opt;
    int total{0};
    auto [s, d, t] = row->get_scores();

    if (s->has_score())
    {
        total += s->value();
        opt = total;
    }

    if (d->has_score())
    {
        total += d->value();
        opt = total;
    }

    if (t->has_score())
    {
        total += t->value();
        opt = total;
    }

    _grand_total->total(opt);
}


//
// Slots
//
void MainWindow::score_entered(Score *score)
{
    if (!score->has_score() && _rolls_left < 3)
    {
        //_current_score_widget = score;
        void die_0_clicked();

        score->preview_score(get_score_value(score));
    }
}

void MainWindow::score_exited(Score *score)
{
    if (!score->has_score() || score->previewing())
    {
        //_current_score_widget = nullptr;
        score->reset();
    }
}

void MainWindow::score_clicked(Score *score)
{
    if ((!score->has_score() || score->previewing()) && _rolls_left < 3)
    {
        if (_current_score_widget)
            _current_score_widget->reset();
        _current_score_widget = score;
        score->set(get_score_value(score));
        if (--_plays_left == 0)
        {
            end_game();
        }
        else
        {
            for (auto d : _dice_chk)
                d->setChecked(false);
            _btn_roll->setEnabled(true);
            _undo_rolls_left = _rolls_left;
            _rolls_left = _max_rolls;
            enable_undo(true);
            update_roll_button();
        }
    }
}

void MainWindow::enable_undo(bool enabled)
{
    ui->action_Undo->setEnabled(enabled);
}

///
/// \brief  Slot for handling when a ScoreRow changes.
/// \param row  Pointer to the row that changed.
///
/// This slot should only be attached to the row holding the three final column scores.
///
void MainWindow::row_changed(ScoreRow *row)
{
    update_grand_total(row);
}

void MainWindow::die_0_clicked()
{
    if (_rolls_left < _max_rolls)
        _dice_chk[0]->toggle();
}
void MainWindow::die_1_clicked()
{
    if (_rolls_left < _max_rolls)
        _dice_chk[1]->toggle();
}
void MainWindow::die_2_clicked()
{
    if (_rolls_left < _max_rolls)
        _dice_chk[2]->toggle();
}
void MainWindow::die_3_clicked()
{
    if (_rolls_left < _max_rolls)
        _dice_chk[3]->toggle();
}
void MainWindow::die_4_clicked()
{
    if (_rolls_left < _max_rolls)
        _dice_chk[4]->toggle();
}

void MainWindow::keep_0_toggled(bool checked)
{
    _dice.select(0, checked);
}
void MainWindow::keep_1_toggled(bool checked)
{
    _dice.select(1, checked);
}
void MainWindow::keep_2_toggled(bool checked)
{
    _dice.select(2, checked);
}
void MainWindow::keep_3_toggled(bool checked)
{
    _dice.select(3, checked);
}
void MainWindow::keep_4_toggled(bool checked)
{
    _dice.select(4, checked);
}

void MainWindow::die_changed(int index, int value)
{
    _dice_btn[index]->setIcon(*_dice_pix[value - 1]);
    _dice_btn[index]->repaint();
}

void MainWindow::roll_clicked(bool checked)
{
    for (auto k : _dice_chk)
        k->setEnabled(true);
    _dice.roll();
    if (--_rolls_left == 0)
        _btn_roll->setEnabled(false);
    update_roll_button();
    _current_score_widget = nullptr;
    enable_undo(false);
}

void MainWindow::on_action_New_game_triggered()
{
    new_game();
}


void MainWindow::on_action_Exit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_action_High_Scores_triggered()
{
    show_high_scores_list();
}

void MainWindow::on_action_Undo_triggered()
{
    // This should cover the basics of undo.
    // More to come once Yahtzee bonus/wildcard code is in place.
    if (_current_score_widget)
    {
        _current_score_widget->reset();
        _current_score_widget = nullptr;
        _rolls_left = _undo_rolls_left;
        ++_plays_left;
        update_roll_button();
        enable_undo(false);
    }
}
