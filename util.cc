// util.h -- May 10, 2009
//    by geohot
// part of "The Embedded Disassembler"
//  released under GPLv3, see http://gplv3.fsf.org/

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>

#include "util.h"
#include "data.h"

using namespace std;
using namespace eda;

uint32_t eda::stoi(const string& num) {
  istringstream in;
  in.str(num);
  uint32_t ret;
  if(num.substr(0,2)=="0x")
    in >> std::hex >> ret;
  else if(num.find_first_of("abcdefABCDEF") != string::npos)
    in >> std::hex >> ret;
  else
    in >> std::dec >> ret;
  return ret;
}

bool eda::file_to_string(const std::string& filename, std::string* out) {
  FILE* f = fopen(filename.c_str(), "rb");
  if (f==0) return false;
  fseek(f, 0, SEEK_END);
  int size = ftell(f);
  fseek(f, 0, SEEK_SET);
  char* dat = (char*)malloc(size);
  int read = fread(dat, 1, size, f);
  out->assign(dat, size);
  delete [] dat;
  return read == size;
}

int eda::find_matching(const std::string& s, int start, char open, char close) {
  //cout << s << endl;
  int count = 0;
  for (int i = start; i < s.length(); i++) {
    if (s[i] == close) {
      if (count == 1) {
        //cout << "returning " << i << endl;
        return i;
      }
      else count--;
    } else if (s[i] == open) {
      count++;
    }
  }
  LOG << "error in brackets" << endl;
  return -1;
}

void eda::SerializeToXML(ostringstream& out, const vector<int>& v, string name, string item) {
  out << "<" << name << ">";
  for (int i = 0; i < v.size(); i++) {
    out << "<" << item << ">";

    out << "</" << item << ">";
  }
  out << "</" << name << ">";
}


// Returns things like 9 and 0x4F
string eda::immed(uint32_t data) {
  ostringstream o;
  if(data < 10) o << data;
  else o << hex << "0x" << data;
  return o.str();
}
// Signed
string eda::immed_signed(int32_t data) {
  ostringstream o;
  if(data<0) {
    o << "-";
    data = data * -1;
  }
  o << immed(data);
  return o.str();
}
