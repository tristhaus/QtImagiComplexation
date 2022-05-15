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

#ifndef BASEZ_H
#define BASEZ_H

#include "expression.h"

namespace Backend {

    /*!
     * \class BaseZ
     * \brief The BaseZ class represents the independent (z) variable.
     */
    class BaseZ final : public Expression
    {
    public:
        /*!
         * \brief Initializes a new instance.
         */
        BaseZ() = default;
        virtual ~BaseZ() = default;
        BaseZ(const BaseZ&) = delete;
        BaseZ(BaseZ&&) = delete;
        BaseZ& operator=(const BaseZ&) = delete;
        BaseZ& operator=(BaseZ&&) = delete;

        /*!
         * \reimp
         */
        [[nodiscard]] int GetLevel() const override;

        /*!
         * \reimp
         */
        [[nodiscard]] bool IsConstant() const override;

        /*!
         * \reimp
         */
        [[nodiscard]] std::optional<complex> Evaluate(complex input) const override;

        /*!
         * \reimp
         */
        [[nodiscard]] bool operator==(const Expression &other) const override;

        /*!
         * \reimp
         */
        [[nodiscard]] bool operator!=(const Expression &other) const override;
    };

}

#endif // BASEZ_H
