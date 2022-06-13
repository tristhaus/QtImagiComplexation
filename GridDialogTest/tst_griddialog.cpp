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

#include "../Frontend/griddialog.h"

class FrontendTest : public QObject
{
    Q_OBJECT

public:
    FrontendTest();

private slots:
    static void ConstructionShallWorkCompletely() ;
#ifdef _USE_LONG_TEST
    static void CancelShallReject();
    static void ValuesOutOfRangeAreNotAllowed();
    static void AddingSquareGridShallYieldCorrectData();
    static void AddingRCAGridShallYieldCorrectData();
    static void AddingRADGridShallYieldCorrectData();
#endif // _USE_LONG_TEST
};

FrontendTest::FrontendTest()
= default;

void FrontendTest::ConstructionShallWorkCompletely() //NOLINT(google-readability-function-size, hicpp-function-size, readability-function-size)
{
    try
    {
        // Act
        Ui::GridDialog gd(nullptr, 10.0, 10.0);

        // Assert
        QVERIFY2(gd.verticalDialogLayout, qPrintable(QString::fromUtf8(u8"not created dialog layout")));

        QVERIFY2(gd.squareGridGroupBox, qPrintable(QString::fromUtf8(u8"not created square grid group box")));
        QVERIFY2(gd.squareGridLayout, qPrintable(QString::fromUtf8(u8"not created square grid layout")));
        QVERIFY2(gd.squareGridDistLabel, qPrintable(QString::fromUtf8(u8"not created square grid dist label")));
        QVERIFY2(gd.squareGridDistSpinBox, qPrintable(QString::fromUtf8(u8"not created square grid dist spinbox")));
        QVERIFY2(gd.squareGridAcceptButton, qPrintable(QString::fromUtf8(u8"not created square grid accept button")));

        QVERIFY2(gd.rcaGridGroupBox, qPrintable(QString::fromUtf8(u8"not created RCA grid group box")));
        QVERIFY2(gd.rcaGridLayout, qPrintable(QString::fromUtf8(u8"not created RCA grid layout")));
        QVERIFY2(gd.rcaGridRadialLabel, qPrintable(QString::fromUtf8(u8"not created RCA grid radial label")));
        QVERIFY2(gd.rcaGridRadialSpinBox, qPrintable(QString::fromUtf8(u8"not created RCA grid radial spinbox")));
        QVERIFY2(gd.rcaGridAngleLabel, qPrintable(QString::fromUtf8(u8"not created RCA grid angle label")));
        QVERIFY2(gd.rcaGridAngleSpinBox, qPrintable(QString::fromUtf8(u8"not created RCA grid angle spinbox")));
        QVERIFY2(gd.rcaGridAcceptButton, qPrintable(QString::fromUtf8(u8"not created RCA grid accept button")));

        QVERIFY2(gd.radGridGroupBox, qPrintable(QString::fromUtf8(u8"not created RAD grid group box")));
        QVERIFY2(gd.radGridLayout, qPrintable(QString::fromUtf8(u8"not created RAD grid layout")));
        QVERIFY2(gd.radGridDistLabel, qPrintable(QString::fromUtf8(u8"not created RAD grid dist label")));
        QVERIFY2(gd.radGridDistSpinBox, qPrintable(QString::fromUtf8(u8"not created RAD grid dist spin box")));
        QVERIFY2(gd.radGridAcceptButton, qPrintable(QString::fromUtf8(u8"not created RAD grid accept button")));

        QVERIFY2(gd.cancelButton, qPrintable(QString::fromUtf8(u8"not created cancel button")));
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

void FrontendTest::CancelShallReject()
{
    // Arrange
    Ui::GridDialog gd(nullptr, 10.0, 10.0);

    // Act
    QTest::mouseClick(gd.cancelButton, Qt::LeftButton);

    auto result = gd.result();

    // Assert
    QVERIFY2(result == QDialog::DialogCode::Rejected, qPrintable(QString::fromUtf8(u8"dialog not rejected")));
}

void FrontendTest::ValuesOutOfRangeAreNotAllowed()
{
    // Arrange
    Ui::GridDialog gd(nullptr, 10.0, 10.0);

    // Act
    gd.squareGridDistSpinBox->setValue(0.1);
    double squareGridDistLowValue = gd.squareGridDistSpinBox->value();
    gd.squareGridDistSpinBox->setValue(6.0);
    double squareGridDistHighValue = gd.squareGridDistSpinBox->value();

    gd.rcaGridRadialSpinBox->setValue(0.1);
    double rcaGridRadialLowValue = gd.rcaGridRadialSpinBox->value();
    gd.rcaGridRadialSpinBox->setValue(6.0);
    double rcaGridRadialHighValue = gd.rcaGridRadialSpinBox->value();
    gd.rcaGridAngleSpinBox->setValue(2);
    int rcaGridAngleLowValue = gd.rcaGridAngleSpinBox->value();
    gd.rcaGridAngleSpinBox->setValue(183);
    int rcaGridAngleHighValue = gd.rcaGridAngleSpinBox->value();

    gd.radGridDistSpinBox->setValue(0.1);
    double radGridDistLowValue = gd.radGridDistSpinBox->value();
    gd.radGridDistSpinBox->setValue(6.0);
    double radGridDistHighValue = gd.radGridDistSpinBox->value();

    // Assert
    QVERIFY2(squareGridDistLowValue > 0.1, qPrintable(QString::fromUtf8(u8"low value found")));
    QVERIFY2(squareGridDistHighValue < 6.0, qPrintable(QString::fromUtf8(u8"high value found")));

    QVERIFY2(rcaGridRadialLowValue > 0.1, qPrintable(QString::fromUtf8(u8"low value found")));
    QVERIFY2(rcaGridRadialHighValue < 6.0, qPrintable(QString::fromUtf8(u8"high value found")));
    QVERIFY2(rcaGridAngleLowValue > 2, qPrintable(QString::fromUtf8(u8"low value found")));
    QVERIFY2(rcaGridAngleHighValue < 183, qPrintable(QString::fromUtf8(u8"high value found")));

    QVERIFY2(radGridDistLowValue > 0.1, qPrintable(QString::fromUtf8(u8"low value found")));
    QVERIFY2(radGridDistHighValue < 6.0, qPrintable(QString::fromUtf8(u8"high value found")));
}

void FrontendTest::AddingSquareGridShallYieldCorrectData()
{
    // Arrange
    Ui::GridDialog gd(nullptr, 10.0, 10.0);

    // Act
    gd.squareGridDistSpinBox->setValue(1.0);
    QTest::mouseClick(gd.squareGridAcceptButton, Qt::LeftButton);

    auto result = gd.GetResult();

    // Assert
    QVERIFY2(result.size() == 441, qPrintable(QString::fromUtf8(u8"incorrect grid found")));
}

void FrontendTest::AddingRCAGridShallYieldCorrectData()
{
    // Arrange
    Ui::GridDialog gd(nullptr, 10.0, 10.0);

    // Act
    gd.rcaGridRadialSpinBox->setValue(1.0);
    gd.rcaGridAngleSpinBox->setValue(15);
    QTest::mouseClick(gd.rcaGridAcceptButton, Qt::LeftButton);

    auto result = gd.GetResult();

    // Assert
    QVERIFY2(result.size() == 265, qPrintable(QString::fromUtf8(u8"incorrect grid found")));
}

void FrontendTest::AddingRADGridShallYieldCorrectData()
{
    // Arrange
    Ui::GridDialog gd(nullptr, 10.0, 10.0);

    // Act
    gd.radGridDistSpinBox->setValue(1.5);
    QTest::mouseClick(gd.radGridAcceptButton, Qt::LeftButton);

    auto result = gd.GetResult();

    // Assert
    QVERIFY2(result.size() == 171, qPrintable(QString::fromUtf8(u8"incorrect grid found")));
}

#endif // _USE_LONG_TEST

QTEST_MAIN(FrontendTest)

#include "tst_griddialog.moc"
