#pragma once

#include "question.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Owns all the questions. When this goes out of scope, the
// unique_ptrs free everything, no manual delete needed.
class QuestionBank {
public:
    // File format, one per line, fields separated by '|':
    //   MCQ|1|What is 2+2?|1|2;3;4;5|2
    //   TF|2|C++ has garbage collection?|1|false
    //   NUM|3|10 / 4 = ?|1|2.5
    // '#' lines and blank lines are skipped.
    bool loadFromFile(const std::string& filename, std::string& error);

    void add(std::unique_ptr<Question> q);

    int size() const { return (int)questions.size(); }
    bool empty() const { return questions.empty(); }
    const Question& get(int index) const { return *questions.at(index); }

    // Fast lookup by id, nullptr if not there.
    const Question* findById(int id) const;

private:
    std::vector<std::unique_ptr<Question>> questions;
    std::unordered_map<int, int> indexById;
};
