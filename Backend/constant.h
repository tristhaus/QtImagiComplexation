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

#ifndef CONSTANT_H
#define CONSTANT_H

#include "expression.h"

namespace Backend {

    /*!
     * \class Constant
     * \brief The Constant class represents a constant complex number.
     */
    class Constant final : public Expression
    {
    private:
        complex value;

    public:
        /*!
         * \brief Initializes a new instance holding the supplied value.
         * \param input The value to hold as a constant.
         */
        explicit Constant(complex input);
        virtual ~Constant() = default;
        Constant(const Constant&) = delete;
        Constant(Constant&&) = delete;
        Constant& operator=(const Constant&) = delete;
        Constant& operator=(Constant&&) = delete;

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
        [[nodiscard]] std::optional<complex> Evaluate(complex) const override;

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

#endif // CONSTANT_H
