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

#include <algorithm>
#include <functional>
#include <regex>
#include <set>
#include <string>

#include "basez.h"
#include "constant.h"
#include "functions.h"
#include "parser.h"
#include "power.h"
#include "product.h"
#include "sum.h"

namespace Backend {

    Parser::Parser(bool optimize)
        : optimize(optimize)
    {
    }

    bool Parser::Register(const std::string & name, CreateFunction createFunction)
    {
        auto & functions = Parser::GetRegisteredFunctions();

        auto it = functions.find(name);
        if (it == functions.end())
        {
            functions.emplace(name, createFunction);
            return true;
        }

        return false;
    }

    std::map<std::string, CreateFunction> & Parser::GetRegisteredFunctions()
    {
        static std::map<std::string, CreateFunction> theFunctions;
        return theFunctions;
    }

    std::regex Parser::GetValidationRegex() const
    {
        // This must be adjusted if another math operator is added to the game.
        std::string regexString("^[-+/*^()0-9.,");

        // allow independent variable and imaginary unit
        std::set<char> allowedLetters {'z', 'Z', 'i', 'I'};

        // grab from all function names all letters
        auto & registrationMap = Parser::GetRegisteredFunctions();
        auto registrationMapIt = registrationMap.begin();
        auto registrationMapEnd = registrationMap.end();

        for(; registrationMapIt != registrationMapEnd; ++registrationMapIt)
        {
            auto functionName = registrationMapIt->first;
            auto functionNameIt = functionName.begin();
            auto functionNameEnd = functionName.end();

            for(; functionNameIt != functionNameEnd; ++functionNameIt)
            {
                allowedLetters.insert(*functionNameIt);
            }
        }

        // extend regex by the collection of letters
        auto allowedLettersIt = allowedLetters.begin();
        auto allowedLettersEnd = allowedLetters.end();

        for(; allowedLettersIt != allowedLettersEnd; ++allowedLettersIt)
        {
            regexString += *allowedLettersIt;
        }

        regexString.append("]+$");

        return std::regex(regexString.c_str(), std::regex_constants::ECMAScript);
    }

    std::shared_ptr<Expression> Parser::Parse(const std::string & input) const
    {
        std::string locale(std::setlocale(LC_ALL, nullptr));

        try
        {
            auto prepared = this->PrepareInput(input);
            if (!this->ValidateInput(prepared))
            {
                return nullptr;
            }

            std::setlocale(LC_ALL, "en_US.UTF-8");
            auto result = this->InternalParse(prepared);
            std::setlocale(LC_ALL, locale.c_str());

            return result;
        }
        catch(std::exception &)
        {
            std::setlocale(LC_ALL, locale.c_str());
            return nullptr;
        }
    }

    bool Parser::IsParseable(const std::string& input) const
    {
        return this->Parse(input) != nullptr;
    }

    std::string Parser::PrepareInput(const std::string & input) const
    {
        static std::regex re("[ \t]", std::regex_constants::ECMAScript);

        return std::regex_replace(input, re, "");
    }

    bool Parser::ValidateInput(const std::string & input) const
    {
        // check unsupported characters. The regex should not change after startup
        static auto inputValidationRegex = this->GetValidationRegex();
        if(!std::regex_match(input, inputValidationRegex))
        {
            return false;
        }

        // check for "^-", "^+", which is hard to parse
        if(input.find("^-") != std::string::npos || input.find("^+") != std::string::npos)
        {
            return false;
        }

        // check for dangling operators at the end of string
        auto lastChar = input.back();
        if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '\\' || lastChar == '^' || lastChar == '(')
        {
            return false;
        }

        // check balanced parentheses

        int count = 0;
        auto inputIt = input.begin();
        auto inputEnd = input.end();

        for (; inputIt != inputEnd; ++inputIt)
        {
            if (*inputIt == '(')
            {
                count++;
            }
            else if (*inputIt == ')')
            {
                count--;
            }

            if (count < 0)
            {
                return false;
            }
        }

