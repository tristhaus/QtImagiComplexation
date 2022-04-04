/*
 * This file is part of QtImagiComplexation.
 *
 * QtImagiComplexation is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QtImagiComplexation is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QtImagiComplexation.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../Frontend/mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    QLocale presentLocale = QLocale::system();
    auto uiLanguages = presentLocale.uiLanguages();

    // add default
    if(!uiLanguages.contains(QString("en")))
    {
        uiLanguages.append(QString("en"));
    }

    // iterate over possible languages
    const QString filenameTemplate(":/QtImagiComplexation_%1.qm");
    auto langIt = uiLanguages.begin();
    auto langEnd = uiLanguages.end();
    bool hasLoaded = false;
    for(; langIt != langEnd && !hasLoaded; ++langIt)
    {
        auto filename = filenameTemplate.arg(*langIt).replace("-", "_");
        hasLoaded = translator.load(filename);
    }

    if(!hasLoaded)
    {
        return 1;
    }

    QApplication::installTranslator(&translator);

    MainWindow w;
    w.show();
    return QApplication::exec();
}
