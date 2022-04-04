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

    connect(ui->plot, &QCustomPlot::mousePress, this, &MainWindow::OnPlotClick);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnPlotClick(QMouseEvent * event)
{
    QColor black = QColor(0x00, 0x00, 0x00);
    auto pen = QPen(black);

    auto & plot = ui->plot;

    double xCoord = plot->xAxis->pixelToCoord(event->pos().x());
    double yCoord = plot->yAxis->pixelToCoord(event->pos().y());

    QVector<double> x, y;
    x.append(0.0);
    x.append(xCoord);
    y.append(0.0);
    y.append(yCoord);

    auto * qcpGraph = plot->addGraph();
    qcpGraph->addData(x, y, true);
    qcpGraph->setPen(pen);

    plot->replot();
}
