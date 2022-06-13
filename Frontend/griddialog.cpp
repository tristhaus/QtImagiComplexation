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

#include "griddialog.h"

Ui::GridDialog::GridDialog(QWidget * parent, double viewportX, double viewportY)
    : QDialog(parent),
      viewportX(viewportX),
      viewportY(viewportY),
      gridType(GridType::NotSet),
      distLike(0.0),
      angleDegrees(0.0)
{
    this->SetupUi();
    connect(this->squareGridAcceptButton, &QAbstractButton::pressed, this, &GridDialog::OnSquareGridAcceptButtonPressed);
    connect(this->rcaGridAcceptButton, &QAbstractButton::pressed, this, &GridDialog::OnRadialConstantAngleGridAcceptButtonPressed);
    connect(this->radGridAcceptButton, &QAbstractButton::pressed, this, &GridDialog::OnRadialApproximateDistanceAcceptButtonPressed);
    connect(this->cancelButton, &QAbstractButton::pressed, this, &GridDialog::OnCancelPressed);
}

std::vector<Backend::complex> Ui::GridDialog::GetResult() const
{
    Backend::GridGenerator gridGenerator(this->viewportX, this->viewportY);

    switch (this->gridType)
    {
    case GridType::Square:
        return gridGenerator.CreateSquare(this->distLike);

    case GridType::RadialConstantAngle:
       return gridGenerator.CreateAngularFromConstantAngle(this->distLike, this->angleDegrees);

    case GridType::RadialApproximateDistance:
        return gridGenerator.CreateAngularFromApproximateDistance(this->distLike);

    default:
        return std::vector<Backend::complex>();
    }
}

