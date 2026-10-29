# Mini Computer-Based Test Engine

A small terminal app for taking a test in C++. A teacher writes
questions in a text file, a student runs the program, answers
one by one, and gets a score at the end. No database, no web
server, just files and C++.

## How to run

```bash
make
./test_engine questions.txt
```

Or without make:

```bash
./build.sh
./test_engine questions.txt
./test_engine questions.txt --shuffle
```

## How it works

1. `questions.txt` is loaded when the program starts.
2. You type your name. Leave it blank and it uses "Student".
3. Questions come one at a time. MCQ shows A/B/C/D options,
   true/false asks for true/false, numeric asks for a number.
4. After each answer you get told right away if it was correct.
   If it was wrong, the correct answer is shown.
5. At the end you get total score, percentage, time taken, and
   a review of every question. The result is also added to
   `results.csv`.

There is no back button. Once you press Enter, that answer is
locked in. A blank answer just counts as wrong.

## Question file format

One question per line, fields separated by `|`. Lines starting
with `#` and blank lines are ignored.

```
MCQ|1|Which C++ smart pointer owns an object alone?|1|shared_ptr;unique_ptr;weak_ptr;raw pointer|1
TF|6|Base destructors should be virtual when deleting via base pointer.|1|true
NUM|10|What is 10 / 4?|1|2.5
```

MCQ options are separated by `;` and the correct index starts
at 0, so `1` means the second option. TF also accepts t/f, yes/no,
1/0. NUM allows a tiny difference, so 2.5 and 2.50 both pass.

## Folder structure

```
mini-test-engine/
  README.md            this file
  Makefile             make / make clean / make run
  build.sh             same build, without make
  questions.txt        sample bank, 10 questions, 11 marks
  results.csv          created when a test finishes (gitignored)
  include/
    question.h         Question base class + MCQ, TF, Numeric
    question_bank.h    owns the questions, loads the file
    test_session.h     runs one test, builds the result
  src/
    question.cpp       how each type shows and checks answers
    question_bank.cpp  file parsing and validation
    test_session.cpp   order, prompting, scoring, timing
    main.cpp           command line handling, result printing
```

## Design notes

The engine only talks to the `Question` base class. It calls
`show()` and `isCorrect()` and doesn't care which type it is.
That is why adding a new question type is just one new class
plus one branch in the file parser.

Questions live in `vector<unique_ptr<Question>>` inside
`QuestionBank`. When the bank goes out of scope everything is
freed automatically, there is no manual `delete` in the code.
`TestSession` only borrows the bank, it doesn't own it.
`Question` also has a virtual destructor, otherwise deleting
a derived question through a base pointer would be wrong.

Lookup by question id uses an `unordered_map`, so `findById`
is O(1) on average instead of scanning the list.

## Tested

Built with `-Wall -Wextra -Werror`, no warnings. Also run
under AddressSanitizer + UndefinedBehaviorSanitizer with no
errors reported.

- All correct answers: 11 / 11
- All wrong answers: 0 / 11
- Mixed answers, lowercase letters, number answers (2 for B),
  and 2.50 for 2.5: 11 / 11
- Early EOF, blank name, blank answers: no crash, blanks count
  as wrong, blank name becomes "Student"
- Missing file, malformed line, unknown type: clean error,
  exit code 1
- Duplicate question id: warning printed, second one skipped
- Windows-style CRLF file: loads and scores correctly
- `--shuffle`: first question changes from run to run

## Ideas to extend

- Negative marking for wrong answers
- A time limit per test
- A new question type, for example fill in the blank
- Ranking students from `results.csv`
