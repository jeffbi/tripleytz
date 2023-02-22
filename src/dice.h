#ifndef DICE_H
#define DICE_H

/**************************************************************************
* Copyright (c) 2023 by Jeff Bienstadt                                    *
*                                                                         *
* This file is part of the tripleytz project.                             *
*                                                                         *
* Released under the MIT license. See the file LICENSE for more details.  *
**************************************************************************/

#include <QObject>
#include <QThread>

#include <array>
#include <random>

///
/// \brief The Dice class represents a set of five dice.
///
class Dice : public QObject
{
    Q_OBJECT

public:
    ///
    /// \brief Construct a Dice object.
    ///
    Dice()
      : _rd{}
      , _gen{_rd()}
      , _distr{1, 6}
      , _bounces_distr{4, 20}
      , _dice{1, 2, 3, 4, 5}
      , _selected{false, false, false, false, false}
    {}

    Dice(const Dice &) = delete;
    Dice(Dice &&) = delete;
    Dice &operator=(const Dice &) = delete;
    Dice &operator=(Dice &&) = delete;

    ///
    /// \brief  Retrieve the number of dice
    /// \return The number of dice
    ///
    constexpr size_t size() const noexcept
    {
        return _dice.size();
    }

    ///
    /// \brief  Retrieve the collection of dice.
    /// \return A \c std::array of five integers representing the face value of each die.
    ///
    const std::array<int, 5> &dice() const noexcept
    {
        return _dice;
    }

    ///
    /// \brief  Mark a single die as selected.
    /// \param ndx      Zero-based index of the die to select,
    /// \param selected true to select, false to un-select.
    ///
    void select(size_t ndx, bool selected)
    {
        _selected[ndx] = selected;
    }
    ///
    /// \brief Determine whether a die is selected
    /// \param ndx  Zero-based index of the die to be checked
    /// \return true if the die is selected, false otherwise
    ///
    bool is_selected(size_t ndx)
    {
        return _selected[ndx];
    }

    ///
    /// \brief  Roll the dice.
    ///
    /// This function emits the \c on_die_changed signal as the face of each die
    /// changes, including during simulated "bounces".
    ///
    void roll()
    {
        std::array<int, 5>  bounces{0, 0, 0, 0, 0};

        for (size_t i{0}; i < _dice.size(); ++i)
            if (!is_selected(i))
                bounces[i] = _bounces_distr(_gen);

        while (bounces[0] || bounces[1] || bounces[2] || bounces[3] || bounces[4])
        {
            for (size_t i{0}; i < _dice.size(); ++i)
            {
                if (bounces[i])
                {
                    --bounces[i];
                    _dice[i] = _distr(_gen);
                    emit on_die_changed(i, _dice[i]);
                    QThread::msleep(10);
                }
            }
        }
    }

    ///
    /// \brief  Reset the dice.
    ///
    void reset()
    {
        for (size_t i{0}; i < _dice.size(); ++i)
        {
            _dice[i] = i + 1;
            _selected[i] = false;
        }
    }

signals:
    ///
    /// \brief  Signal indicating that a die's face has changed.
    /// \param index    Zero-based index of the changed die
    /// \param value    New face value of the die
    ///
    void on_die_changed(int index, int value);

private:
    std::random_device  _rd;
    std::mt19937        _gen;
    std::uniform_int_distribution<> _distr;
    std::uniform_int_distribution<> _bounces_distr;
    std::array<int, 5>  _dice;
    std::array<bool, 5> _selected;
};

#endif // DICE_H