void Ui::GridDialog::SetupUi()
{
    this->setMinimumSize(20, 20);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());

    this->setSizePolicy(sizePolicy);

    verticalDialogLayout = new QVBoxLayout(this); //NOLINT(cppcoreguidelines-owning-memory)
    verticalDialogLayout->setObjectName(QString::fromUtf8(u8"verticaldialoglayout"));

    // ---------------------------------------------------------------------------------

    squareGridGroupBox = new QGroupBox(this); //NOLINT(cppcoreguidelines-owning-memory)
    squareGridGroupBox->setObjectName(QString::fromUtf8(u8"squareGridGroupBox"));
    verticalDialogLayout->addWidget(squareGridGroupBox);

    squareGridLayout = new QGridLayout(squareGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    squareGridLayout->setObjectName(QString::fromUtf8(u8"squareGridLayout"));

    squareGridDistLabel = new QLabel(squareGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    squareGridDistLabel->setObjectName(QString::fromUtf8(u8"squareGridDistLabel"));
    squareGridLayout->addWidget(squareGridDistLabel, 0, 0, 1, 1);

    squareGridDistSpinBox = new QDoubleSpinBox(squareGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    squareGridDistSpinBox->setObjectName(QString::fromUtf8(u8"squareGridDistSpinBox"));
    squareGridLayout->addWidget(squareGridDistSpinBox, 0, 1, 1, 1);

    squareGridDistSpinBox->setValue(1.5);
    squareGridDistSpinBox->setRange(0.5, 5.0);
    squareGridDistSpinBox->setDecimals(2);
    squareGridDistSpinBox->setSingleStep(0.25);

    squareGridAcceptButton = new QPushButton(squareGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    squareGridAcceptButton->setObjectName(QString::fromUtf8(u8"squareGridAcceptButton"));
    squareGridLayout->addWidget(squareGridAcceptButton, 1, 1, 1, 1);

    // ---------------------------------------------------------------------------------

    rcaGridGroupBox = new QGroupBox(this); //NOLINT(cppcoreguidelines-owning-memory)
    rcaGridGroupBox->setObjectName(QString::fromUtf8(u8"rcaGridGroupBox"));
    verticalDialogLayout->addWidget(rcaGridGroupBox);

    rcaGridLayout = new QGridLayout(rcaGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    rcaGridLayout->setObjectName(QString::fromUtf8(u8"rcaGridLayout"));

    rcaGridRadialLabel = new QLabel(rcaGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    rcaGridRadialLabel->setObjectName(QString::fromUtf8(u8"rcaGridRadialLabel"));
    rcaGridLayout->addWidget(rcaGridRadialLabel, 0, 0, 1, 1);

    rcaGridRadialSpinBox = new QDoubleSpinBox(rcaGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    rcaGridRadialSpinBox->setObjectName(QString::fromUtf8(u8"rcaGridRadialSpinBox"));
    rcaGridLayout->addWidget(rcaGridRadialSpinBox, 0, 1, 1, 1);

    rcaGridRadialSpinBox->setValue(1.5);
    rcaGridRadialSpinBox->setRange(0.5, 5.0);
    rcaGridRadialSpinBox->setDecimals(2);
    rcaGridRadialSpinBox->setSingleStep(0.25);

    rcaGridAngleLabel = new QLabel(rcaGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    rcaGridAngleLabel->setObjectName(QString::fromUtf8(u8"rcaGridAngleLabel"));
    rcaGridLayout->addWidget(rcaGridAngleLabel, 1, 0, 1, 1);

    rcaGridAngleSpinBox = new QSpinBox(rcaGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    rcaGridAngleSpinBox->setObjectName(QString::fromUtf8(u8"rcaGridAngleSpinBox"));
    rcaGridLayout->addWidget(rcaGridAngleSpinBox, 1, 1, 1, 1);

    rcaGridAngleSpinBox->setValue(30);
    rcaGridAngleSpinBox->setRange(5, 180);
    rcaGridAngleSpinBox->setSingleStep(5);

    rcaGridAcceptButton = new QPushButton(rcaGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    rcaGridAcceptButton->setObjectName(QString::fromUtf8(u8"rcaGridAcceptButton"));
    rcaGridLayout->addWidget(rcaGridAcceptButton, 2, 1, 1, 1);

    // ---------------------------------------------------------------------------------

    radGridGroupBox = new QGroupBox(this); //NOLINT(cppcoreguidelines-owning-memory)
    radGridGroupBox->setObjectName(QString::fromUtf8(u8"radGridGroupBox"));
    verticalDialogLayout->addWidget(radGridGroupBox);

    radGridLayout = new QGridLayout(radGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    radGridLayout->setObjectName(QString::fromUtf8(u8"radGridLayout"));

    radGridDistLabel = new QLabel(radGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    radGridDistLabel->setObjectName(QString::fromUtf8(u8"radGridDistLabel"));
    radGridLayout->addWidget(radGridDistLabel, 0, 0, 1, 1);

    radGridDistSpinBox = new QDoubleSpinBox(radGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    radGridDistSpinBox->setObjectName(QString::fromUtf8(u8"radGridDistSpinBox"));
    radGridLayout->addWidget(radGridDistSpinBox, 0, 1, 1, 1);

    radGridDistSpinBox->setValue(1.5);
    radGridDistSpinBox->setRange(0.5, 5.0);
    radGridDistSpinBox->setDecimals(2);
    radGridDistSpinBox->setSingleStep(0.25);

    radGridAcceptButton = new QPushButton(radGridGroupBox); //NOLINT(cppcoreguidelines-owning-memory)
    radGridAcceptButton->setObjectName(QString::fromUtf8(u8"radGridAcceptButton"));
    radGridLayout->addWidget(radGridAcceptButton, 1, 1, 1, 1);

    // ---------------------------------------------------------------------------------

    cancelButton = new QPushButton(this); //NOLINT(cppcoreguidelines-owning-memory)
    cancelButton->setObjectName(QString::fromUtf8(u8"cancelButton"));
    verticalDialogLayout->addWidget(cancelButton);

    // ---------------------------------------------------------------------------------

    this->setWindowTitle(tr("Add grid"));

    squareGridGroupBox->setTitle(tr("Square grid"));
    squareGridDistLabel->setText(tr("Distance"));
    squareGridAcceptButton->setText(tr("Add"));

    rcaGridGroupBox->setTitle(tr("Angular grid with fixed angle"));
    rcaGridRadialLabel->setText(tr("Radial increment"));
    rcaGridAngleLabel->setText(tr("Angle"));
    rcaGridAcceptButton->setText(tr("Add"));

    radGridGroupBox->setTitle(tr("Angular grid with approximate distance"));
    radGridDistLabel->setText(tr("Distance"));
    radGridAcceptButton->setText(tr("Add"));

    cancelButton->setText(tr("Cancel"));
}

void Ui::GridDialog::OnSquareGridAcceptButtonPressed()
{
    this->gridType = GridType::Square;
    this->distLike = this->squareGridDistSpinBox->value();
    this->accept();
}

void Ui::GridDialog::OnRadialConstantAngleGridAcceptButtonPressed()
{
    this->gridType = GridType::RadialConstantAngle;
    this->distLike = this->rcaGridRadialSpinBox->value();
    this->angleDegrees = this->rcaGridAngleSpinBox->value();
    this->accept();
}

void Ui::GridDialog::OnRadialApproximateDistanceAcceptButtonPressed()
{
    this->gridType = GridType::RadialApproximateDistance;
    this->distLike = this->radGridDistSpinBox->value();
    this->accept();
}

void Ui::GridDialog::OnCancelPressed()
{
    this->reject();
}
