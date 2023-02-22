#ifndef CONFIG_H
#define CONFIG_H

#include <QDateTime>
#include <QString>

#include <vector>

///
/// \brief Contains configuratino information including a collection of high scores.
///
class Config
{
public:
    ///
    /// \brief Represents a high score including the players name and when it occurred
    ///
    struct HighScore
    {
        int         score;
        QDateTime   when;
        QString     name;

        HighScore(int score, const QDateTime &dt, const QString &name)
          : score{score}
          , when{dt}
          , name{name}
         {}
    };

    ///
    /// \brief Construct a Config object with a string containing the location of the configuration file.
    /// \param path A reference to a QString containing the location of the configuration file.
    ///
    explicit Config(const QString &path)
      : _path(path)
    {}

    void load();
    void save() const;

    ///
    /// \brief last_used_name   Retrieve the last name used for high score.
    /// \return A QString containing the name last used for high score.
    ///
    QString last_used_name() const
    {
        return _last_used_name;
    }
    ///
    /// \brief last_used_name   Set the last name used for high score.
    /// \param name A reference to a QString containing the newest name used for high score.
    ///
    void last_used_name(const QString &name)
    {
        _last_used_name = name;
    }

    ///
    /// \brief clear_high_scores    Clear the collection of high scores.
    ///
    void clear_high_scores()
    {
        _scores.clear();
    }

    ///
    /// \brief hi_scores    Retrieve the high scores collection.
    /// \return A const reference to a \c std::vector of \c HighScore structures.
    ///
    const std::vector<HighScore> &hi_scores() const noexcept
    {
        return _scores;
    }

    ///
    /// \brief is_high_score    Determine if a given score is a high score.
    /// \param score    The score value to be checked.
    /// \return true if the score is a high score, false otherwise.
    ///
    bool is_high_score(int score)
    {
        if (_scores.empty() || _scores.size() < _max_high_scores || score > _scores.back().score)
            return true;

        return false;
    }

    bool add_high_score(int score, const QString &name, QDateTime datetime = QDateTime::currentDateTime());

private:
    QString                 _path;
    QString                 _last_used_name;
    size_t                  _max_high_scores{20};
    std::vector<HighScore>  _scores;
};

#endif // CONFIG_H
