// Mini Computer-Based Test Engine
// Run: ./test_engine [questions.txt] [--shuffle]

#include "question_bank.h"
#include "test_session.h"
#include <fstream>
#include <iomanip>
#include <iostream>

static void printResult(const Result& r) {
    std::cout << "\n========== RESULT ==========\n";
    std::cout << "Student: " << r.student << "\n";
    std::cout << "Score: " << r.obtained << " / " << r.total
              << " (" << std::fixed << std::setprecision(1) << r.percentage() << "%)\n";
    std::cout << "Time taken: " << r.seconds << " second(s)\n";
    std::cout << "\nPer-question review:\n";
    for (const Answer& a : r.answers) {
        std::cout << "  Q" << a.questionId << ": " << (a.correct ? "Correct" : "Wrong")
                  << " | your answer: '" << a.given << "' | scored: " << a.scored << "\n";
    }
    std::cout << "============================\n";
}

int main(int argc, char* argv[]) {
    std::string filename = "questions.txt";
    bool shuffle = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--shuffle") shuffle = true;
        else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: " << argv[0] << " [questions.txt] [--shuffle]\n";
            return 0;
        } else {
            filename = arg;
        }
    }

    QuestionBank bank;
    std::string error;
    if (!bank.loadFromFile(filename, error)) {
        std::cerr << "Error: " << error << "\n";
        return 1;
    }

    std::cout << "Mini Computer-Based Test Engine\n";
    std::cout << "Loaded " << bank.size() << " question(s) from " << filename << ".\n";
    std::cout << "Enter your name: ";
    std::string name;
    std::getline(std::cin, name);
    if (name.empty()) name = "Student";

    TestSession session(bank, name, shuffle);
    Result result = session.run(std::cin, std::cout);
    printResult(result);

    // Save a one-line summary. If this fails, the test still counts.
    std::ofstream out("results.csv", std::ios::app);
    if (out) {
        out << result.student << "," << result.obtained << "/" << result.total << ","
            << std::fixed << std::setprecision(1) << result.percentage() << "%,"
            << result.seconds << "s\n";
        std::cout << "Result also saved to results.csv\n";
    }
    return 0;
}