        return count == 0;
    }

    unsigned long long Parser::FindMatchingBrace(const std::string & input, unsigned long long pos) const
    {
        if (pos > input.length() - 1)
        {
            return -1;
        }

        bool lookForClosing = true;
        if (input[pos] == '(')
        {
            lookForClosing = true;
        }
        else if (input[pos] == ')')
        {
            lookForClosing = false;
        }
        else
        {
            return -1;
        }

        int count = 0;
        if (lookForClosing)
        {
            for (unsigned long long index = pos; index < input.length(); index++)
            {
                char c = input[index];
                if (c == '(')
                {
                    count++;
                }
                else if (c == ')')
                {
                    count--;
                }

                if (count == 0)
                {
                    return index;
                }
            }
        }
        else
        {
            for (int index = static_cast<int>(pos); index >= 0; index--)
            {
                char c = input[index];
                if (c == ')')
                {
                    count++;
                }
                else if (c == '(')
                {
                    count--;
                }

                if (count == 0)
                {
                    return index;
                }
            }
        }

        return -1;
    }

    std::shared_ptr<Expression> Parser::InternalParse(std::string input) const //NOLINT(misc-no-recursion)
    {
        using namespace std::complex_literals;

        if (!ValidateInput(input))
        {
            return nullptr;
        }

        // if fully enclosed in braces, we remove them
        if (input[0] == '(' && input.length() - 1 == this->FindMatchingBrace(input, 0))
        {
            return this->InternalParse(input.substr(1, input.length() - 2));
        }

        // deal with a simple case: plain x
        if (input == "Z" || input == "z")
        {
            return std::make_shared<BaseZ>();
        }

        if (input == "I" || input == "i" || input == "+I" || input == "+i")
        {
            return std::make_shared<Constant>(1.0i);
        }

        if (input == "-I" || input == "-i")
        {
            return std::make_shared<Constant>(-1.0i);
        }

        static std::regex realConstantRegex("^[-+]?[0-9]+[.,]?[0-9]*$", std::regex_constants::ECMAScript);

        // deal with a simple case: a real constant
        if (std::regex_search(input, realConstantRegex))
        {
            return this->ParseToRealConstant(input);
        }

        static std::regex imaginaryConstantRegex("^[-+]?[0-9]+[.,]?[0-9]*[iI]$", std::regex_constants::ECMAScript);

        // deal with a simple case: an imaginary constant
        if (std::regex_search(input, imaginaryConstantRegex))
        {
            return this->ParseToImaginaryConstant(input);
        }

        // now, tokenize
        std::vector<std::string> tokens;
        std::vector<std::string> ops;

        this->Tokenize(input, tokens, ops);

        // deal with a signed single token
        static std::regex singleSignedTokenRegex("^[-+]", std::regex_constants::ECMAScript);

        if (tokens.size() == 1 && std::regex_search(input, singleSignedTokenRegex))
        {
            std::string subToken = input.substr(1);
            std::shared_ptr<Expression> bracketedExpression = this->InternalParse(subToken);

            if (bracketedExpression == nullptr)
            {
                return nullptr;
            }

            if(input[0] == '-' )
            {
                return std::make_shared<Sum>(std::vector<Sum::Summand>({Sum::Summand(Sum::Sign::Minus, bracketedExpression)}));
            }
            else
            {
                return bracketedExpression;
            }
        }

        // reassemble for recursive parsing
        auto opsBegin = ops.begin();
        auto opsEnd = ops.end();

        // First case: plus and minus
        auto findPlus = std::find(opsBegin, opsEnd, PlusString);
        auto findMinus = std::find(opsBegin, opsEnd, MinusString);

        if (findPlus != opsEnd || findMinus != opsEnd)
        {
            return this->ParseToSum(tokens, ops);
        }

        // Second case: multiply and divide
        auto findTimes = std::find(opsBegin, opsEnd, TimesString);
        auto findDivide = std::find(opsBegin, opsEnd, DivideString);

        if (findTimes != opsEnd || findDivide != opsEnd)
        {
            return this->ParseToProduct(tokens, ops);
        }

        // Third case: power expressions
        auto findPower = std::find(opsBegin, opsEnd, PowerString);
        if (findPower != opsEnd)
        {
            return this->ParseToPower(tokens, ops);
        }

        // Fourth case: functions
        if (tokens.size() == 1)
        {
            return this->ParseToFunction(tokens);
        }

        return nullptr;
    }

    void Parser::Tokenize(const std::string & input, std::vector<std::string> & tokens, std::vector<std::string> & ops) const
    {
        static std::function<bool(char)> isOperatorChar = [](char c)
        {
            return c == '-' || c == '+' || c == '*' || c == '/' || c == '^';
        };

        std::string token;
        for (unsigned long long index = 0; index < input.length(); ++index)
        {
            char c = input[index];

            if (c == L'(')
            {
                auto endIndex = FindMatchingBrace(input, index);
                token += input.substr(index, endIndex - index + 1);
                index = endIndex;
                continue;
            }

            if (isOperatorChar(c) && token.length() > 0)
            {
                tokens.push_back(token);
                token = "";
                ops.emplace_back(1, c);
                continue;
            }

            token += c;
        }

        if (token.length() > 0)
        {
            tokens.push_back(token);
        }
    }

    std::shared_ptr<Expression> Parser::ParseToRealConstant(const std::string & input) const
    {
        try
        {
            static std::regex re(",", std::regex_constants::ECMAScript);

            auto anglified = std::regex_replace(input, re, ".");

            double parsed = std::stod(anglified);
            return std::make_shared<Constant>(parsed);
        }
        catch (std::exception &)
        {
            return nullptr;
        }
    }

    std::shared_ptr<Expression> Parser::ParseToImaginaryConstant(const std::string & input) const
    {
        using namespace std::complex_literals;

        try
        {
            auto numericalOnly = input.substr(0, input.size() - 1);

            static std::regex re(",", std::regex_constants::ECMAScript);

            auto anglified = std::regex_replace(numericalOnly, re, ".");

            double parsed = std::stod(anglified);
            return std::make_shared<Constant>(parsed * 1.0i);
        }
        catch (std::exception &)
        {
            return nullptr;
        }
    }

    std::shared_ptr<Expression> Parser::ParseToSum(std::vector<std::string> & tokens, std::vector<std::string> & ops) const
    {
        if(tokens.size() != ops.size() + 1)
        {
            return nullptr;
        }

        std::vector<Sum::Summand> targetList;

        Sum::Sign sign = Sum::Sign::Plus;

        std::string token = tokens[0];
        tokens.erase(tokens.begin());

        auto opsIt = ops.begin();
        auto opsEnd = ops.end();

        for(;opsIt != opsEnd; ++opsIt)
        {
            if (*opsIt == "+" || *opsIt == "-")
            {
                auto expression = this->InternalParse(token);
                if (expression == nullptr)
                {
                    return nullptr;
                }

                targetList.emplace_back(Sum::Summand(sign, expression));
                token = tokens[0];
                tokens.erase(tokens.begin());
                sign = *opsIt == "+" ? Sum::Sign::Plus : Sum::Sign::Minus;
            }
            else
            {
                token += (*opsIt) + tokens[0];
                tokens.erase(tokens.begin());
            }
        }

        // one last token to take care of
        if (tokens.size() == 1)
        {
            token = tokens[0];
        }

        if (!token.empty())
        {
            auto expression = this->InternalParse(token);
            if (expression == nullptr)
            {
                return nullptr;
            }

            targetList.emplace_back(Sum::Summand(sign, expression));
        }

        if (!optimize)
        {
            return std::make_shared<Sum>(targetList);
        }

        std::vector<Sum::Summand> constantList;
        std::vector<Sum::Summand> variableList;

        for (const auto & target : targetList) {
            if(target.expression->IsConstant())
            {
                constantList.emplace_back(target);
            }
            else
            {
                variableList.emplace_back(target);
            }
        }

        if (constantList.empty())
        {
            return std::make_shared<Sum>(variableList);
        }

        auto constantSum = std::make_shared<Sum>(constantList);
        auto constantValue = constantSum->Evaluate(0.0).value();

        auto replacementConstant = std::make_shared<Constant>(constantValue);

        if(variableList.empty())
        {
            return replacementConstant;
        }

        variableList.insert(variableList.begin(), Sum::Summand(Sum::Sign::Plus, replacementConstant));

        return std::make_shared<Sum>(variableList);
    }

    std::shared_ptr<Expression> Parser::ParseToProduct(std::vector<std::string> & tokens, std::vector<std::string> & ops) const
    {
        if(tokens.size() != ops.size() + 1)
        {
            return nullptr;
        }

        std::vector<Product::Factor> targetList;

        Product::Exponent sign = Product::Exponent::Positive;

        std::string token = tokens[0];
        tokens.erase(tokens.begin());

        auto opsIt = ops.begin();
        auto opsEnd = ops.end();

        for(;opsIt != opsEnd; ++opsIt)
        {
            if (*opsIt == "*" || *opsIt == "/")
            {
                auto expression = this->InternalParse(token);
                if (expression == nullptr)
                {
                    return nullptr;
                }

                targetList.emplace_back(Product::Factor(sign, expression));
                token = tokens[0];
                tokens.erase(tokens.begin());
                sign = *opsIt == "*" ? Product::Exponent::Positive : Product::Exponent::Negative;
            }
            else
            {
                token += (*opsIt) + tokens[0];
                tokens.erase(tokens.begin());
            }
        }

        // one last token to take care of
        if (tokens.size() == 1)
        {
            token = tokens[0];
        }

        if (!token.empty())
        {
            auto expression = this->InternalParse(token);
            if (expression == nullptr)
            {
                return nullptr;
            }

            targetList.emplace_back(Product::Factor(sign, expression));
        }

        if(!optimize)
        {
            return std::make_shared<Product>(targetList);
        }

        std::vector<Product::Factor> constantList;
        std::vector<Product::Factor> variableList;

        for (const auto & target : targetList) {
            if(target.expression->IsConstant())
            {
                constantList.emplace_back(target);
            }
            else
            {
                variableList.emplace_back(target);
            }
        }

        if (constantList.empty())
        {
            return std::make_shared<Product>(variableList);
        }

        auto constantFactor = std::make_shared<Product>(constantList);
        auto constantValue = constantFactor->Evaluate(0.0).value();

        auto replacementConstant = std::make_shared<Constant>(constantValue);

        if(variableList.empty())
        {
            return replacementConstant;
        }

        variableList.insert(variableList.begin(), Product::Factor(Product::Exponent::Positive, replacementConstant));

        return std::make_shared<Product>(variableList);
    }

    std::shared_ptr<Expression> Parser::ParseToPower(std::vector<std::string>& tokens, std::vector<std::string>& ops) const
    {
        if (tokens.size() != ops.size() + 1)
        {
            return nullptr;
        }

        auto opsBegin = ops.begin();
        auto opsEnd = ops.end();
        auto findPlus = std::find(opsBegin, opsEnd, PlusString);
        auto findMinus = std::find(opsBegin, opsEnd, MinusString);
        auto findTimes = std::find(opsBegin, opsEnd, TimesString);
        auto findDivide = std::find(opsBegin, opsEnd, DivideString);

        if (findPlus != opsEnd || findMinus != opsEnd || findTimes != opsEnd || findDivide != opsEnd)
        {
            return nullptr;
        }

        auto baseToken = tokens[0];
        tokens.erase(tokens.begin());
        auto baseExpression = this->InternalParse(baseToken);
        if (!baseExpression)
        {
            return nullptr;
        }

        auto exponentToken = tokens[0];
        tokens.erase(tokens.begin());
        for (unsigned long int index = 1; index < ops.size(); index++)
        {
            exponentToken += ops[index] + tokens[0];
            tokens.erase(tokens.begin());
        }

        auto exponentExpression = this->InternalParse(exponentToken);
        if (!exponentExpression)
        {
            return nullptr;
        }

        return std::make_shared<Power>(baseExpression, exponentExpression);
    }

    std::shared_ptr<Expression> Parser::ParseToFunction(std::vector<std::string>& tokens) const
    {
        auto functionToken = tokens[0];
        tokens.erase(tokens.begin());
        auto index = functionToken.find('(');
        auto name = functionToken.substr(0, index);

        auto & functions = Parser::GetRegisteredFunctions();

        auto createFunction = functions.find(name);

        if(createFunction == functions.end())
        {
            return nullptr;
        }

        auto argumentToken = functionToken.substr(index);
        auto argument = this->InternalParse(argumentToken);

        if(!argument)
        {
            return nullptr;
        }

        return (*createFunction).second(argument);
    }
}
