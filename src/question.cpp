#include "question.h"
#include <algorithm>
#include <cctype>
#include <cmath>

std::string Question::trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

static std::string toLower(std::string s) {
    for (char& c : s) c = (char)tolower((unsigned char)c);
    return s;
}

// --- MCQ ---

void McqQuestion::show(std::ostream& out) const {
    out << getText() << "\n";
    for (size_t i = 0; i < options.size(); i++) {
        out << "  " << (char)('A' + i) << ". " << options[i] << "\n";
    }
    out << "Your answer (A/B/C/D): ";
}

bool McqQuestion::isCorrect(const std::string& answer) const {
    std::string a = toLower(trim(answer));
    if (a.empty()) return false;
    int picked = -1;
    if (a[0] >= 'a' && a[0] <= 'z') picked = a[0] - 'a';
    else if (a[0] >= '1' && a[0] <= '9') picked = a[0] - '1';
    return picked == answerIndex;
}

std::string McqQuestion::correctAnswer() const {
    if (answerIndex < 0 || answerIndex >= (int)options.size()) return "";
    return std::string(1, (char)('A' + answerIndex)) + ". " + options[answerIndex];
}

// --- True / False ---

void TrueFalseQuestion::show(std::ostream& out) const {
    out << getText() << "\nYour answer (true/false): ";
}

bool TrueFalseQuestion::isCorrect(const std::string& input) const {
    std::string a = toLower(trim(input));
    if (a == "true" || a == "t" || a == "1" || a == "yes") return answer == true;
    if (a == "false" || a == "f" || a == "0" || a == "no") return answer == false;
    return false;
}

std::string TrueFalseQuestion::correctAnswer() const {
    return answer ? "true" : "false";
}

// --- Numeric ---

void NumericQuestion::show(std::ostream& out) const {
    out << getText() << "\nYour answer (number): ";
}

bool NumericQuestion::isCorrect(const std::string& input) const {
    try {
        std::string t = trim(input);
        size_t used = 0;
        double value = std::stod(t, &used);
        if (used != t.size()) return false;
        return fabs(value - answer) < 0.000001;
    } catch (...) {
        return false;
    }
}

std::string NumericQuestion::correctAnswer() const {
    // Print 60 instead of 60.000000 for whole numbers.
    if (fabs(answer - round(answer)) < 0.000000001) {
        return std::to_string((long long)llround(answer));
    }
    return std::to_string(answer);
}
