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

///
/// \brief Config::load Load the configuration settings and high scores
/// collection from the file.
///
/// The configuration data is stored in a JSON-format file.
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

///
/// \brief Config::save Store the configuration and high scores collection
/// to the file
///
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

///
/// \brief Config::add_high_score   Add a score to the collection of high scores.
/// \param score    The score to be added to the high scores collection
/// \param name     The name of the player that attained the score
/// \param datetime The data and time the score occurred
/// \return true if the score was successfully added, false otherwise.
///
/// It is usually not an error for this function to fail. The most likely cause
/// is that there is no more room in the collection for high scores as determined
/// by the configuration's setting for the maximum number of high scores allowed.
bool Config::add_high_score(int score, const QString &name, QDateTime datetime/* = QDateTime::currentDateTime()*/)
{
    if (_max_high_scores < 1)
        return false;

    auto pos = std::find_if(begin(_scores), end(_scores), [&score](const HighScore &hs) { return hs.score < score; });

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

    return true;
}
