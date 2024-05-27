#include "api.hpp"
#include <string>
#include <iostream>
#include <cctype>
#include <vector>
#include <set>
#include <algorithm>
#include <climits>
#include <unordered_map>
#include <stack>

std::vector<std::set<int>> global_followpos;
int counter;
struct CharNode
{
    char value;
    int index;

    CharNode(char value, int index) : value(value), index(index) {}
};

struct OperatorNode
{
    char value;
    int index;
    int operator_priority;
    OperatorNode(char value, int index, int priority) : value(value), index(index), operator_priority(priority) {}
};

std::vector<CharNode> char_nodes;
std::vector<OperatorNode> operator_nodes;
struct TreeNode
{
    char elem;
    int index;
    TreeNode *left = NULL;
    TreeNode *right = NULL;
    char nullable = 'N';
    std::set<int> firstpos, lastpos;

    TreeNode(char elem, int index, TreeNode *left, TreeNode *right) : elem(elem), index(index), left(left), right(right) {}

    ~TreeNode()
    {
        delete left;
        delete right;
    }

    // void cal_n_f_l_p();
    void cal_null();
    void cal_f_l_p();
    void calculate_global_followpos();
};
void TreeNode::cal_null()
{
    counter++;
    if (elem == '\n')

        // nullable = true;
        nullable = 'T';
    else if (elem == '*')
    {
        nullable = 'T';
    }
    else if (elem == '|')
    {

        if (left->nullable == 'T' || right->nullable == 'T')
        {
            nullable = 'T';
        }
        else
        {
            nullable = 'F';
        }
    }
    else if (elem == '\'')
    {
        // nullable = left->nullable && right->nullable;
        if (left->nullable == 'T' && right->nullable == 'T')
        {
            nullable = 'T';
        }
        else
        {
            nullable = 'F';
        }
    }
    else if (std::isalnum(elem) || elem == ']')
    {
        if (elem == ']')
        {
            nullable = 'T';
        }
        else
        {
            nullable = 'F';
        }
    }
}
void TreeNode::cal_f_l_p()
{

    if (elem == '*')
    {
        counter++;
        firstpos = left->firstpos;
        lastpos = left->lastpos;
    }
    else if (elem == '|')
    {
        counter++;
        firstpos = left->firstpos;
        firstpos.insert(right->firstpos.begin(), right->firstpos.end());

        lastpos = left->lastpos;
        lastpos.insert(right->lastpos.begin(), right->lastpos.end());
    }
    else if (elem == '\'')
    {
        counter++;
        firstpos = left->firstpos;
        if (left->nullable == 'T')
        {
            firstpos.insert(right->firstpos.begin(), right->firstpos.end());
        }
        lastpos = right->lastpos;
        if (right->nullable == 'T')
        {
            lastpos.insert(left->lastpos.begin(), left->lastpos.end());
        }
    }
    else if (std::isalnum(elem) || elem == ']')
    {

        firstpos.emplace(index);
        counter++;
        lastpos.emplace(index);
    }
}


void TreeNode::calculate_global_followpos()
{
    std::stack<TreeNode *> nodesStack;
    nodesStack.push(this);
    counter++;
    while (!nodesStack.empty())
    {
        counter++;
        TreeNode *currentNode = nodesStack.top();
        nodesStack.pop();
        counter++;
        if (currentNode->left)
        {
            nodesStack.push(currentNode->left);
        }
        if (currentNode->right)
        {
            nodesStack.push(currentNode->right);
        }
        counter++;
        if (currentNode->elem == '*' || currentNode->elem == '\'')
        {
            for (auto &item : (currentNode->elem == '*' ? currentNode->left->lastpos : currentNode->left->lastpos))
            {
                counter++;
                global_followpos[item].insert((currentNode->elem == '*' ? currentNode->left->firstpos : currentNode->right->firstpos).begin(),
                                              (currentNode->elem == '*' ? currentNode->left->firstpos : currentNode->right->firstpos).end());
            }
        }
    }
}

int find_min_index(int start, int end)
{
    int min_index = -1;
    for (int i = 0; i < operator_nodes.size(); i++)
    {
        counter++;
        if (operator_nodes[i].index >= start && operator_nodes[i].index <= end && operator_nodes[i].operator_priority != 0 && (min_index == -1 || operator_nodes[i].operator_priority <= operator_nodes[min_index].operator_priority))
        {
            counter++;
            min_index = i;
        }
    }
    return min_index;
}

