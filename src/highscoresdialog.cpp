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

#include <QHBoxLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "config.h"
#include "highscoresdialog.h"

#include <vector>

///
/// \brief HighScoresDialog::HighScoresDialog Construct a \c HighScoresDilog
/// \param config   Reference to a \c Config object.
/// \param parent   Pointer to a \c QWidget object to be the parent of the dialog
///
HighScoresDialog::HighScoresDialog(Config &config, QWidget *parent)
  : QDialog(parent)
  , _tree{new QTreeWidget()}
  , _clear{new QPushButton(tr("&Clear"))}
  , _ok{new QPushButton(tr("&Ok"))}
  , _config(config)
{
    _tree->setColumnCount(4);
    _tree->setHeaderLabels(QStringList{} << "Rank" << "Name" << "Score" << "When");

    populate_tree();

    _ok->setDefault(true);

    QHBoxLayout *btn_layout{new QHBoxLayout};
    btn_layout->addWidget(_clear);
    btn_layout->addWidget(_ok);

    QVBoxLayout *main_layout{new QVBoxLayout};
    main_layout->addWidget(_tree);
    main_layout->addLayout(btn_layout);
    setLayout(main_layout);

    resize(480, 420);

    connect(_ok, &QPushButton::clicked, this, &HighScoresDialog::accept);
    connect(_clear, &QPushButton::clicked, this, &HighScoresDialog::clear_clicked);
}

///
/// \brief HighScoresDialog::populate_tree  Fill the display widget with high score data.
///
void HighScoresDialog::populate_tree()
{
    _tree->clear();
    int rank{1};
    for (const auto &hs : _config.hi_scores())
    {
        QTreeWidgetItem *item{new QTreeWidgetItem{_tree}};
        item->setText(0, QString::number(rank++));
        item->setText(1, hs.name);
        item->setText(2, QString::number(hs.score));
        item->setText(3, hs.when.toString("yyyy/MM/dd - hh:mm"));
    }
}

void HighScoresDialog::clear_clicked()
{
    _config.clear_high_scores();
    populate_tree();
    _config.save();
}
