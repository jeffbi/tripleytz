#include <QDateTime>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

#include <algorithm>
#include <iterator>
#include <vector>

#include "config.h"

namespace {
    constexpr const char *LastUsedName{"last_used_name"};
    constexpr const char *MaxHighScores{"max_high_scores"};
    constexpr const char *HighScores{"high_scores"};
    constexpr const char *Score{"score"};
    constexpr const char *When{"when"};
    constexpr const char *Name{"name"};
}

void Config::load()
{
    QFileInfo   fi(_path);

    if (fi.exists() && fi.isFile())
    {
        try
        {
            QFile   file{_path};

            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QByteArray      json{file.readAll()};
                QJsonParseError err;
                QJsonDocument   doc = QJsonDocument::fromJson(json, &err);

                if (err.error == QJsonParseError::NoError && doc.isObject())
                {
                    auto    obj{doc.object()};

                    if (obj.contains(LastUsedName) && obj[LastUsedName].isString())
                        _last_used_name = obj[LastUsedName].toString();
                    if (obj.contains(MaxHighScores) && obj[MaxHighScores].isDouble())
                        _max_high_scores = obj[MaxHighScores].toInt();
                    clear_high_scores();
                    if (obj.contains(HighScores) && obj[HighScores].isArray())
                    {
                        const QJsonArray  array = obj[HighScores].toArray();    // Avoid initializer-list constructor

                        for (int ndx{0}; ndx < array.size(); ++ndx)
                        {
                            QJsonObject elem{array[ndx].toObject()};

                            if (   elem.contains(Score) && elem[Score].isDouble()
                                && elem.contains(When) && elem[When].isString()
                                && elem.contains(Name) && elem[Name].isString())
                            {
                                _scores.emplace_back(elem[Score].toInt(),
                                                     QDateTime::fromString(elem[When].toString(), Qt::ISODate),
                                                     elem[Name].toString());
                            }
                        }
                    }
                }
            }
        }
        catch (...)
        {
            // do nothing. can't read the config data.
        }
    }
}

void Config::save() const
{
    try
    {
        QFile   file(_path);

        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            QJsonObject     obj;

            obj[LastUsedName] = _last_used_name;
            obj[MaxHighScores] = static_cast<int>(_max_high_scores);

            QJsonArray  scores;
            size_t  count = std::min(_scores.size(), _max_high_scores);
            for (size_t i{0}; i < count; ++i)
            {
                QJsonObject score;

                score[Score] = _scores[i].score;
                score[When] = _scores[i].when.toString(Qt::ISODate);
                score[Name] = _scores[i].name;

                scores.append(score);
            }

            obj[HighScores] = scores;

            file.write(QJsonDocument(obj).toJson());
        }
    }
    catch (...)
    {
        // do nothing. can't save the config data
    }
}

bool Config::try_add_high_score(int score, const QString &name, QDateTime datetime/* = QDateTime::currentDateTime()*/)
{
    //if (_max_high_scores < 1 || (!_scores.empty() && score <= _scores.back().score))
    //    return false;
    if (_max_high_scores < 1)
        return false;

#if 0
    if (_scores.size() < _max_high_scores)
    {
        _scores.emplace_back(score, datetime, name);
    }
    else
    {
        //TODO: Handle other cases!!!
    }

    // sort the vector
    std::sort(_scores.begin(), _scores.end(), [](const HighScore &a, const HighScore &b)
        {
            if (a.score == b.score)
                return a.when > b.when;
            return a.score > b.score;
        }
    );
#else
    auto is_less = [&score](const HighScore &hs) { return hs.score < score; };
    auto pos = std::find_if(begin(_scores), end(_scores), is_less);

    if (pos == end(_scores))
    {
        if (_scores.size() >= _max_high_scores)
            return false;
        _scores.emplace_back(score, datetime, name);
        return true;
    }

    _scores.emplace(pos, score, datetime, name);
    while (_scores.size() > _max_high_scores)   // should happen no more than once
        _scores.pop_back();
#endif

    return true;
}
