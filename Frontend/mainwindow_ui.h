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

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include "qcustomplot.h"

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class MainWindow;
class FrontendTest;

class Ui_MainWindow
{
    friend MainWindow;
    friend FrontendTest;

    Q_DECLARE_TR_FUNCTIONS(Ui_MainWindow)

protected:
    explicit Ui_MainWindow() = default; //NOLINT(cppcoreguidelines-pro-type-member-init)

private:

    QWidget * centralwidget{};
    QVBoxLayout * mainLayout{};

    QFrame * functionFrame{};
    QHBoxLayout * functionLayout{};

    QLabel * funcLabel{};
    QLineEdit * funcLineEdit{};
    QPushButton *funcSetButton{};
    QPushButton *funcClearButton{};
    QPushButton *aboutButton{};

    QCustomPlot * plot{};

public:
    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
        {
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        }
        MainWindow->resize(600, 621);
        centralwidget = new QWidget(MainWindow); //NOLINT(cppcoreguidelines-owning-memory)
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        mainLayout = new QVBoxLayout(centralwidget); //NOLINT(cppcoreguidelines-owning-memory)
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));

        functionFrame = new QFrame(centralwidget); //NOLINT(cppcoreguidelines-owning-memory)
        functionFrame->setObjectName(QString::fromUtf8("functionFrame"));
        functionFrame->setFrameShape(QFrame::StyledPanel);
        functionFrame->setFrameShadow(QFrame::Raised);

        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(functionFrame->sizePolicy().hasHeightForWidth());
        functionFrame->setSizePolicy(sizePolicy);

        functionLayout = new QHBoxLayout(functionFrame); //NOLINT(cppcoreguidelines-owning-memory)
        functionLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

        funcLabel = new QLabel(functionFrame); //NOLINT(cppcoreguidelines-owning-memory)
        funcLabel->setObjectName(QString::fromUtf8("funcLabel"));
        functionLayout->addWidget(funcLabel);

        funcLineEdit = new QLineEdit(functionFrame); //NOLINT(cppcoreguidelines-owning-memory)
        funcLineEdit->setObjectName(QString::fromUtf8("funcLineEdit"));
        functionLayout->addWidget(funcLineEdit);

        funcSetButton = new QPushButton(functionFrame); //NOLINT(cppcoreguidelines-owning-memory)
        funcSetButton->setObjectName(QString::fromUtf8("funcSetButton"));
        functionLayout->addWidget(funcSetButton);

        funcClearButton = new QPushButton(functionFrame); //NOLINT(cppcoreguidelines-owning-memory)
        funcClearButton->setObjectName(QString::fromUtf8("funcClearButton"));
        functionLayout->addWidget(funcClearButton);

        aboutButton = new QPushButton(functionFrame); //NOLINT(cppcoreguidelines-owning-memory)
        aboutButton->setObjectName(QString::fromUtf8("funcAboutButton"));
        functionLayout->addWidget(aboutButton);

        mainLayout->addWidget(functionFrame);

        plot = new QCustomPlot(centralwidget); //NOLINT(cppcoreguidelines-owning-memory)
        plot->setObjectName(QString::fromUtf8("plot"));

        mainLayout->addWidget(plot);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "QtImagiComplexation", nullptr));
        funcLabel->setText(QCoreApplication::translate("MainWindow", "f(z) =", nullptr));
        funcSetButton->setText(QCoreApplication::translate("MainWindow", "Set", nullptr));
        funcClearButton->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        aboutButton->setText(QCoreApplication::translate("MainWindow", "About ... ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow
    {

    Q_DECLARE_TR_FUNCTIONS(MainWindow)

    public:
        explicit MainWindow() = default;
    };
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
