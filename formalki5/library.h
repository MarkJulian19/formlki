#ifndef ALGO_EARLY_H
#define ALGO_EARLY_H

#include <vector>
#include <iostream>
#include <set>

namespace lib {
    using Rule = std::pair<char, std::string>;

    class Grammar {
    private:
        std::vector<char> N;
        std::vector<char> sigma;
        std::vector<Rule> P;
        char S;
    public:
        Grammar(std::vector<char> N,
                std::vector<char> sigma,
                std::vector<Rule> P,
                char S) : N(N), sigma(sigma), P(P), S(S) {}

        const std::vector<Rule>& getRules() const {
            return P;
        }
    };

    class Early {
    public:
        Early(const Grammar &g) : g(g) {}

        bool Check(const std::string& s) {
            w = s;

            D.clear();
            D.resize(w.size() + 1);

            D[0].insert(Situation(g.getRules()[0], 0, 0));
            size_t current_size;

            do {
                current_size = D[0].size();
                Predict(0);
                Complete(0);
            } while (D[0].size() != current_size);

            for (size_t i = 1; i <= w.size(); ++i) {
                Scan(i - 1, w[i - 1]);
                do {
                    current_size = D[i].size();
                    Predict(i);
                    Complete(i);
                } while (D[i].size() != current_size);
            }

            return D[w.size()].find(Situation(g.getRules()[0], 1, 0)) != D[w.size()].end();
        }
    private:
        struct Situation {
            Rule rule;
            size_t point;
            size_t i;

            Situation(Rule rule, size_t point, size_t i) : rule(rule), point(point), i(i) {};

            bool operator<(const Situation& another) const {
                if (rule == another.rule) {
                    if (point == another.point) {
                        return i < another.i;
                    }
                    return point < another.point;
                }
                return rule < another.rule;
            }
        };

        std::string w;
        std::vector<std::set<Situation>> D;
        Grammar g;

        void Predict(size_t j) {
            std::vector<Situation> new_situations;
            for (auto situation : D[j]) {
                for (auto rule : g.getRules()) {
                    if (rule.first == situation.rule.second[situation.point]) {
                        new_situations.push_back(Situation(rule, 0, j));
                    }
                }
            }

            for (auto situation : new_situations) {
                D[j].insert(situation);
            }
        }

        void Scan(size_t j, char x) {
            for (auto situation : D[j]) {
                if (situation.rule.second[situation.point] == x) {
                    D[j + 1].insert(Situation(situation.rule, situation.point + 1, situation.i));
                }
            }
        }

        void Complete(size_t j) {
            std::vector<Situation> new_situations;
            for (auto situation_a : D[j]) {
                if (situation_a.point != situation_a.rule.second.size()) continue;
                for (auto situation_b : D[situation_a.i]) {
                    if (situation_b.rule.second[situation_b.point] == situation_a.rule.first) {
                        new_situations.push_back(Situation(situation_b.rule, situation_b.point + 1, situation_b.i));
                    }
                }
            }

            for (auto situation : new_situations) {
                D[j].insert(situation);
            }
        }
    };
}

#endif //ALGO_EARLY_H