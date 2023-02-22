#ifndef GAMESCORER_H
#define GAMESCORER_H

/**************************************************************************
* Copyright (c) 2023 by Jeff Bienstadt                                    *
*                                                                         *
* This file is part of the tripleytz project.                             *
*                                                                         *
* Released under the MIT license. See the file LICENSE for more details.  *
**************************************************************************/

#include <array>

#include "dice.h"

class GameScorer
{
public:
    GameScorer(const Dice &dice)
      : _pip_counts{0/*, 0, 0, 0, 0, 0*/}
    {
        for (const auto die : dice.dice())
            ++_pip_counts[die - 1];
    }

    int aces() const noexcept
    {
        return _pip_counts[0];
    }
    int twos() const noexcept
    {
        return _pip_counts[1] * 2;
    }
    int threes() const noexcept
    {
        return _pip_counts[2] * 3;
    }
    int fours() const noexcept
    {
        return _pip_counts[3] * 4;
    }
    int fives() const noexcept
    {
        return _pip_counts[4] * 5;
    }
    int sixes() const noexcept
    {
        return _pip_counts[5] * 6;
    }
    int three_of_a_kind() const noexcept
    {
        for (auto c : _pip_counts)
            if (c >= 3)
                return sum_of_pips();

        return 0;
    }
    int four_of_a_kind() const noexcept
    {
        for (auto c : _pip_counts)
            if (c >= 4)
                return sum_of_pips();

        return 0;
    }
    int full_house() const noexcept
    {
        bool three{false};

        for (auto c : _pip_counts)
            if (c == 3)
            {
                three = true;
                break;
            }
        if (three)
            for (auto c : _pip_counts)
                if (c == 2)
                    return 25;

        return 0;
    }
    int small_straight() const noexcept
    {
        if (   (_pip_counts[0] >= 1 && _pip_counts[1] >= 1 && _pip_counts[2] >= 1 && _pip_counts[3] >= 1)
            || (_pip_counts[1] >= 1 && _pip_counts[2] >= 1 && _pip_counts[3] >= 1 && _pip_counts[4] >= 1)
            || (_pip_counts[2] >= 1 && _pip_counts[3] >= 1 && _pip_counts[4] >= 1 && _pip_counts[5] >= 1))
            return 30;
        //else if (m_yahtzee_joker)
        //    return 30;
        return 0;
    }
    int large_straight() const noexcept
    {
        if (   (_pip_counts[0] == 1 && _pip_counts[1] == 1 && _pip_counts[2] == 1 && _pip_counts[3] == 1 && _pip_counts[4] == 1)
            || (_pip_counts[1] == 1 && _pip_counts[2] == 1 && _pip_counts[3] == 1 && _pip_counts[4] == 1 && _pip_counts[5] == 1))
            return 40;
        //else if (m_yahtzee_joker)
        //    return 40;
        return 0;
    }
    int yahtzee() const noexcept
    {
        for (auto c : _pip_counts)
            if (c == 5)
                return 50;

        return 0;
    }
    int chance() const noexcept
    {
        return sum_of_pips();
    }
private:
    int sum_of_pips() const noexcept
    {
        return   (_pip_counts[0])
               + (_pip_counts[1] * 2)
               + (_pip_counts[2] * 3)
               + (_pip_counts[3] * 4)
               + (_pip_counts[4] * 5)
               + (_pip_counts[5] * 6);
    }

private:
    std::array<int, 6>  _pip_counts;
};

#endif // GAMESCORER_H
