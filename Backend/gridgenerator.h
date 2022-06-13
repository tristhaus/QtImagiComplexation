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

#include <vector>

#include "expression.h"

#ifndef GRIDGENERATOR_H
#define GRIDGENERATOR_H

namespace Backend {

    /*!
     * \brief The GridGenerator class provides the generation of grids of
     *        regularly spaced input values
     */
    class GridGenerator
    {
    private:
        const double maxX;
        const double maxY;

    public:
        /*!
         * \brief GridGenerator
         * \param maxX The maximum absolute value in real/x-direction.
         * \param maxY The maximum absolute value in imaginary/y-direction.
         */
        GridGenerator(double maxX, double maxY);

        /*!
         * \brief CreateRectangular creates a square grid
         *        defined by the point-to-point distance
         *        and aligned with the axis.
         * \param dist The point-to-point distance.
         * \return A list of values constituting the square grid.
         */
        [[nodiscard]] std::vector<complex> CreateSquare(double dist);

        /*!
         * \brief CreateAngularFromConstantAngle creates an circular/angular grid
         *        defined by the radius increment for each circle
         *        and the angle between points on each circle.
         *        The first point on each circle is aligned with the real axis.
         * \param radial The radius increment.
         * \param angle The angle in degrees.
         * \return A list of values constituting the angular grid.
         */
        [[nodiscard]] std::vector<complex> CreateAngularFromConstantAngle(double radial, double angle);

        /*!
         * \brief CreateAngularFromApproximateDistance creates an circular/angular grid
         *        defined by the approximate distance
         *        which is used as the inter-circle distance
         *        and to determine the number of equally-spaced points
         *        on each individual circle.
         *        The first point on each circle is aligned with the real axis.
         * \param dist The approximate point-to-point distance.
         * \return A list of values constituting the angular grid.
         */
        [[nodiscard]] std::vector<complex> CreateAngularFromApproximateDistance(double dist);

    private:
        [[nodiscard]] std::vector<complex> CreatePointsOnCircle(double radius, double angle);
    };

}

#endif // GRIDGENERATOR_H
