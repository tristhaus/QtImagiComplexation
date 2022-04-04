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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow //NOLINT (cppcoreguidelines-special-member-functions)
{
    Q_OBJECT

private:
    const double viewport = 10.0;
    const int minHue = 0;
    const int maxHue = 359;
    const int minSaturation = 150;
    const int maxSaturation = 255;
    const int minValue = 180;
    const int maxValue = 240;

    Ui::MainWindow * ui;
    std::unique_ptr<QMessageBox> aboutMessageBox;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void OnPlotClick(QMouseEvent * event);
    void OnClearPressed();
    void OnAboutPressed();

private:
    void ShowAboutDialog();
    QColor GenerateColor();
};

#endif // MAINWINDOW_H
