#include "test_session.h"
#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>

Result TestSession::run(std::istream& in, std::ostream& out) {
    std::vector<int> order(bank.size());
    std::iota(order.begin(), order.end(), 0);
    if (shuffle) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(order.begin(), order.end(), gen);
    }

    Result result;
    result.student = student;
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < (int)order.size(); i++) {
        const Question& q = bank.get(order[i]);
        out << "\nQ" << (i + 1) << " [" << q.getMarks() << " mark(s)]\n";
        q.show(out);

        std::string answer;
        if (!std::getline(in, answer)) answer = "";  // EOF = wrong, not a crash

        bool ok = q.isCorrect(answer);
        double scored = ok ? q.getMarks() : 0;
        result.total += q.getMarks();
        result.obtained += scored;
        result.answers.push_back({q.getId(), answer, ok, scored});

        if (ok) out << "Correct!\n";
        else out << "Wrong. Correct answer: " << q.correctAnswer() << "\n";
    }

    auto end = std::chrono::steady_clock::now();
    result.seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    return result;
}
