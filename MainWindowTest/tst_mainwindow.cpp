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
    static void WindowShallBeStateful();
    static void ClickingPlotShallAddArrowWhenPossible();
    static void ClickingPlotShallNotAddArrowWhenImpossible();
    static void ClearButtonShallClearGraph();
    static void ParseabilityShallBeCorrectlyIndicated();
    static void ReturnKeyOnParseableInputShallActivatePlotting();
    void GridAdditionShallAddArrows();
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
        QVERIFY2(mw.ui->gridButton, qPrintable(QString::fromUtf8(u8"not created grid button")));
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

void FrontendTest::WindowShallBeStateful()
{
    // Arrange
    MainWindow mw;
    mw.ui->funcLineEdit->setText(QString("z"));

    // Act
    QTest::mouseClick(mw.ui->funcSetButton, Qt::LeftButton);
    bool funcIsDisabledAfterSet = !mw.ui->funcLineEdit->isEnabled();
    bool gridIsEnabledAfterSet = mw.ui->gridButton->isEnabled();

    QTest::mouseClick(mw.ui->plot, Qt::LeftButton);
    QTest::mouseClick(mw.ui->plot, Qt::LeftButton);
    bool graphHasTwoItems = mw.ui->plot->itemCount() == 2;

    QTest::mouseClick(mw.ui->funcClearButton, Qt::LeftButton);
    bool funcIsEnabledAfterClear = mw.ui->funcLineEdit->isEnabled();
    bool gridIsDisabledAfterClear = !mw.ui->gridButton->isEnabled();
    bool graphHasNoItem = mw.ui->plot->itemCount() == 0;

    QTest::mouseClick(mw.ui->plot, Qt::LeftButton);
    bool graphStillHasNoItem = mw.ui->plot->itemCount() == 0;

    // Assert
    QVERIFY2(funcIsDisabledAfterSet, qPrintable(QString::fromUtf8(u8"line edit enabled after set")));
    QVERIFY2(gridIsEnabledAfterSet, qPrintable(QString::fromUtf8(u8"grid button not enabled after set")));
    QVERIFY2(graphHasTwoItems, qPrintable(QString::fromUtf8(u8"graph does not have expected items")));
    QVERIFY2(funcIsEnabledAfterClear, qPrintable(QString::fromUtf8(u8"line edit not enabled after clear")));
    QVERIFY2(gridIsDisabledAfterClear, qPrintable(QString::fromUtf8(u8"grid button enabled after clear")));
    QVERIFY2(graphHasNoItem, qPrintable(QString::fromUtf8(u8"graph has item after clear")));
    QVERIFY2(graphStillHasNoItem, qPrintable(QString::fromUtf8(u8"graph has item after clicking")));
}

void FrontendTest::ClickingPlotShallAddArrowWhenPossible()
{
    // Arrange
    MainWindow mw;

    mw.ui->funcLineEdit->setText(QString::fromUtf8(u8"z * i"));

    bool graphHasNoItems = mw.ui->plot->itemCount() == 0;

    // Act
    QTest::mouseClick(mw.ui->funcSetButton, Qt::LeftButton);
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

    mw.ui->funcLineEdit->setText(QString::fromUtf8(u8"1.0 / 0.0"));

    bool graphHasNoItems = mw.ui->plot->itemCount() == 0;

    // Act
    QTest::mouseClick(mw.ui->funcSetButton, Qt::LeftButton);
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

    mw.ui->funcLineEdit->setText(QString::fromUtf8(u8"z * i"));

    QTest::mouseClick(mw.ui->funcSetButton, Qt::LeftButton);
    QTest::mouseClick(mw.ui->plot, Qt::LeftButton);

    bool graphHasItems = mw.ui->plot->itemCount() > 0;

    // Act
    QTest::mouseClick(mw.ui->funcClearButton, Qt::LeftButton);

    bool graphHasNoMoreItems = mw.ui->plot->itemCount() == 0;

    // Assert
    QVERIFY2(graphHasItems, qPrintable(QString::fromUtf8(u8"initial arrow not found")));
    QVERIFY2(graphHasNoMoreItems, qPrintable(QString::fromUtf8(u8"arrow present after clear")));
}

void FrontendTest::ParseabilityShallBeCorrectlyIndicated()
{
    // Arrange
    MainWindow mw;
    QColor lightPink = QColor(0xFF, 0xB6, 0xC1);

    // spy needed such that events actually happen
    QSignalSpy spyFuncLineEdit(mw.ui->funcLineEdit, &QLineEdit::textChanged);

    // Act
    mw.ui->funcLineEdit->setText(QString::fromUtf8(u8"z z"));

    auto unparseablePalette = mw.ui->funcLineEdit->palette();

    mw.ui->funcLineEdit->setText(QString::fromUtf8(u8"z * z"));

    auto parseablePalette = mw.ui->funcLineEdit->palette();

    // Assert
    QVERIFY2(unparseablePalette.base().color() == lightPink, qPrintable(QString::fromUtf8(u8"parseable when it should not be")));
    QVERIFY2(parseablePalette.base().color() == Qt::white, qPrintable(QString::fromUtf8(u8"not parseable when it should be")));
}

void FrontendTest::ReturnKeyOnParseableInputShallActivatePlotting()
{
    // Arrange
    MainWindow mw;
    mw.ui->funcLineEdit->setText(QString(""));

    QString function = "z*(z)\r";

    // Act
    QTest::keyClicks(mw.ui->funcLineEdit, function);

    // Assert
    QVERIFY2(mw.plotting, qPrintable(QString::fromUtf8(u8"not plotting when it should be")));
}

void FrontendTest::GridAdditionShallAddArrows()
{
    // Arrange
    MainWindow mw;
    mw.ui->funcLineEdit->setText(QString("z * i"));
    QTest::mouseClick(mw.ui->funcSetButton, Qt::LeftButton);

    // spy needed such that events actually happen
    QSignalSpy spyGridButton(mw.ui->gridButton, &QAbstractButton::pressed);

    // Act
    int preCount = mw.ui->plot->itemCount();

    bool gridDialogFound = false;
    QTimer::singleShot(500, this, [&]()
    {
        gridDialogFound = mw.gridDialog != nullptr;
        if(gridDialogFound)
        {
            QTest::mouseClick(mw.gridDialog->squareGridAcceptButton, Qt::LeftButton);
        }
    });

    QTest::mouseClick(mw.ui->gridButton, Qt::LeftButton);

    Sleep(1000);

    int postCount = mw.ui->plot->itemCount();

    // Assert
    QVERIFY2(gridDialogFound, qPrintable(QString::fromUtf8(u8"grid dialog not found")));
    QVERIFY2(mw.gridDialog == nullptr, qPrintable(QString::fromUtf8(u8"grid dialog still reachable")));
    QVERIFY2(preCount == 0, qPrintable(QString::fromUtf8(u8"preCount not equal 0")));
    QVERIFY2(postCount > 0, qPrintable(QString::fromUtf8(u8"postCount not greater 0")));
}

#endif // _USE_LONG_TEST

QTEST_MAIN(FrontendTest)

#include "tst_mainwindow.moc"
