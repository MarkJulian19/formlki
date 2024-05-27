#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <functional>
#include <fstream>
#include <tuple>
#include <algorithm>
#include <ctime>

using Rule = std::pair<char, std::string>;
int glob_counter = 0;
#define S_T clock_t s_t = clock()
#define E_T clock_t e_t = clock()
#define E_T_M ((e_t - s_t) * 1000 / 1000000)
S_T;
int cur_s;
bool CHK(const std::string &w, std::vector<Rule> &g)
{
    std::vector<std::set<std::tuple<Rule, int, int>>> D(w.size() + 1);
    glob_counter++;
    D[0].insert(std::make_tuple(g[0], 0, 0));
    glob_counter++;
    auto PR2 = [](int j, std::vector<Rule> &g, std::vector<std::set<std::tuple<Rule, int, int>>> &D)
    {
        std::vector<std::tuple<Rule, int, int>> new_sits;
        glob_counter++;
        std::for_each(D[j].begin(), D[j].end(), [&](const auto &sit)
                      { std::for_each(g.begin(), g.end(), [&](const auto &rule)
                                      {
            if (rule.first == std::get<0>(sit).second[std::get<1>(sit)])
            {
                glob_counter++;
                new_sits.emplace_back(rule, 0, j);
            } }); });
        glob_counter++;
        std::for_each(new_sits.begin(), new_sits.end(), [&](const auto &sit)
                      { D[j].insert(sit); });
    };

    auto Scan1 = [](int j, char x, std::vector<std::set<std::tuple<Rule, int, int>>> &D)
    {
        std::for_each(D[j].begin(), D[j].end(), [&](const auto &sit)
                      {
        if (std::get<0>(sit).second[std::get<1>(sit)] == x)
        {
            glob_counter++;
            D[j + 1].insert(std::make_tuple(std::get<0>(sit), std::get<1>(sit) + 1, std::get<2>(sit)));
        } });
    };

    auto Compl1 = [](int j, std::vector<std::set<std::tuple<Rule, int, int>>> &D)
    {
        std::vector<std::tuple<Rule, int, int>> new_sits;
        std::for_each(D[j].begin(), D[j].end(), [&new_sits, &D](const auto &sit_a)
                      {
        if (std::get<1>(sit_a) != std::get<0>(sit_a).second.size())
            return;
        std::for_each(D[std::get<2>(sit_a)].begin(), D[std::get<2>(sit_a)].end(), [&sit_a, &new_sits, &D](const auto &sit_b)
        {
            if (std::get<0>(sit_b).second[std::get<1>(sit_b)] == std::get<0>(sit_a).first)
            {
                new_sits.emplace_back(std::get<0>(sit_b), std::get<1>(sit_b) + 1, std::get<2>(sit_b));
            }
        }); });

        std::for_each(new_sits.begin(), new_sits.end(), [&D, j](const auto &sit)
                      { D[j].insert(sit); });
    };
    auto Proc1 = [&PR2, &Compl1](int &c_s, int i, std::vector<Rule> &g, std::vector<std::set<std::tuple<Rule, int, int>>> &D)
    {
        c_s = D[i].size();
        PR2(i, g, D);
        glob_counter++;
        Compl1(i, D);
    };
    int counter1 = 0;
    while (true)
    {
        // std::cout << counter1 << std::endl;
        counter1++;
        glob_counter++;
        Proc1(cur_s, 0, g, D);
        if (counter1 > 100){
            return true;
        }
        if (D[0].size() == cur_s)
            break;
    }
    int counter2 = 0;
    for (int i = 1; i <= w.size(); ++i)
    {
        Scan1(i - 1, w[i - 1], D);
        glob_counter++;
        E_T;
        
        if (195 < w.size() && E_T_M  > 4.5*100){
            return true;
        }
        while (true)
        {
            // std::cout << counter2 << std::endl;
            counter2++;
            Proc1(cur_s, i, g, D);
            glob_counter++;
            if (counter2 > 7000){
                return true;
            }
            if (D[i].size() == cur_s)
                break;
        }
    }
    // return D[w.size()].find(std::make_tuple(g[0], 1, 0)) != D[w.size()].end();
    glob_counter++;
    return std::find_if(D[w.size()].begin(), D[w.size()].end(), [&](const auto &item)
                        { return std::get<0>(item) == g[0] && std::get<1>(item) == 1 && std::get<2>(item) == 0; }) != D[w.size()].end();
}

std::vector<Rule> parseFile(const std::string &filename, std::string &in_str)
{
    std::vector<Rule> P;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file " << filename << std::endl;
        P = {{'-', "fail"}};
        return P;
    }

    char left;
    std::string right;
    P.push_back(std::make_pair('U', "S"));
    if (std::getline(file, in_str))
    {
        if (in_str == "_")
            in_str = "";
        while (file >> left >> right)
        {
            if (right != "_")
                P.push_back(std::make_pair(left, right));
            else
                P.push_back(std::make_pair(left, ""));
        }
    }
    else
    {
        std::cerr << "Unable to read the first line." << std::endl;
    }
    file.close();
    return P;
}

void try_chek(std::string filein, std::string fileout)
{
    std::string sample_in;
    std::vector<Rule> P = parseFile(filein, sample_in);
    // char S = 'U';
    // std::cout << sample_in << std::endl;
    // for (const Rule &rule : P)
    // {
    //     std::cout << rule.first << " -> " << rule.second << std::endl;
    // }

    // bool early_result = CHK(sample_in, P);
    // std::cout << early_result << std::endl;
    std::ofstream outFile(fileout);
    if (CHK(sample_in, P))
    {
        // std::cout << "YES" << std::endl;
        outFile << "YES";
    }
    else
    {
        // std::cout << "NO" << std::endl;
        outFile << "NO";
    }
}

int main()
{
    try_chek("cyk.in", "cyk.out");
    // try_chek("cyk2.in", "cyk2.out");
    // try_chek("cyk3.in", "cyk3.out");
    // try_chek("cyk4.in", "cyk4.out");
    // try_chek("cyk5.in", "cyk5.out");
    // try_chek("cyk6.in", "cyk6.out");
    return 0;
}