#ifndef CONFIG_H
#define CONFIG_H

#include <QDateTime>
#include <QString>

#include <vector>

class Config
{
public:
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

    explicit Config(const QString &path)
      : _path(path)
    {}

    void load();
    void save() const;

    QString last_used_name() const
    {
        return _last_used_name;
    }
    void last_used_name(const QString &name)
    {
        _last_used_name = name;
    }

    void clear_high_scores()
    {
        _scores.clear();
    }

    const std::vector<HighScore> &hi_scores() const noexcept
    {
        return _scores;
    }

    bool is_high_score(int score)
    {
        if (_scores.empty() || score > _scores.back().score)
            return true;

        //TODO: Do we need more checking?
        return false;
    }

    bool try_add_high_score(int score, const QString &name, QDateTime datetime = QDateTime::currentDateTime());

private:
    QString                 _path;
    QString                 _last_used_name;
    size_t                  _max_high_scores{20};
    std::vector<HighScore>  _scores;
};

#endif // CONFIG_H
