#include "question_bank.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

static std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(s);
    std::string part;
    while (std::getline(ss, part, delim)) parts.push_back(part);
    return parts;
}

static std::string trimStr(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

static bool toInt(const std::string& s, int& out) {
    try {
        std::string t = trimStr(s);
        size_t used = 0;
        out = std::stoi(t, &used);
        return used == t.size();
    } catch (...) {
        return false;
    }
}

static bool toDouble(const std::string& s, double& out) {
    try {
        std::string t = trimStr(s);
        size_t used = 0;
        out = std::stod(t, &used);
        return used == t.size();
    } catch (...) {
        return false;
    }
}

static bool toBool(const std::string& s, bool& out) {
    std::string t = trimStr(s);
    for (char& c : t) c = (char)tolower((unsigned char)c);
    if (t == "true" || t == "t" || t == "1") { out = true; return true; }
    if (t == "false" || t == "f" || t == "0") { out = false; return true; }
    return false;
}

void QuestionBank::add(std::unique_ptr<Question> q) {
    if (!q) return;
    int id = q->getId();
    if (indexById.count(id)) {
        std::cerr << "Warning: duplicate question id " << id << ", skipping it.\n";
        return;
    }
    indexById[id] = (int)questions.size();
    questions.push_back(std::move(q));
}

const Question* QuestionBank::findById(int id) const {
    auto it = indexById.find(id);
    if (it == indexById.end()) return nullptr;
    return questions[it->second].get();
}

bool QuestionBank::loadFromFile(const std::string& filename, std::string& error) {
    std::ifstream file(filename);
    if (!file) {
        error = "Cannot open question file: " + filename;
        return false;
    }

    std::string line;
    int lineNo = 0;
    while (std::getline(file, line)) {
        lineNo++;
        std::string t = trimStr(line);
        if (t.empty() || t[0] == '#') continue;

        std::vector<std::string> f = split(line, '|');
        if (f.size() < 4) {
            error = "Line " + std::to_string(lineNo) + ": not enough fields";
            return false;
        }

        std::string type = trimStr(f[0]);
        int id;
        double marks;
        if (!toInt(f[1], id) || !toDouble(f[3], marks) || marks <= 0) {
            error = "Line " + std::to_string(lineNo) + ": bad id or marks";
            return false;
        }
        std::string text = trimStr(f[2]);
        if (text.empty()) {
            error = "Line " + std::to_string(lineNo) + ": empty question text";
            return false;
        }

        if (type == "MCQ") {
            if (f.size() != 6) {
                error = "Line " + std::to_string(lineNo) + ": MCQ needs 6 fields";
                return false;
            }
            std::vector<std::string> options = split(f[4], ';');
            for (std::string& o : options) o = trimStr(o);
            int correct;
            if (options.size() < 2 || !toInt(f[5], correct) ||
                correct < 0 || correct >= (int)options.size()) {
                error = "Line " + std::to_string(lineNo) + ": bad MCQ options or answer index";
                return false;
            }
            add(std::make_unique<McqQuestion>(id, text, marks, options, correct));
        } else if (type == "TF") {
            if (f.size() != 5) {
                error = "Line " + std::to_string(lineNo) + ": TF needs 5 fields";
                return false;
            }
            bool correct;
            if (!toBool(f[4], correct)) {
                error = "Line " + std::to_string(lineNo) + ": bad TF answer";
                return false;
            }
            add(std::make_unique<TrueFalseQuestion>(id, text, marks, correct));
        } else if (type == "NUM") {
            if (f.size() != 5) {
                error = "Line " + std::to_string(lineNo) + ": NUM needs 5 fields";
                return false;
            }
            double correct;
            if (!toDouble(f[4], correct)) {
                error = "Line " + std::to_string(lineNo) + ": bad NUM answer";
                return false;
            }
            add(std::make_unique<NumericQuestion>(id, text, marks, correct));
        } else {
            error = "Line " + std::to_string(lineNo) + ": unknown type '" + type + "'";
            return false;
        }
    }

    if (questions.empty()) {
        error = "No questions found in: " + filename;
        return false;
    }
    return true;
}
