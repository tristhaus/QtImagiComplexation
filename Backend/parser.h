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

#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <memory>
#include <regex>

#include "expression.h"

namespace Backend {

    using CreateFunction = std::shared_ptr<Expression> (*)(std::shared_ptr<Expression>);

    /*!
     * \class Parser
     * \brief The Parser class provides functionality to obtain a \ref Expression from a string.
     */
    class Parser final
    {
    private:
        bool optimize;

        /*!
         * \brief InputValidationRegex provides cheap validation for the input string.
         * It must be adjusted any time another math function is added to the game.
         */
        static const std::regex InputValidationRegex;

        constexpr static const std::string_view PlusString = "+";
        constexpr static const std::string_view MinusString = "-";
        constexpr static const std::string_view TimesString = "*";
        constexpr static const std::string_view DivideString = "/";
        constexpr static const std::string_view PowerString = "^";

    public:
        /*!
         * \brief Initializes a new instance.
         * \param optimize Flag indicating whether to optimize away some constant terms
         */
        explicit Parser(bool optimize);

        /*!
         * \brief Parse creates an \ref expression from a string, if possible.
         * \param input The string to parse.
         * \return A pointer to the expression or a nullptr.
         */
        [[nodiscard]] std::shared_ptr<Expression> Parse(const std::string & input) const;

        /*!
         * \brief IsParseable indicates whether the supplied string is parseable.
         * \param input The string to check for parseability.
         * \return true if the argument is parseable, false otherwise.
         */
        [[nodiscard]] bool IsParseable(const std::string & input) const;

         /*!
         * \brief Register registers a function to create an expression representing
         *        a mathematical function under the given human-readable name.
         * \param name The human-readable name of the function, e.g. "sin".
         * \param createFunction Pointer to a function to create an expression
         *        representing the mathematical function.
         * \return A dummy bool such that the function can be used in static initialization.
         */
        static bool Register(const std::string & name, CreateFunction createFunction);

    private:
        /*!
         * \brief GetRegisteredFunctions Gets the class-static map of registered functions for the parser.
         *
         * By doing it this way, we avoid the static initialization fiasco,
         * because the map returned is actually method-static, initialized on first access.
         *
         * \return The registration map.
         */
        static std::map<std::string, CreateFunction> & GetRegisteredFunctions();

        [[nodiscard]] std::regex GetValidationRegex() const;
        [[nodiscard]] std::string PrepareInput(const std::string & input) const;
        [[nodiscard]] bool ValidateInput(const std::string & input) const;
        [[nodiscard]] unsigned long long FindMatchingBrace(const std::string & input, unsigned long long pos) const;

        [[nodiscard]] std::shared_ptr<Expression> InternalParse(std::string input) const;
        void Tokenize(const std::string & input, std::vector<std::string> & tokens, std::vector<std::string> & ops) const;
        [[nodiscard]] std::shared_ptr<Expression> ParseToRealConstant(const std::string & input) const;
        [[nodiscard]] std::shared_ptr<Expression> ParseToImaginaryConstant(const std::string & input) const;
        [[nodiscard]] std::shared_ptr<Expression> ParseToSum(std::vector<std::string>& tokens, std::vector<std::string>& ops) const;
        [[nodiscard]] std::shared_ptr<Expression> ParseToProduct(std::vector<std::string>& tokens, std::vector<std::string>& ops) const;
        [[nodiscard]] std::shared_ptr<Expression> ParseToPower(std::vector<std::string>& tokens, std::vector<std::string>& ops) const;
        [[nodiscard]] std::shared_ptr<Expression> ParseToFunction(std::vector<std::string>& tokens) const;
    };
}

#endif // PARSER_H
