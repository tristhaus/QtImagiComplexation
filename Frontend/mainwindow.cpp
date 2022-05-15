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
      plotting(false),
      ui(new Ui::MainWindow),
      parser(Backend::Parser(true))
{
    ui->setupUi(this);

    ui->plot->xAxis->setRange(-viewport, viewport);
    ui->plot->yAxis->setRange(-viewport, viewport);
    ui->plot->replot();

    ui->funcLineEdit->setText(QString::fromUtf8(u8"z * i"));
    this->UpdateUiState();

    connect(ui->plot, &QCustomPlot::mousePress, this, &MainWindow::OnPlotClick);
    connect(ui->funcLineEdit, &QLineEdit::textChanged, this, &MainWindow::OnFuncLineEditTextChanged);
    connect(ui->funcLineEdit, &QLineEdit::returnPressed, this, &MainWindow::OnReturnKeyPressed);
    connect(ui->funcSetButton, &QAbstractButton::pressed, this, &MainWindow::OnSetPressed);
    connect(ui->funcClearButton, &QAbstractButton::pressed, this, &MainWindow::OnClearPressed);
    connect(ui->aboutButton, &QAbstractButton::pressed, this, &MainWindow::OnAboutPressed);

    QColor lightPink = QColor(0xFF, 0xB6, 0xC1);

    this->parseablePalette.setColor(QPalette::Base, Qt::white);
    this->parseablePalette.setColor(QPalette::Text, Qt::black);
    this->nonParseablePalette.setColor(QPalette::Base, lightPink);
    this->nonParseablePalette.setColor(QPalette::Text, Qt::black);

    ui->funcLineEdit->setPalette(this->parseablePalette);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddArrow(double startX, double startY, double endX, double endY)
{
    auto & plot = ui->plot;

    auto pen = QPen(this->GenerateColor());

    auto *arrow = new QCPItemLine(plot); //NOLINT(cppcoreguidelines-owning-memory)
    arrow->setHead(QCPLineEnding::esSpikeArrow);
    arrow->start->setCoords(startX, startY);
    arrow->end->setCoords(endX, endY);
    arrow->setPen(pen);

    plot->replot();
}

void MainWindow::OnPlotClick(QMouseEvent * event)
{
    if (!this->plotting)
    {
        return;
    }

    double inputX = ui->plot->xAxis->pixelToCoord(event->pos().x());
    double inputY = ui->plot->yAxis->pixelToCoord(event->pos().y());

    auto result = this->expression->Evaluate(Backend::complex(inputX, inputY));

    if (!result.has_value())
    {
        return;
    }

    this->AddArrow(inputX, inputY, result.value().real(), result.value().imag());
}

void MainWindow::OnFuncLineEditTextChanged()
{
    this->UpdateParseability();
}

void MainWindow::OnReturnKeyPressed()
{
    this->UpdateExpression();
}

void MainWindow::OnSetPressed()
{
    this->UpdateExpression();
}

void MainWindow::OnClearPressed()
{
    this->ClearPlot();
}

void MainWindow::OnAboutPressed()
{
    this->ShowAboutDialog();
}

void MainWindow::ShowAboutDialog()
{
    auto messageBoxTitleTemplate = QCoreApplication::translate("MainWindow", "About %1", nullptr);
    auto messageBoxTitle = messageBoxTitleTemplate.arg(QCoreApplication::translate("MainWindow", "QtImagiComplexation", nullptr));

    //: Arg 1 is a placeholder for the program name. Format is Qt Rich Text.
    auto messageBoxTextTemplate = QCoreApplication::translate("MainWindow", R"(An interactive vector field visualization for complex functions.<br /><br />%1 Copyright (C) 2022 and later, tristhaus<br />This program comes with ABSOLUTELY NO WARRANTY.<br />This is free software, and you are welcome to redistribute it under certain conditions. See provided LICENSE file for details.<br /><br />Graphical user interface built using <a href="https://doc.qt.io/">Qt</a>.<br /><a href="https://www.qcustomplot.com/">QCustomPlot</a> library (Version 2.1.0) by Emanuel Eichhammer used under the <a href="https://www.gnu.org/licenses/gpl-3.0.html">GPL v3</a>.)", nullptr);
    auto messageBoxText = messageBoxTextTemplate.arg(QCoreApplication::translate("MainWindow", "QtImagiComplexation", nullptr));

    this->aboutMessageBox = std::make_unique<QMessageBox>(
                    QMessageBox::Icon::NoIcon,
                    messageBoxTitle,
                    messageBoxText);

    this->aboutMessageBox->setTextFormat(Qt::RichText);
    this->aboutMessageBox->setTextInteractionFlags(Qt::TextBrowserInteraction);

    this->aboutMessageBox->exec();

    this->aboutMessageBox.reset();
}

void MainWindow::UpdateUiState()
{
    ui->funcLineEdit->setDisabled(this->plotting);
    ui->funcSetButton->setDisabled(this->plotting);
    ui->funcClearButton->setDisabled(!this->plotting);
}

void MainWindow::UpdateParseability()
{
    std::string funcString = ui->funcLineEdit->text().toStdString();
    bool isParseable = funcString.empty() || this->parser.IsParseable(funcString);
    QPalette & palette = isParseable ? this->parseablePalette : this->nonParseablePalette;

    ui->funcLineEdit->setPalette(palette);
}

void MainWindow::UpdateExpression()
{
    auto input = std::string(ui->funcLineEdit->text().toStdString());
    if (this->parser.IsParseable(input))
    {
        this->expression = this->parser.Parse(input);

        this->plotting = true;
    }

    this->UpdateUiState();
}

void MainWindow::ClearPlot()
{
    ui->plot->clearItems();
    ui->plot->replot();
    this->expression.reset();
    this->plotting = false;

    this->UpdateUiState();
}

QColor MainWindow::GenerateColor() const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<unsigned int> distHue(minHue, maxHue);
    static std::uniform_int_distribution<unsigned int> distSaturation(minSaturation, maxSaturation);
    static std::uniform_int_distribution<unsigned int> distValue(minValue, maxValue);

    return QColor::fromHsv(distHue(gen), distSaturation(gen), distValue(gen));
}
