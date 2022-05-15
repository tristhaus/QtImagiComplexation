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

#ifndef POWER_H
#define POWER_H

#include "expression.h"
#include <memory>

namespace Backend
{
    /*!
     * \class Power
     * \brief The Power class represents a power, consisting of a base and an exponent.
     */
    class Power final : public Expression
    {
    private:
        std::shared_ptr<Expression> base;
        std::shared_ptr<Expression> exponent;

    public:
        /*!
         * \brief Initializes a new instance holding the supplied base and exponent.
         * \param base The base of the power expression.
         * \param exponent The exponent of the power expression.
         */
        Power(std::shared_ptr<Expression> base, std::shared_ptr<Expression> exponent);
        virtual ~Power();
        Power(const Power&) = delete;
        Power(Power&&) = delete;
        Power& operator=(const Power&) = delete;
        Power& operator=(Power&&) = delete;

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

#endif // POWER_H
