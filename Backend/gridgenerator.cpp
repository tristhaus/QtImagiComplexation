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

#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

#include "gridgenerator.h"

namespace Backend {

    GridGenerator::GridGenerator(double maxX, double maxY)
        : maxX(maxX), maxY(maxY)
    {
    }

    std::vector<complex> GridGenerator::CreateSquare(double dist)
    {
        using namespace std::complex_literals;

        std::vector<complex> list;

        int xCount = static_cast<int>(this->maxX / dist);
        int yCount = static_cast<int>(this->maxY / dist);

        for(int x = -xCount; x <= xCount; ++x)
        {
            for(int y = -yCount; y <= yCount; ++y)
            {
                list.emplace_back(static_cast<double>(x) * complex(dist) + static_cast<double>(y) * dist * complex(1.0i));
            }
        }

        return list;
    }

    std::vector<complex> GridGenerator::CreateAngularFromConstantAngle(double radial, double angle)
    {
        using namespace std::complex_literals;

        std::vector<complex> list;

        list.emplace_back(complex(0.0));

        auto rCount = static_cast<int>(std::sqrt(this->maxX * this->maxX + this->maxY * this->maxY) / radial);

        for (int r = 1; r <= rCount; ++r)
        {
            auto circle = CreatePointsOnCircle(r * radial, angle);
            list.insert(list.end(), circle.begin(), circle.end());
        }

        return list;
    }

    std::vector<complex> GridGenerator::CreateAngularFromApproximateDistance(double dist)
    {
        using namespace std::complex_literals;

        std::vector<complex> list;

        list.emplace_back(complex(0.0));

        auto rCount = static_cast<int>(std::sqrt(this->maxX * this->maxX + this->maxY * this->maxY) / dist);

        for (int r = 1; r <= rCount; ++r)
        {
            auto alpha = 2 * asin(0.5 / r);
            auto k = std::floor(2.0 * M_PI / alpha);
            auto gamma = 360.0 / k;

            auto circle = CreatePointsOnCircle(r * dist, gamma);
            list.insert(list.end(), circle.begin(), circle.end());
        }

        return list;
    }

    std::vector<complex> GridGenerator::CreatePointsOnCircle(double radius, double angle)
    {
        using namespace std::complex_literals;

        std::vector<complex> list;

        auto tCount = static_cast<int>(360.0 / angle);

        for (int t = 0; t < tCount; ++t)
        {
            auto z = std::polar(radius, t * angle / 180.0 * M_PI);

            if(std::abs(z.real()) <= this->maxX && std::abs(z.imag()) <= this->maxY)
            {
                list.push_back(z);
            }
        }

        return list;
    }

}
