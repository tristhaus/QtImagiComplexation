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

#if defined(_SKIP_LONG_TEST)
#elif defined(_USE_LONG_TEST)
#else
#error "you need to make a choice between using or skipping long tests, -D_USE_LONG_TEST -D_SKIP_LONG_TEST"
#endif

#include <QTest>
#include <QtTest>

#include "../Backend/basez.h"
#include "../Backend/constant.h"
#include "../Backend/product.h"

#include "../Frontend/mainwindow.h"
#include "../Frontend/mainwindow_ui.h"

class FrontendTest : public QObject
{
    Q_OBJECT

private:
    const int SingleShotInterval = 1000;

public:
    FrontendTest();

private slots:
    static void ConstructionShallWorkCompletely() ;
#ifdef _USE_LONG_TEST
    void AboutButtonShallTriggerDialogAndOKShallClose();
    static void ClickingPlotShallAddArrowWhenPossible();
    static void ClickingPlotShallNotAddArrowWhenImpossible();
    static void ClearButtonShallClearGraph();
#endif // _USE_LONG_TEST
};

FrontendTest::FrontendTest()
= default;

void FrontendTest::ConstructionShallWorkCompletely() //NOLINT(google-readability-function-size, hicpp-function-size, readability-function-size)
{
    try
    {
        // Act
        MainWindow mw;

        // Assert
        QVERIFY2(mw.ui->centralwidget, qPrintable(QString::fromUtf8(u8"not created central widget")));
        QVERIFY2(mw.ui->mainLayout, qPrintable(QString::fromUtf8(u8"not created main layout")));

        QVERIFY2(mw.ui->functionFrame, qPrintable(QString::fromUtf8(u8"not created function frame")));
        QVERIFY2(mw.ui->functionLayout, qPrintable(QString::fromUtf8(u8"not created function layout")));
        QVERIFY2(mw.ui->funcLabel, qPrintable(QString::fromUtf8(u8"not created function label")));
        QVERIFY2(mw.ui->funcLineEdit, qPrintable(QString::fromUtf8(u8"not created function line edit")));
        QVERIFY2(mw.ui->funcSetButton, qPrintable(QString::fromUtf8(u8"not created function set button")));
        QVERIFY2(mw.ui->funcClearButton, qPrintable(QString::fromUtf8(u8"not created function clear button")));
        QVERIFY2(mw.ui->aboutButton, qPrintable(QString::fromUtf8(u8"not created about button")));
        QVERIFY2(mw.ui->plot, qPrintable(QString::fromUtf8(u8"not created plot")));

    }
    catch (std::exception & ex)
    {
        QFAIL(ex.what());
    }
    catch (...)
    {
        QFAIL(u8"unknown exception");
    }
}

#ifdef _USE_LONG_TEST

void FrontendTest::AboutButtonShallTriggerDialogAndOKShallClose()
{
    // Arrange
    MainWindow mw;

    // spy needed such that events actually happen
    QSignalSpy spyAboutButton(mw.ui->aboutButton, &QAbstractButton::pressed);

    // Act
    bool aboutMessageBoxFound = false;
    bool aboutMessageBoxHasOneButton = false;
    QTimer::singleShot(this->SingleShotInterval, this, [&]()
    {
        aboutMessageBoxFound = mw.aboutMessageBox != nullptr;
        aboutMessageBoxHasOneButton = mw.aboutMessageBox->buttons().count() == 1 && mw.aboutMessageBox->buttons().first() != nullptr;
        if(aboutMessageBoxFound && aboutMessageBoxHasOneButton)
        {
            QTest::mouseClick(mw.aboutMessageBox->buttons().first(), Qt::LeftButton);
        }
    });

    QTest::mouseClick(mw.ui->aboutButton, Qt::LeftButton);

    // Assert
    QVERIFY2(aboutMessageBoxFound, qPrintable(QString::fromUtf8(u8"aboutMessageBox not found")));
    QVERIFY2(aboutMessageBoxHasOneButton, qPrintable(QString::fromUtf8(u8"aboutMessageBox does not have exactly one button")));

    QVERIFY2(mw.aboutMessageBox == nullptr, qPrintable(QString::fromUtf8(u8"aboutMessageBox still reachable")));
}

void FrontendTest::ClickingPlotShallAddArrowWhenPossible()
{
    // Arrange
    MainWindow mw;

    bool graphHasNoItems = mw.ui->plot->itemCount() == 0;

    // Act
    QTest::mouseClick(mw.ui->plot, Qt::LeftButton);

    bool graphHasOneItem = mw.ui->plot->itemCount() == 1;

    // Assert
    QVERIFY2(graphHasNoItems, qPrintable(QString::fromUtf8(u8"initially arrow found")));
    QVERIFY2(graphHasOneItem, qPrintable(QString::fromUtf8(u8"no arrow present after click")));
}

void FrontendTest::ClickingPlotShallNotAddArrowWhenImpossible()
{
    // Arrange
    MainWindow mw;

    // 1 / 0
    auto c1 = std::make_shared<Backend::Constant>(1.0);
    auto c2 = std::make_shared<Backend::Constant>(0.0);
    mw.expression = std::make_unique<Backend::Product>(std::vector<Backend::Product::Factor> {Backend::Product::Factor(Backend::Product::Exponent::Positive, c1), Backend::Product::Factor(Backend::Product::Exponent::Negative, c2)});

    bool graphHasNoItems = mw.ui->plot->itemCount() == 0;

    // Act
    QTest::mouseClick(mw.ui->plot, Qt::LeftButton);

    bool graphHasOneItem = mw.ui->plot->itemCount() == 0;

    // Assert
    QVERIFY2(graphHasNoItems, qPrintable(QString::fromUtf8(u8"initially arrow found")));
    QVERIFY2(graphHasOneItem, qPrintable(QString::fromUtf8(u8"arrow present after click")));
}

void FrontendTest::ClearButtonShallClearGraph()
{
    // Arrange
    MainWindow mw;
    mw.AddArrow(1.0, 1.0, 2.0, 2.0); //NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

    bool graphHasItems = mw.ui->plot->itemCount() > 0;

    //// spy needed such that events actually happen
    //QSignalSpy spyFuncClearButton(mw.ui->funcClearButton, &QAbstractButton::pressed);

    // Act
    QTest::mouseClick(mw.ui->funcClearButton, Qt::LeftButton);

    bool graphHasNoMoreItems = mw.ui->plot->itemCount() == 0;

    // Assert
    QVERIFY2(graphHasItems, qPrintable(QString::fromUtf8(u8"initial arrow not found")));
    QVERIFY2(graphHasNoMoreItems, qPrintable(QString::fromUtf8(u8"arrow present after clear")));
}

#endif // _USE_LONG_TEST

QTEST_MAIN(FrontendTest)

#include "tst_mainwindow.moc"
