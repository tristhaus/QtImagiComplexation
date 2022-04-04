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

class Ui_MainWindow
{
    friend MainWindow;

protected:
    explicit Ui_MainWindow() //NOLINT(cppcoreguidelines-pro-type-member-init)
    {
    }

private:

    QWidget *centralwidget{};
    QHBoxLayout *centralLayout{};

    QLabel *helloWorldLabel{};

public:
    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
        {
            MainWindow->setObjectName(QString::fromUtf8(u8"MainWindow"));
        }

        MainWindow->resize(1200, 800); //NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

        QSizePolicy sizePolicyExpExp(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicyExpExp.setHorizontalStretch(0);
        sizePolicyExpExp.setVerticalStretch(0);

        QSizePolicy sizePolicyPreFix(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicyPreFix.setHorizontalStretch(0);
        sizePolicyPreFix.setVerticalStretch(0);

        QSizePolicy sizePolicyFixPre(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicyFixPre.setHorizontalStretch(0);
        sizePolicyFixPre.setVerticalStretch(0);

        QSizePolicy sizePolicyFixFix(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicyFixFix.setHorizontalStretch(0);
        sizePolicyFixFix.setVerticalStretch(0);

        QSizePolicy sizePolicyMinMin(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicyMinMin.setHorizontalStretch(0);
        sizePolicyMinMin.setVerticalStretch(0);

        centralwidget = new QWidget(MainWindow); //NOLINT(cppcoreguidelines-owning-memory
        centralwidget->setObjectName(QString::fromUtf8(u8"centralwidget"));
        sizePolicyExpExp.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicyExpExp);

        centralLayout = new QHBoxLayout(centralwidget); //NOLINT(cppcoreguidelines-owning-memory
        centralLayout->setObjectName(QString::fromUtf8(u8"centralLayout"));

        helloWorldLabel = new QLabel(centralwidget); //NOLINT(cppcoreguidelines-owning-memory
        helloWorldLabel->setObjectName(QString::fromUtf8(u8"helloWorldLabel"));
        centralLayout->addWidget(helloWorldLabel);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", u8"QtImagiComplexation", nullptr));

        helloWorldLabel->setText(QCoreApplication::translate("MainWindow", u8"Hello World!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow
    {
    public:
        explicit MainWindow()
        {
        }
    };
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
