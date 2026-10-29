#pragma once

#include <ostream>
#include <string>
#include <vector>

// Base class for every question type. The test code only talks to
// this interface, it doesn't care which type a question actually is.
class Question {
public:
    Question(int id, std::string text, double marks)
        : id(id), text(text), marks(marks) {}
    virtual ~Question() = default;

    // Questions are owned by the bank, so don't copy them around.
    Question(const Question&) = delete;
    Question& operator=(const Question&) = delete;

    int getId() const { return id; }
    double getMarks() const { return marks; }
    std::string getText() const { return text; }

    virtual void show(std::ostream& out) const = 0;
    virtual bool isCorrect(const std::string& answer) const = 0;
    virtual std::string correctAnswer() const = 0;

protected:
    static std::string trim(const std::string& s);

private:
    int id;
    std::string text;
    double marks;
};

// Multiple choice, one right option.
class McqQuestion : public Question {
public:
    McqQuestion(int id, std::string text, double marks,
                std::vector<std::string> options, int answerIndex)
        : Question(id, text, marks), options(options), answerIndex(answerIndex) {}

    void show(std::ostream& out) const override;
    bool isCorrect(const std::string& answer) const override;
    std::string correctAnswer() const override;

private:
    std::vector<std::string> options;
    int answerIndex;
};

class TrueFalseQuestion : public Question {
public:
    TrueFalseQuestion(int id, std::string text, double marks, bool answer)
        : Question(id, text, marks), answer(answer) {}

    void show(std::ostream& out) const override;
    bool isCorrect(const std::string& answer) const override;
    std::string correctAnswer() const override;

private:
    bool answer;
};

// Numeric answer, small tolerance so 2.50 and 2.5 both pass.
class NumericQuestion : public Question {
public:
    NumericQuestion(int id, std::string text, double marks, double answer)
        : Question(id, text, marks), answer(answer) {}

    void show(std::ostream& out) const override;
    bool isCorrect(const std::string& answer) const override;
    std::string correctAnswer() const override;

private:
    double answer;
};
