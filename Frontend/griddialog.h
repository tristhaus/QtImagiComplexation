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

#ifndef GRIDDIALOG_H
#define GRIDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "../Backend/gridgenerator.h"

class FrontendTest;

namespace Ui
{

    class GridDialog : public QDialog
    {
        Q_OBJECT

        friend FrontendTest;

    private:
        enum class GridType
        {
            NotSet,
            Square,
            RadialConstantAngle,
            RadialApproximateDistance,
        };

    private:
        const double viewportX;
        const double viewportY;

        GridType gridType;
        double distLike;
        double angleDegrees;

        QVBoxLayout * verticalDialogLayout{};

        QGroupBox * squareGridGroupBox{};
        QGridLayout * squareGridLayout{};
        QLabel * squareGridDistLabel{};
        QDoubleSpinBox * squareGridDistSpinBox{};
        QPushButton * squareGridAcceptButton{};

        QGroupBox * rcaGridGroupBox{};
        QGridLayout * rcaGridLayout{};
        QLabel * rcaGridRadialLabel{};
        QDoubleSpinBox * rcaGridRadialSpinBox{};
        QLabel * rcaGridAngleLabel{};
        QSpinBox * rcaGridAngleSpinBox{};
        QPushButton * rcaGridAcceptButton{};

        QGroupBox * radGridGroupBox{};
        QGridLayout * radGridLayout{};
        QLabel * radGridDistLabel{};
        QDoubleSpinBox * radGridDistSpinBox{};
        QPushButton * radGridAcceptButton{};

        QPushButton * cancelButton{};

    public:
        /*!
         * \brief Initializes a new instance.
         * \param parent The Qt parent widget
         * \param viewportX The maximum absolute value in x-direction.
         * \param viewportY The maximum absolute value in y-direction.
         */
        GridDialog(QWidget * parent, double viewportX, double viewportY);

        /*!
         * \brief GetResult gets the result of the grid generation, if any.
         * \return A list of points representing the generated grid.
         */
        [[nodiscard]] std::vector<Backend::complex> GetResult() const;

    private:
        void SetupUi();

    private slots:
        void OnCancelPressed();
        void OnSquareGridAcceptButtonPressed();
        void OnRadialConstantAngleGridAcceptButtonPressed();
        void OnRadialApproximateDistanceAcceptButtonPressed();
    };

}

#endif // GRIDDIALOG_H
