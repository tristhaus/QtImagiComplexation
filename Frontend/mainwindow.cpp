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

#include "mainwindow.h"
#include "mainwindow_ui.h"

#include <QMessageBox>
#include <utility>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->plot->xAxis->setRange(-10.0, 10.0);
    ui->plot->yAxis->setRange(-10.0, 10.0);
    ui->plot->replot();

    ui->funcLineEdit->setText(QString::fromUtf8("z * i"));
    ui->funcLineEdit->setDisabled(true);
    ui->funcSetButton->setDisabled(true);

    connect(ui->plot, &QCustomPlot::mousePress, this, &MainWindow::OnPlotClick);
    connect(ui->funcClearButton, &QAbstractButton::pressed, this, &MainWindow::OnClearPressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnPlotClick(QMouseEvent * event)
{
    auto & plot = ui->plot;

    auto pen = QPen(this->GenerateColor());

    double xCoord = plot->xAxis->pixelToCoord(event->pos().x());
    double yCoord = plot->yAxis->pixelToCoord(event->pos().y());

    QCPItemLine *arrow = new QCPItemLine(plot); //NOLINT(cppcoreguidelines-owning-memory
    arrow->setHead(QCPLineEnding::esSpikeArrow);
    arrow->start->setCoords(xCoord, yCoord);
    arrow->end->setCoords(-yCoord, xCoord);
    arrow->setPen(pen);

    plot->replot();
}

void MainWindow::OnClearPressed()
{
    ui->plot->clearItems();
    ui->plot->replot();
}

QColor MainWindow::GenerateColor()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<unsigned int> distHue(0, 360);
    static std::uniform_int_distribution<unsigned int> distSaturation(150, 255);
    static std::uniform_int_distribution<unsigned int> distValue(180, 240);

    return QColor::fromHsv(distHue(gen), distSaturation(gen), distValue(gen));
}
