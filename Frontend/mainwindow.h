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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <memory>

#include "../Backend/expression.h"
#include "../Backend/parser.h"

class FrontendTest;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow //NOLINT(cppcoreguidelines-special-member-functions)
{
    friend FrontendTest;

    Q_OBJECT

private:
    const double viewport = 10.0;
    const int minHue = 0;
    const int maxHue = 359;
    const int minSaturation = 150;
    const int maxSaturation = 255;
    const int minValue = 180;
    const int maxValue = 240;

    QPalette parseablePalette;
    QPalette nonParseablePalette;

    bool plotting;
    Ui::MainWindow * ui;
    Backend::Parser parser;
    std::unique_ptr<QMessageBox> aboutMessageBox;
    std::shared_ptr<Backend::Expression> expression;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;
    ~MainWindow() override;

private slots:
    void OnPlotClick(QMouseEvent * event);
    void OnFuncLineEditTextChanged();
    void OnReturnKeyPressed();
    void OnSetPressed();
    void OnClearPressed();
    void OnAboutPressed();

private:
    void UpdateUiState();
    void UpdateParseability();
    void UpdateExpression();
    void ClearPlot();
    [[nodiscard]] QColor GenerateColor() const;
    void AddArrow(double startX, double startY, double endX, double endY);
    void ShowAboutDialog();
};

#endif // MAINWINDOW_H
