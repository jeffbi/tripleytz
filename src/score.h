#ifndef SCORE_H
#define SCORE_H

#include <QPushButton>
#include <QWidget>

#include <optional>

///
/// \brief  The Score class implements a single score space on the score sheet,
///         both active spaces such as Ones or Full House, and passive spaces
///         such as Bonus or totals.
///
class Score : public QPushButton
{
    Q_OBJECT

public:
    ///
    /// \brief  Constructs a Score widget.
    /// \param parent   Pointer to the parent widget
    ///
    explicit Score(QWidget *parent = nullptr)
      : QPushButton(parent)
    {}

    ///
    /// Destroys a Score widget.
    ~Score()
    {}

    ///
    /// \brief  Determines if a Score widget contains a score.
    /// \return True if the widget contains a score, false otherwise.
    ///
    bool has_score() const noexcept
    {
        return _score.has_value();
    }

    ///
    /// \brief  Return the score's value. The score must have a score value.
    /// \return The score value.
    ///
    int value() const
    {
        return _score.value();
    }

    ///
    /// \brief  Set the score value.
    /// \param value    The value to be set.
    ///
    void set(int value)
    {
        _previewing = false;
        _score = value;
        update();
    }

    ///
    /// \brief  Clears the score value. The has_value function returns false after calling this function.
    ///
    void reset()
    {
        _score.reset();
        _previewing = false;
        update();
    }

    ///
    /// \brief  Set the score in preview mode, allowing it to be changed.
    /// \param value    The new value of the score
    ///
    void preview_score(int value)
    {
        _previewing = true;
        //setText(QString::number(value));
        //set(value);
        _score = value;
        update();
    }
    ///
    /// \brief  Determine if the score object is in preview mode.
    /// \return True if the score object is in preview mode, false otherwise.
    ///
    bool previewing()
    {
        return _previewing;
    }

signals:
    ///
    /// \brief  Signal to indicate the mouse has entered a Score widget.
    /// \param score    A pointer to the Score widget that was entered.
    ///
    void on_enter(Score *score);
    ///
    /// \brief  Signal to indicate the mouse has exited a Score widget.
    /// \param score    A pointer to the Score widget that was left.
    ///
    void on_leave(Score *score);
    ///
    /// \brief  Signal to indicate a score has changed
    /// \param score    A pointer to the Score widget that changed.
    ///
    void on_changed(Score *score);

private:
    ///
    /// \brief  Update the text of the widget with the current score value if any.
    ///
    void update()
    {
        static QString empty{""};

        setText(has_score() ? QString::number(_score.value()) : empty);
        emit on_changed(this);
    }

    //
    // Events
    //
private:
    ///
    /// \brief  Event fired when the mouse enters the score widget's area.
    ///         Emits the on_enter signal.
    /// \param event    Pointer to a QEnterEvent. This parameter is not used.
    ///
    void enterEvent([[maybe_unused]]QEnterEvent *event)
    {
        emit on_enter(this);
    }
    ///
    /// \brief  Event fired when the mouse leaves the score widget's area.
    ///         Emits the on_leave signal.
    /// \param event    Pointer to a QEvent. This parameter is not used.
    ///
    void leaveEvent([[maybe_unused]]QEvent *event)
    {
        emit on_leave(this);
    }

private:
    std::optional<int>  _score;
    bool                _previewing{false};
};

#endif // SCORE_H
