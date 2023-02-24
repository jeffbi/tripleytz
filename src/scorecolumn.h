#ifndef SCORECOLUMN_H
#define SCORECOLUMN_H

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

#include <QObject>

#include <array>

#include "score.h"

class ScoreColumn : public QObject
{
    Q_OBJECT

public:
    explicit ScoreColumn(int multiplier, const std::array<Score *, 20> &scores)
      : _multiplier{multiplier}
      , _aces{scores[0]}
      , _twos{scores[1]}
      , _threes{scores[2]}
      , _fours{scores[3]}
      , _fives{scores[4]}
      , _sixes{scores[5]}
      , _upper_sub_total{scores[6]}
      , _bonus{scores[7]}
      , _upper_total{scores[8]}
      , _three_of_a_kind{scores[9]}
      , _four_of_a_kind{scores[10]}
      , _full_house{scores[11]}
      , _small_straight{scores[12]}
      , _large_straight{scores[13]}
      , _yahtzee{scores[14]}
      , _chance{scores[15]}
      , _lower_section_total{scores[16]}
      , _upper_section_total{scores[17]}
      , _combined_total{scores[18]}
      , _total{scores[19]}
    {
        connect(_aces, &Score::on_changed, this, &ScoreColumn::score_changed);
        connect(_twos, &Score::on_changed, this, &ScoreColumn::score_changed);
        connect(_threes, &Score::on_changed, this, &ScoreColumn::score_changed);
        connect(_fours, &Score::on_changed, this, &ScoreColumn::score_changed);
        connect(_fives, &Score::on_changed, this, &ScoreColumn::score_changed);
        connect(_sixes, &Score::on_changed, this, &ScoreColumn::score_changed);
        connect(_three_of_a_kind, &Score::on_changed, this, &ScoreColumn::score_changed);
        connect(_four_of_a_kind, &Score::on_changed, this, &ScoreColumn::score_changed);
        connect(_full_house, &Score::on_changed, this, &ScoreColumn::score_changed);
        connect(_small_straight, &Score::on_changed, this, &ScoreColumn::score_changed);
        connect(_large_straight, &Score::on_changed, this, &ScoreColumn::score_changed);
        connect(_yahtzee, &Score::on_changed, this, &ScoreColumn::score_changed);
        connect(_chance, &Score::on_changed, this, &ScoreColumn::score_changed);
    }

private:
    bool is_upper(Score *score)
    {
        return (   score == _aces
                || score == _twos
                || score == _threes
                || score == _fours
                || score == _fives
                || score == _sixes);
    }
    bool is_lower(Score *score)
    {
        return (   score == _three_of_a_kind
                || score == _four_of_a_kind
                || score == _full_house
                || score == _small_straight
                || score == _large_straight
                || score == _yahtzee
                || score == _chance);
    }

private slots:
    void score_changed(Score *score)
    {
        if (is_upper(score))
        {
            int     total{0};
            int     has_score_count{0};

            if (_aces->has_score())
            {
                total += _aces->value();
                ++has_score_count;
            }
            if (_twos->has_score())
            {
                total += _twos->value();
                ++has_score_count;
            }
            if (_threes->has_score())
            {
                total += _threes->value();
                ++has_score_count;
            }
            if (_fours->has_score())
            {
                total += _fours->value();
                ++has_score_count;
            }
            if (_fives->has_score())
            {
                total += _fives->value();
                ++has_score_count;
            }
            if (_sixes->has_score())
            {
                total += _sixes->value();
                ++has_score_count;
            }

            if (has_score_count)
            {
                _upper_sub_total->set(total);
                if (total >= 63)
                {
                    _bonus->set(35);
                    _upper_total->set(35 + total);
                }
                else
                {
                    _bonus->reset();
                    _upper_total->set(total);
                }
                _upper_section_total->set(_upper_total->value());
            }
            else
            {
                _upper_sub_total->reset();
                _bonus->reset();
                _upper_total->reset();
                _upper_section_total->reset();
            }
        }
        else if (is_lower(score))
        {
            int     total{0};
            int     has_score_count{0};

            if (_three_of_a_kind->has_score())
            {
                total += _three_of_a_kind->value();
                ++has_score_count;
            }
            if (_four_of_a_kind->has_score())
            {
                total += _four_of_a_kind->value();
                ++has_score_count;
            }
            if (_full_house->has_score())
            {
                total += _full_house->value();
                ++has_score_count;
            }
            if (_small_straight->has_score())
            {
                total += _small_straight->value();
                ++has_score_count;
            }
            if (_large_straight->has_score())
            {
                total += _large_straight->value();
                ++has_score_count;
            }
            if (_yahtzee->has_score())
            {
                total += _yahtzee->value();
                ++has_score_count;
            }
            if (_chance->has_score())
            {
                total += _chance->value();
                ++has_score_count;
            }

            if (has_score_count)
                _lower_section_total->set(total);
            else
                _lower_section_total->reset();
        }

        if (_lower_section_total->has_score() || _upper_section_total->has_score())
        {
            _combined_total->set(_upper_section_total->value_or(0) + _lower_section_total->value_or(0));
            _total->set(_combined_total->value() * _multiplier);
        }
        else
        {
            _combined_total->reset();
            _total->reset();
        }
    }

private:
    int     _multiplier;
    Score  *_aces;
    Score  *_twos;
    Score  *_threes;
    Score  *_fours;
    Score  *_fives;
    Score  *_sixes;
    Score  *_upper_sub_total;
    Score  *_bonus;
    Score  *_upper_total;
    Score  *_three_of_a_kind;
    Score  *_four_of_a_kind;
    Score  *_full_house;
    Score  *_small_straight;
    Score  *_large_straight;
    Score  *_yahtzee;
    Score  *_chance;
    Score  *_lower_section_total;
    Score  *_upper_section_total;
    Score  *_combined_total;
    Score  *_total;
};

#endif // SCORECOLUMN_H
