/*
 * CS106L Assignment 2: Marriage Pact
 * Created by Haven Whitney with modifications by Fabio Ibanez & Jacob Roberts-Baca.
 *
 * Welcome to Assignment 2 of CS106L! Please complete each STUDENT TODO
 * in this file. You do not need to modify any other files.
 *
 */

#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>

std::string kYourName = "Ascend Ho"; // Don't forget to change this!

/**
 * Takes in a file name and returns a set containing all of the applicant names as a set.
 *
 * @param filename  The name of the file to read.
 *                  Each line of the file will be a single applicant's name.
 * @returns         A set of all applicant names read from the file.
 *
 * @remark Feel free to change the return type of this function (and the function
 * below it) to use a `std::unordered_set` instead. If you do so, make sure
 * to also change the corresponding functions in `utils.h`.
 */
std::set<std::string> get_applicants(std::string filename) {
  std::set<std::string> applicants;
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    return applicants;
  }

  std::string line;
  const std::string whitespace = " \t\r\n";
  while (std::getline(infile, line)) {
    auto start = line.find_first_not_of(whitespace);
    if (start == std::string::npos) continue; // empty or all-whitespace line
    auto end = line.find_last_not_of(whitespace);
    std::string name = line.substr(start, end - start + 1);
    if (!name.empty()) applicants.insert(name);
  }

  return applicants;
}

/**
 * Takes in a set of student names by reference and returns a queue of names
 * that match the given student name.
 *
 * @param name      The returned queue of names should have the same initials as this name.
 * @param students  The set of student names.
 * @return          A queue containing pointers to each matching name.
 */
std::queue<const std::string*> find_matches(std::string name, std::set<std::string>& students) {
  std::queue<const std::string*> matches;

  // Helper: compute initials (first char of each word, case-insensitive)
  auto initials_of = [](const std::string& s) {
    std::string initials;
    bool take = true;
    for (char c : s) {
      if (take && !std::isspace(static_cast<unsigned char>(c))) {
        initials.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
        take = false;
      }
      if (std::isspace(static_cast<unsigned char>(c))) take = true;
    }
    return initials;
  };

  std::string target = initials_of(name);
  for (const auto& student : students) {
    if (initials_of(student) == target) {
      matches.push(&student);
    }
  }

  return matches;
}

/**
 * Takes in a queue of pointers to possible matches and determines the one true match!
 *
 * You can implement this function however you'd like, but try to do something a bit
 * more complicated than a simple `pop()`.
 *
 * @param matches The queue of possible matches.
 * @return        Your magical one true love.
 *                Will return "NO MATCHES FOUND." if `matches` is empty.
 */
std::string get_match(std::queue<const std::string*>& matches) {
  if (matches.empty()) return "NO MATCHES FOUND.";

  // Choose the shortest name (fewest characters). Tie-breaker: lexicographical order.
  const std::string* best = nullptr;
  std::vector<const std::string*> items;
  while (!matches.empty()) {
    items.push_back(matches.front());
    matches.pop();
  }

  for (const std::string* p : items) {
    if (p == nullptr) continue;
    if (best == nullptr) {
      best = p;
      continue;
    }
    if (p->size() < best->size()) best = p;
    else if (p->size() == best->size() && *p < *best) best = p;
  }

  if (best) return *best;
  return "NO MATCHES FOUND.";
  
}

/* #### Please don't remove this line! #### */
#include "autograder/utils.hpp"