void processSubstring(const std::string &s, int &start, int &end)
{
    counter++;
    int offset = 0;
    while (s[start + offset] == '(' && s[end - offset] == ')')
    {
        counter++;
        offset++;
        int count = 0;
        int subStart = start + offset;
        int subEnd = end - offset;

        while (subStart <= subEnd)
        {
            counter++;
            if (s[subStart] == '(')
            {
                count++;
            }
            else if (s[subStart] == ')')
            {
                counter++;
                count--;
                if (count < 0)
                {
                    break;
                }
            }
            subStart++;
        }

        if (subStart < subEnd)
        {
            offset--;
            break;
        }
    }
    counter++;
    start += offset;
    end -= offset;
}

TreeNode *rec_TreeNode(int end, int start, const std::string &s)
{
    // std::cout << s.substr(start, end - start + 1) << ' ' << start << ' ' << end << std::endl;
    counter++;
    processSubstring(s, start, end);
    int min_index = find_min_index(start, end);
    // std::cout << min_index << std::endl;
    TreeNode *treeNode = NULL;
    if (min_index != -1)
    {
        counter++;
        treeNode = new TreeNode(operator_nodes[min_index].value, 0, rec_TreeNode(operator_nodes[min_index].index - 1, start, s), NULL);
        if (operator_nodes[min_index].value != '*')
        {
            treeNode->right = rec_TreeNode(end, operator_nodes[min_index].index + 1, s);
        }
        counter++;
    }
    else if (start == end)
    {
        int index_in_char_index = 0;
        auto it = std::find_if(char_nodes.begin(), char_nodes.end(), [start](const CharNode &node)
                               { return node.index == start; });
        counter++;
        if (it != char_nodes.end())
        {
            counter++;
            index_in_char_index = std::distance(char_nodes.begin(), it);
        }
        treeNode = new TreeNode(s[start], index_in_char_index, NULL, NULL);
    }
    else if (start > end)
    {
        counter++;
        treeNode = new TreeNode('\n', -1000000, NULL, NULL);
    }
    // treeNode->cal_n_f_l_p();
    treeNode->cal_null();
    treeNode->cal_f_l_p();

    // TreeNode->calculate_global_followpos();
    return treeNode;
}

int getPriority(char symbol)
{
    counter++;
    std::unordered_map<char, int> priorityMap = {
        {'(', 4},
        {')', -4},
        {'|', 1},
        {'\'', 2},
        {'*', 3}};

    if (priorityMap.find(symbol) != priorityMap.end())
    {
        counter++;
        return priorityMap[symbol];
    }
    return 0;
}
void create_TreeNode(const std::string &s)
{
    counter++;
    int cur_priority = 0;
    for (size_t i = 0; i < s.size(); i++)
    {
        counter++;
        if (std::isalnum(s[i]) || s[i] == ']')
        {
            counter++;
            char_nodes.emplace_back(s[i], i);
        }
        else if (s[i] == '(' || s[i] == ')')
        {
            counter++;
            cur_priority += (s[i] == '(' ? 4 : -4);
            operator_nodes.emplace_back(s[i], i, 0);
        }
        else
        {
            counter++;
            operator_nodes.emplace_back(s[i], i, getPriority(s[i]) + cur_priority);
        }
        // }
    }
    counter++;
    // Output char nodes
    // for (const auto &node : char_nodes)
    // {
    //     std::cout << node.value << "  ";
    // }
    // std::cout << std::endl;

    // for (const auto &node : char_nodes)
    // {
    //     std::cout << node.index << "  ";
    // }
    // std::cout << std::endl;

    // // Output operator nodes
    // for (const auto &node : operator_nodes)
    // {
    //     std::cout << node.value << "  ";
    // }
    // std::cout << std::endl;

    // for (const auto &node : operator_nodes)
    // {
    //     std::cout << node.index << "  ";
    // }
    // std::cout << std::endl;

    // // Output operator priorities
    // for (const auto &node : operator_nodes)
    // {
    //     std::cout << node.operator_priority << "  ";
    // }
    // std::cout << std::endl;

    // Call other functions with updated char_nodes and operator_nodes
}
std::string processString(const std::string &s)
{
    counter++;
    std::string s1;
    s1.reserve(s.size() * 2); // Резервируем достаточно места, чтобы избежать лишних реаллокаций
    s1.push_back('(');
    s1.append(s);
    s1.push_back(')');
    s1.push_back(']');

    for (int i = 1; i < s1.size(); ++i)
    {
        counter++;
        if (s1[i] != '*' && s1[i] != '|' && s1[i - 1] != '|' && s1[i] != ')' && s1[i - 1] != '(')
        {
            counter++;
            s1.insert(i, "\'");
            ++i; // Увеличиваем i, чтобы пропустить только что вставленный символ '.'
        }
    }

    return s1;
}

