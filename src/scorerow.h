#ifndef SCOREROW_H
#define SCOREROW_H

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

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>

#include <functional>
#include <optional>
#include <tuple>

#include "dice.h"
#include "gamescorer.h"
#include "score.h"


class ScoreRow : public QWidget
{
    Q_OBJECT

public:
    explicit ScoreRow(const QString &text, QWidget *parent = nullptr)
      : QWidget(parent)
      , _ui_enabled{true}
      , _label{new QLabel(text, this)}
      , _score_single{new Score(this)}
      , _score_double{new Score(this)}
      , _score_triple{new Score{this}}
    {
        _label->setAlignment(Qt::AlignRight);

        constexpr int score_width{40};
        _score_single->setStyleSheet("Score {background-color: rgb(250, 250, 250); color: black}");
        _score_single->setFixedWidth(score_width);
        _score_double->setStyleSheet("Score {background-color: rgb(114, 159, 207); color: black}");
        _score_double->setFixedWidth(score_width);
        _score_triple->setStyleSheet("Score {background-color: rgb(242, 76, 80); color: black}");
        _score_triple->setFixedWidth(score_width);

        // Lay out the child widgets
        QHBoxLayout    *layout{new QHBoxLayout(this)};
        QHBoxLayout    *text_layout{new QHBoxLayout()};

        text_layout->addWidget(_label);
        layout->addLayout(text_layout);
        layout->addSpacing(6);
        layout->addWidget(_score_single);
        layout->addWidget(_score_double);
        layout->addWidget(_score_triple);

        layout->setSpacing(0);
        layout->setContentsMargins(-1, 0, -1, 0);

        connect(_score_single, &Score::on_enter, this, &ScoreRow::score_entered);
        connect(_score_single, &Score::on_leave, this, &ScoreRow::score_exited);
        connect(_score_single, &Score::clicked, this, &ScoreRow::single_clicked);
        connect(_score_double, &Score::on_enter, this, &ScoreRow::score_entered);
        connect(_score_double, &Score::on_leave, this, &ScoreRow::score_exited);
        connect(_score_double, &Score::clicked, this, &ScoreRow::double_clicked);
        connect(_score_triple, &Score::on_enter, this, &ScoreRow::score_entered);
        connect(_score_triple, &Score::on_leave, this, &ScoreRow::score_exited);
        connect(_score_triple, &Score::clicked, this, &ScoreRow::triple_clicked);

        connect(_score_single, &Score::on_changed, this, &ScoreRow::score_changed);
        connect(_score_double, &Score::on_changed, this, &ScoreRow::score_changed);
        connect(_score_triple, &Score::on_changed, this, &ScoreRow::score_changed);
    }

    ~ScoreRow()
    {}

    bool ui_enabled() const noexcept
    {
        return _ui_enabled;
    }
    void ui_enabled(bool enabled)
    {
        if (enabled != _ui_enabled)
        {
            _ui_enabled = enabled;
            _score_single->setEnabled(_ui_enabled);
            _score_double->setEnabled(_ui_enabled);
            _score_triple->setEnabled(_ui_enabled);
        }
    }

    void reset()
    {
        _score_single->reset();
        _score_double->reset();
        _score_triple->reset();
        ui_enabled(true);
    }

    int get_score_value(const Dice &dice,
                        std::function<int(const GameScorer &)> func)
    {
        return func(GameScorer{dice});
    }

    std::tuple<Score *, Score *, Score *> get_scores() const
    {
        return {_score_single, _score_double, _score_triple};
    }

signals:
    void on_enter(Score *score);
    void on_exit(Score *score);
    void on_click(Score *score);
    void on_changed(ScoreRow *row);

private slots:
    void score_entered(Score *score)
    {
        emit on_enter(score);
    }
    void score_exited(Score *score)
    {
        emit on_exit(score);
    }
    void score_clicked(Score *score)
    {
        emit on_click(score);
    }
    void score_changed(Score *score)
    {
        emit on_changed(this);
    }

    void single_clicked()
    {
        emit on_click(_score_single);
    }
    void double_clicked()
    {
        emit on_click(_score_double);
    }
    void triple_clicked()
    {
        emit on_click(_score_triple);
    }

private:
    bool    _ui_enabled;
    QLabel *_label;
    Score  *_score_single;
    Score  *_score_double;
    Score  *_score_triple;
};

class GrandTotalRow : public QWidget
{
public:
    GrandTotalRow(QWidget *parent = nullptr)
      : QWidget{parent}
      , _label{new QLabel{tr("Grand Total"), this}}
      , _button{new QPushButton{this}}
    {
        _button->setFixedWidth(120);
        _button->setStyleSheet("QPushButton {background-color: rgb(250, 250, 250); color: black}");
        _label->setAlignment(Qt::AlignRight);

        QHBoxLayout    *layout{new QHBoxLayout{this}};

        layout->addWidget(_label);
        layout->addWidget(_button);
    }

    void total(std::optional<int> value)
    {
        static QString empty{""};

        _value = value;
        _button->setText(value.has_value() ? QString::number(value.value()) : empty);
    }
    std::optional<int> total()
    {
        return _value;
    }

private:
    std::optional<int>  _value;
    QLabel             *_label;
    QPushButton        *_button;
};

#endif // SCOREROW_H
