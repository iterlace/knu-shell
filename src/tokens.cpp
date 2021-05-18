
#include "tokens.h"
#include "util.h"


BaseStringToken::BaseStringToken(std::string s) {
    value.assign(s.begin(), s.end());
}

void BaseStringToken::push_back(char c) {
    value.push_back(c);
}

std::string BaseStringToken::to_str() const {
    return value;
}

std::string BaseStringToken::get_raw() const {
    return value;
}

bool BaseStringToken::operator==(Token &other) {
    return other.to_str() == to_str() && other.type() == type();
}

bool BaseStringToken::operator!=(Token &other) {
    return !(this->operator==(other));
}

CommandToken::CommandToken(const TempToken &token) {
    value = token.get_raw();
}

VariableToken::VariableToken(const std::string &s) {
    value = s;
}

VariableToken::VariableToken(const TempToken &token) {
    value = token.get_raw();
}

bool AssignmentToken::operator==(Token &other) {
    return other.type() == type();
}

bool AssignmentToken::operator!=(Token &other) {
    return !(this->operator==(other));
}

std::string AssignmentToken::to_str() const {
    return "=";
}

StringToken::StringToken(const std::vector<std::shared_ptr<Token>> &tokens) {
    for (const auto& token : tokens) {
        if (auto text = dynamic_cast<TextToken *>(token.get())) {
            auto owned_text = std::make_shared<TextToken>();
            *owned_text = *text;
            literals.push_back(owned_text);
        } else if (auto link = dynamic_cast<LinkToken *>(token.get())) {
            auto owned_link = std::make_shared<LinkToken>();
            *owned_link = *link;
            literals.push_back(owned_link);
        } else {
            throw;
        }
    }
}

bool StringToken::operator==(Token &other) {
    if (other.type() != type())
        return false;
    auto other_string = dynamic_cast<StringToken *>(&other);

    if (literals.size() != other_string->get_vector().size())
        return false;

    for (int i=0; i < literals.size(); i++) {
        Token *this_literal = literals[i].get();
        Token *other_literal = other_string->get_vector()[i].get();
        if (*this_literal != *other_literal)
            return false;
    }
    return true;

}

bool StringToken::operator!=(Token &other) {
    return !(this->operator==(other));
}

const std::vector<std::shared_ptr<Token>> &StringToken::get_vector() const {
    return literals;
}


std::string StringToken::to_str() const {
    std::string output;
    for (const auto& token : literals) {
        if (auto text = dynamic_cast<TextToken *>(token.get())) {
            output += text->to_str();
        } else if (auto link = dynamic_cast<LinkToken *>(token.get())) {
            output += "${" + link->to_str() + "}";
        }
    }
    return output;
}