std::string to_string(int num)
{
    counter++;
    std::string str;
    if (num == 0)
    {
        counter++;
        str.push_back('0');
    }
    else
    {
        counter++;
        std::string tmp_str;
        while (num != 0)
        {
            tmp_str.push_back('0' + num % 10);
            num /= 10;
        }
        for (size_t i = tmp_str.size(); i > 0; --i)
        {
            str.push_back(tmp_str[i - 1]);
        }
    }
    return str;
}

void processStates(DFA &ansver, const Alphabet &alph, TreeNode *root, const std::string &s1)
{
    std::vector<std::set<int>> stat;
    stat.push_back(root->firstpos);

    for (int index = 0; index < stat.size(); index++)
    {
        counter++;
        std::vector<std::set<int>::iterator> matching_state_iters;
        for (const char &alpha_from_alph : alph)
        {
            counter++;
            std::set<int> new_s;
            for (const int &in_al : stat[index])
            {
                counter++;
                if (s1[char_nodes[in_al].index] == alpha_from_alph)
                {
                    new_s.insert(global_followpos[in_al].begin(), global_followpos[in_al].end());
                }
            }
            counter++;
            // Check if new_s is not in stat
            bool state_exists = false;
            for (const auto &state : stat)
            {
                counter++;
                bool found = true;
                if (state.size() == new_s.size())
                {
                    counter++;
                    for (const auto &elem : new_s)
                    {
                        counter++;
                        if (state.find(elem) == state.end())
                        {
                            counter++;
                            found = false;
                            break;
                        }
                    }
                }
                else
                {
                    counter++;
                    found = false;
                }

                if (found)
                {
                    counter++;
                    state_exists = true;
                    break;
                }
            }

            if (!new_s.empty() && !state_exists)
            {
                counter++;
                // New state
                int index_to = stat.size();
                stat.push_back(new_s);
                ansver.create_state(to_string(index_to), new_s.count(char_nodes.size() - 1) == 1);
                ansver.set_trans(to_string(index), alpha_from_alph, to_string(index_to));
                continue;
            }
            else if (!new_s.empty())
            {
                counter++;
                // Existing state
                int index_to = -1;
                for (int i = 0; i < stat.size(); i++)
                {
                    if (stat[i] == new_s)
                    {
                        index_to = i;
                        break;
                    }
                }
                if (index_to != -1)
                {
                    counter++;
                    ansver.set_trans(to_string(index), alpha_from_alph, to_string(index_to));
                }
            }
        }
    }
}

DFA re2dfa(const std::string &s)
{
    counter++;
    // Alphabet alphabet = Alphabet(s);
    // DFA res = DFA(alphabet);
    if (s.empty())
    {
        counter++;
        DFA zero_dFA = DFA("0");
        zero_dFA.create_state("0", true);
        zero_dFA.set_initial("0");
        return zero_dFA;
    }
       Alphabet alphabet = Alphabet(s);
    DFA res = DFA(alphabet);
    std::string s1 = processString(s);
    // std::cout << s1 << std::endl;
    create_TreeNode(s1);
    TreeNode *root = rec_TreeNode(s1.size() - 1, 0, s1);
    counter++;
    global_followpos = std::vector<std::set<int>>(char_nodes.size());
    counter++;
    res.create_state("0", root->nullable == 'T');
    res.set_initial("0");
    counter++;
    root->calculate_global_followpos();
    counter++;
    processStates(res, alphabet, root, s1);
    counter++;
    return res;
}