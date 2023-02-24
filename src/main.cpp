/**************************************************************************
* Copyright (c) 2023 by Jeff Bienstadt                                    *
*                                                                         *
* This file is part of the tripleytz project.                             *
*                                                                         *
* Released under the MIT license. See the file LICENSE for more details.  *
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
