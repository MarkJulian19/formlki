#include "api.hpp"
#include <string>
#include <map>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>
using namespace std;
// int counter;
struct AutomationInfo
{
  map<vector<string>, string> RLS;
  set<string> del_s;
  set<string> in_s;
  set<string> out_st;
  string Q;
  string W;
  string res;
};

void cal_res(DFA &d, AutomationInfo &a)
{
  a.res.clear();

  // Check and add R
  a.W.empty() ? "" : (a.res += "(" + a.W + ")*");
  // counter++;
  a.Q.empty() ? "" : (a.res += "(");
  // counter++;
  d.is_final(d.get_initial_state()) ? (a.res += "|") : "";
  // counter++;
  a.res += a.Q;
  // counter++;
  a.Q.empty() ? "" : (a.res += ")");
}

string find_s(const string &s1, const string &s2, map<vector<string>, string> &RLS)
{
  // cout<<"find_s"<<endl;
  for (const auto &rule : RLS)
  {

    // counter++;
    vector<string> key = rule.first;
    if (key.size() == 2 && key[0] == s1 && key[1] == s2)
    {
      // counter++;
      return rule.second;
    }
  }
  // counter++;
  return "";
}

int my_c(set<string> &s, string &element)
{
  // cout<<"my_c"<<endl;
  int count = 0;
  for (const auto &el : s)
  {
    // counter++;
    if (el == element)
    {
      // counter++;
      count++;
    }
  }
  return count;
}
void process_data(DFA &d, const string &st1, const string &st2, AutomationInfo &Data, string &cur_s)
{
  // cout<<"process_data"<<endl;
  stringstream final_string_stream;
  // counter++;
  final_string_stream << (!find_s(st1, cur_s, Data.RLS).empty() ? "(" + find_s(st1, cur_s, Data.RLS) + ")" : "");
  final_string_stream << (!find_s(cur_s, cur_s, Data.RLS).empty() ? "(" + find_s(cur_s, cur_s, Data.RLS) + ")*" : "");
  final_string_stream << (!find_s(cur_s, st2, Data.RLS).empty() ? "(" + find_s(cur_s, st2, Data.RLS) + (d.is_final(cur_s) && st2 == "VO_CHTO_IA_WLIAPALSA_PAMAGITI_MNE" ? "|" : "") + ")" : "");
  final_string_stream << (!find_s(st1, st2, Data.RLS).empty() && find_s(st1, st2, Data.RLS) != final_string_stream.str() ? "|" + find_s(st1, st2, Data.RLS) : "");

  string final_string = final_string_stream.str();
  // counter++;
  Data.RLS[{st1, st2}] = final_string;
  // counter++;
  Data.W = Data.RLS[{d.get_initial_state(), d.get_initial_state()}];
  // cal_res(d, Data);
  // counter++;
  Data.Q = Data.RLS[{d.get_initial_state(), "VO_CHTO_IA_WLIAPALSA_PAMAGITI_MNE"}];
  cal_res(d, Data);
  // counter++;
}
void p_d(DFA &d, AutomationInfo &Data, string &cur_s)
{
  // cout<<"p_d"<<endl;
  for (const string st1 : Data.in_s)
  {
    // counter++;
    for (const string st2 : Data.out_st)
    {
      process_data(d, st1, st2, Data, cur_s);
      // counter++;
    }
  }
}
void in_out(map<vector<string>, string> &RLS, string &cur_s, AutomationInfo &a, DFA &d)
{
  // cout<<"in_out"<<endl;
  for (const auto &rule : RLS)
  {
    vector<string> key = rule.first;
    // counter++;
    string st1 = key[0];
    string st2 = key[1];
    // counter++;
    a.in_s.insert((st2 == cur_s && !my_c(a.del_s, st1)) ? st1 : "");
    // counter++;
    a.out_st.insert((st1 == cur_s && !my_c(a.del_s, st2)) ? st2 : "");
  }
  // counter++;
  p_d(d, a, cur_s);
}

void processStates(DFA &d, AutomationInfo &Data)
{
  // cout<<"processStates"<<endl;
  // counter++;
  for (string cur_s : d.get_states())
  {
    // counter++;
    if (cur_s == d.get_initial_state())
    {
      // counter++;
      // cout << "+" << endl;
      continue;
    }
    else
    {
      // counter++;
      in_out(Data.RLS, cur_s, Data, d);

      // for (auto it : Data.in_s)
      //   cout << it << " ";
      // cout << endl;
      // for (auto it : Data.out_st)
      //   cout << it << " ";
      // cout << endl;

      // counter++;
      Data.in_s.clear();
      // counter++;
      Data.out_st.clear();
      // counter++;
      Data.del_s.insert(cur_s);
      // counter++;
    }
  }
}

void generateRLS(DFA &d, AutomationInfo &Data)
{
  // counter++;
  for (const auto symbol : d.get_alphabet())
  {
    // counter++;
    for (const auto cur_s : d.get_states())
    {
      d.has_trans(cur_s, symbol) ? (!Data.RLS[{cur_s, d.get_trans(cur_s, symbol)}].empty() ? (Data.RLS[{cur_s, d.get_trans(cur_s, symbol)}] += "|" + string(1, symbol)) : (Data.RLS[{cur_s, d.get_trans(cur_s, symbol)}] = string(1, symbol))) : "";
      d.is_final(cur_s) ? (Data.RLS[{cur_s, "VO_CHTO_IA_WLIAPALSA_PAMAGITI_MNE"}] = "") : "";
    }
  }
}

string worker(DFA &d)
{

  // cout<<"worker"<<endl;
  AutomationInfo Data;
  generateRLS(d, Data);

  if (d.get_states().size() == 1)
  {
    Data.W = Data.RLS[{d.get_initial_state(), d.get_initial_state()}];
    cal_res(d, Data);

    Data.Q = Data.RLS[{d.get_initial_state(), "VO_CHTO_IA_WLIAPALSA_PAMAGITI_MNE"}];
    cal_res(d, Data);

    return Data.res;
  }

  // cout<<"worker"<<endl;
  processStates(d, Data);
  // counter++;
  cal_res(d, Data);
  // cout << Data.res << endl;
  return Data.res;
}

string dfa2re(DFA &d)
{
  // cout<<"dfa2re"<<endl;
  // counter++;
  // cout << d.to_string() << endl;
  return worker(d);
}