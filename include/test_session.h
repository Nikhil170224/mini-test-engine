#pragma once

#include "question_bank.h"
#include <istream>
#include <ostream>
#include <string>
#include <vector>

struct Answer {
    int questionId = 0;
    std::string given;
    bool correct = false;
    double scored = 0;
};

struct Result {
    std::string student;
    double obtained = 0;
    double total = 0;
    long long seconds = 0;
    std::vector<Answer> answers;

    double percentage() const {
        return total > 0 ? (obtained / total) * 100.0 : 0;
    }
};

// Runs one test. It borrows the bank, it doesn't own it.
class TestSession {
public:
    TestSession(const QuestionBank& bank, std::string student, bool shuffle)
        : bank(bank), student(student), shuffle(shuffle) {}

    Result run(std::istream& in, std::ostream& out);

private:
    const QuestionBank& bank;
    std::string student;
    bool shuffle;
};
