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

#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QStandardPaths>
#include <QTranslator>

#include "config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString   baseName = "TripleYtz_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }

    Config  config{QStandardPaths::writableLocation(QStandardPaths::StandardLocation::GenericConfigLocation) + "/.tripleytz"};
    config.load();
    MainWindow w(config);
    w.show();
    return a.exec();
}
