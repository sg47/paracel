#include <unordered_map>
#include <fstream>
#include <string>
#include <vector>
#include "utils.hpp"

using namespace std;

template <class T>
unordered_map<string, double> 
bias_mix(const T & fn1, 
         const T & fn2,
         double frac = 0.7) {
  
  unordered_map<string, double> ubias1, ubias2, ubias3;
  
  auto handler_lambda = [] (unordered_map<string, double> & dct,
                           const string & line) {
    auto tmp = paracel::str_split(line, "\t");
    dct[tmp[0]] = stod(tmp[1]);
  };
  
  auto fn_lst1 = paracel::expand(fn1);
  auto fn_lst2 = paracel::expand(fn2);
  string line_buf;

  for(auto & ufn1 : fn_lst1) {
    ifstream f1(ufn1);
    while(getline(f1, line_buf)) {
      handler_lambda(ubias1, line_buf);
    }
    f1.close();
  }
  for(auto & ufn2 : fn_lst2) {
    ifstream f2(ufn2);
    while(getline(f2, line_buf)) {
      handler_lambda(ubias2, line_buf);
    }
    f2.close();
  }
  std::cout << "init done" << std::endl;

  // mix
  assert(ubias1.size() == ubias2.size());
  for(auto & kv : ubias1) {
    ubias3[kv.first] = frac * kv.second + (1 - frac) * ubias2[kv.first];
  }
  std::cout << "done" << std::endl;

  return ubias3;
}

void dump(const unordered_map<string, double> & ubias,
          const string & output) {
  // dump
  ofstream fout;
  //fout.open(output, ofstream::app);
  fout.open(output);
  std::cout << "dump begin" << std::endl;
  for(auto & kv : ubias) {
    fout << kv.first << '\t' << kv.second << '\n';
  }
  std::cout << "dump done" << std::endl;
  fout.close();
}

int main(int argc, char *argv[])
{
  string ubn1 = "/mfs/user/wuhong/paracel/data/netflix_result8/ubias_*";
  string ubn2 = "/mfs/user/wuhong/paracel/data/cbr_result_parallel/ubias_*";
  string output = "/mfs/user/wuhong/paracel/test/serial/mix_ubias_netflix";
  auto mix_ubias = bias_mix(ubn1, ubn2);
  dump(mix_ubias, output);
  return 0;
}
