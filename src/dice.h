#ifndef DICE_H
#define DICE_H

#include <array>
#include <random>

///
/// \brief The Dice class represents a set of five dice.
///
class Dice
{
public:
    ///
    /// \brief Construct a dice object.
    ///
    Dice()
      : _rd{}
      , _gen{_rd()}
      , _distr{1, 6}
      , _dice{1, 2, 3, 4, 5}
      , _selected{false, false, false, false, false}
    {}

    Dice(const Dice &) = delete;
    Dice(Dice &&) = delete;
    Dice &operator=(const Dice &) = delete;
    Dice &operator=(Dice &&) = delete;

    constexpr size_t size() const noexcept
    {
        return _dice.size();
    }

    const std::array<int, 5> &dice() const noexcept
    {
        return _dice;
    }
    const std::array<bool, 5> &selected() const noexcept
    {
        return _selected;
    }

    void select(size_t ndx, bool selected)
    {
        _selected[ndx] = selected;
    }
    bool is_selected(size_t ndx)
    {
        return _selected[ndx];
    }

    void roll()
    {
        for (size_t i{0}; i < _dice.size(); ++i)
            if (!is_selected(i))
                _dice[i] = _distr(_gen);
    }

    void reset()
    {
        for (size_t i{0}; i < _dice.size(); ++i)
        {
            _dice[i] = i + 1;
            _selected[i] = false;
        }
    }

private:
    std::random_device  _rd;
    std::mt19937        _gen;
    std::uniform_int_distribution<> _distr;
    std::array<int, 5>  _dice;
    std::array<bool, 5> _selected;
};

#endif // DICE_H
