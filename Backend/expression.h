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

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <complex>
#include <optional>
#include <string>

namespace Backend
{
    using complex = std::complex<double>;

    /*!
     * \class Expression
     * \brief The Expression class forms the base for all mathematical expressions.
     *
     * It is abstract and disallows most operations. Its inheritors should basically be immutable.
     */
    class Expression
    {
    public:
        explicit Expression() = default;
        virtual ~Expression() = default;
        Expression(const Expression&) = delete;
        Expression(Expression&&) = delete;
        Expression& operator=(const Expression&) = delete;
        Expression& operator=(Expression&&) = delete;

        /*!
         * \brief Gets the precedence level of the underlying operation.
         * \return The precedence level of the underlying operation.
         */
        [[nodiscard]] virtual int GetLevel() const = 0;

        /*!
         * \brief Gets a value indicating whether the expression is monadic.
         * \return A value indicating whether the expression is monadic.
         */
        [[nodiscard]] virtual bool IsMonadic() const = 0;

        /*!
         * \brief Evaluates the expression using the \a input value as x-coordinate.
         * \param input The value to plug in to the expression.
         * \return The evaluated value or nothing if undefined.
         */
        [[nodiscard]] virtual std::optional<complex> Evaluate(complex input) const = 0;

        /*!
         * \brief Equality operator for the expression, checking type and content.
         * \param other The instance to compare to.
         * \return A value indicating equality.
         */
        [[nodiscard]] virtual bool operator==(const Expression &other) const = 0;

        /*!
         * \brief Inequality operator for the expression, checking type and content.
         * \param other The instance to compare to.
         * \return A value indicating inequality.
         */
        [[nodiscard]] virtual bool operator!=(const Expression &other) const = 0;
    };

}

#endif // EXPRESSION_H
