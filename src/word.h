#ifndef _WORD_H_
#define _WORD_H_

#include "ein_util.h"
#include "ein_aibo.h"

using namespace std;
#include <sstream>
#include <map>
#include <boost/algorithm/string.hpp>


class MachineState;

using namespace boost::algorithm;

class Word:  public enable_shared_from_this<Word> {
  
public:

  virtual void execute(MachineState * ms);
  virtual string name() = 0;

  virtual string description() {
    return "";
  }

  virtual vector<string> names() {
    vector<string> result;
    result.push_back(name());
    return result;
  }

  virtual bool is_value() {
    return false;
  }
     

  virtual int character_code() {
    return -1;
  }

  virtual bool equals(std::shared_ptr<Word> word) {
    if (word.get() == this) {
      return true;
    } else {
      return false;
    }
  }

  virtual bool to_bool() {
    return true;
  }
  virtual int to_int() {
    return 1;
  }
  virtual double to_double() {
    return 1;
  }

  /**
   * Returns it as a string in the repl (e.g., quotes, back ticks for strings and symbols)
   */
  virtual string repr() {
    return name();
  }

  /**
   * Converts to a string (e.g., no quotes)
   */
  virtual string to_string() {
    return name();
  }

};

class DoubleWord: public Word
{
private:
  double n;

public:

  double value() {
    return n;
  }

  virtual bool is_value() {
    return true;
  }

  static std::shared_ptr<DoubleWord> parse(string token);

  virtual bool is_static() {
    return false;
  }
  static bool isDouble(string token);
  
  DoubleWord(double _n) {
    n = _n;
  }
  string name() {
    stringstream ss;
    ss << scientific;
    ss << n;
    return ss.str();
  }

  bool equals(shared_ptr<Word> word) {
    return word->to_double()== this->to_double();
  }
  
  virtual bool to_bool() {
    if (n == 0) {
      return false;
    } else {
      return true;
    }
  }
  virtual double to_double() {
    return n;
  }
  virtual int to_int() {
    return (int) n;
  }
};


class IntegerWord: public Word
{
private:
  int n;

public:

  int value() {
    return n;
  }

  virtual bool is_value() {
    return true;
  }

  static std::shared_ptr<IntegerWord> parse(string token);
  virtual bool is_static() {
    return false;
  }
  static bool isInteger(string token) {
    try {
      parse(token);
      return true;
    } catch (...) {
      return false;
    }
  }
  
  IntegerWord(int _n) {
    n = _n;
  }
  string name() {
    stringstream ss;
    ss << n;
    return ss.str();
  }

  bool equals(shared_ptr<Word> word) {
    return word->to_int() == this->to_int();
  }
  
  virtual bool to_bool() {
    if (n == 0) {
      return false;
    } else {
      return true;
    }
  }
  virtual int to_int() {
    return n;
  }
  virtual double to_double() {
    return n;
  }
};


class StringWord: public Word
{
private:
  string s;

public:

  string value() {
    return s;
  }

  virtual bool is_value() {
    return true;
  }

  virtual bool is_static() {
    return false;
  }

  static std::shared_ptr<StringWord> parse(string token) {
    if (token[0] == '\"' && token[token.size() - 1] == '\"') {
      return std::make_shared<StringWord>(token.substr(1, token.size() - 2));      
    } else {
      return NULL;
    }

  }
  static bool isString(string token) {
    try {
      if (parse(token) != NULL) {
        return true;
      } else {
        return false;
      }
    } catch (...) {
      return false;
    }
  }
  
  StringWord(string _s) {
    s = _s;
  }

  string name() {
    stringstream ss;
    ss << "\"" << s << "\"";
    return ss.str();
  }

  string to_string() {
    return s;
  }

  bool equals(shared_ptr<Word> word);

  
};


class CommentWord: public Word
{
private:
  string s;

public:

  string value() {
    return s;
  }

  virtual bool is_value() {
    return true;
  }

  virtual bool is_static() {
    return false;
  }

  virtual void execute(MachineState * ms) {
    
  }

  static std::shared_ptr<CommentWord> parse(string token);

  static bool isComment(string token);

  CommentWord(string _s) {
    s = _s;
  }

  string name() {
    stringstream ss;
    ss << "/*" << s << "*/";
    return ss.str();
  }

  string to_string() {
    return s;
  }
  
};




class SymbolWord: public Word
{
private:
  string s;

public:

  virtual bool is_value() {
    return true;
  }

  virtual bool is_static() {
    return false;
  }

  static std::shared_ptr<SymbolWord> parse(string token) {
    if (token.size() != 0) {
      return std::make_shared<SymbolWord>(token);
    } else {
      return NULL;
    }
  }

  static bool isSymbol(string token);
  string repr();
  
  SymbolWord(string _s) {
    s = _s;
  }
  shared_ptr<Word> getReferencedWord(MachineState * ms);

  virtual void execute(MachineState * ms);


  string name() {
    stringstream ss;
    ss << "'" << s;
    return ss.str();
  }

  string to_string() {
    return s;
  }
  
};


class EePoseWord: public Word
{
private:
  eePose pose;

public:
  eePose value() {
    return pose;
  }

  virtual bool is_value() {
    return true;
  }

  static std::shared_ptr<EePoseWord> parse(string token) {
    
    eePose pose;
    return std::make_shared<EePoseWord>(pose);
  }
  virtual bool is_static() {
    return false;
  }
  static bool isInteger(string token) {
    if (token.substr(0,5) == "eePose") {
      return true;
    } else {
      return false;
    }
  }
  
  EePoseWord(eePose _pose) {
    pose = _pose;
  }

  virtual string repr();

  string name() {
    stringstream ss;
    ss << pose;
    return ss.str();
  }

  bool equals(shared_ptr<Word> word) {
    shared_ptr<EePoseWord> w1 = dynamic_pointer_cast<EePoseWord>(word);
    if (w1 == NULL) {
      return false;
    } else {
      return w1->value().equals(this->value());
    }
  }
  
  virtual bool to_bool() {
    return true;
  }
  virtual int to_int() {
    return 1;
  }
};

class ArmPoseWord: public Word
{
private:
  armPose pose;

public:
  armPose value() {
    return pose;
  }

  virtual bool is_value() {
    return true;
  }

  static std::shared_ptr<ArmPoseWord> parse(string token) {
    
    armPose pose;
    return std::make_shared<ArmPoseWord>(pose);
  }
  virtual bool is_static() {
    return false;
  }
  static bool isInteger(string token) {
    if (token.substr(0,6) == "armPose") {
      return true;
    } else {
      return false;
    }
  }
  
  ArmPoseWord(armPose _pose) {
    pose = _pose;
  }

  virtual string repr();

  string name() {
    stringstream ss;
    ss << pose;
    return ss.str();
  }

  bool equals(shared_ptr<Word> word) {
    shared_ptr<ArmPoseWord> w1 = dynamic_pointer_cast<ArmPoseWord>(word);
    if (w1 == NULL) {
      return false;
    } else {
      return w1->value().equals(this->value());
    }
  }
  
  virtual bool to_bool() {
    return true;
  }
  virtual int to_int() {
    return 1;
  }
};




class CompoundWord : public Word {
 private:
  vector<std::shared_ptr<Word> > stack;
  string description_text;
 public:
  CompoundWord() {
  }
  CompoundWord(string description) {
    description_text = description;
  }
  void pushWord(shared_ptr<Word> word) {
    stack.push_back(word);
  }
  void pushWord(MachineState * ms, string word);
  void setDescription(string d) {
    description_text = d;
  }
  virtual string description() {
    return description_text;
  }

  int size();
  shared_ptr<Word> popWord();
  shared_ptr<Word> getWord(int i);

  virtual void execute(MachineState * ms);
  string repr();
  string name();
  string to_string();
  static shared_ptr<CompoundWord> copy(shared_ptr<CompoundWord> cw);
};

class AiboPoseWord: public Word
{
private:
  EinAiboJoints pose;

public:
  EinAiboJoints value() {
    return pose;
  }

  virtual bool is_value() {
    return true;
  }

  static std::shared_ptr<AiboPoseWord> parse(string token) {
    EinAiboJoints pose;
    return std::make_shared<AiboPoseWord>(pose);
  }
  virtual bool is_static() {
    return false;
  }
  static bool isInteger(string token) {
    if (token.substr(0,5) == "EinAiboJoints") {
      return true;
    } else {
      return false;
    }
  }
  
  AiboPoseWord(EinAiboJoints _pose) {
    pose = _pose;
  }

  virtual string repr();

  string name() {
    stringstream ss;
    //ss << pose;
    return ss.str();
    // XXX need to overload << 
  }

  bool equals(shared_ptr<Word> word) {
    shared_ptr<AiboPoseWord> w1 = dynamic_pointer_cast<AiboPoseWord>(word);
    if (w1 == NULL) {
      return false;
    } else {
      //return w1->value().equals(this->value());
      // XXX not done
      return false;
    }
  }
  
  virtual bool to_bool() {
    return true;
  }
  virtual int to_int() {
    return 1;
  }
};






std::map<int, std::shared_ptr<Word> > create_character_code_to_word(std::vector<std::shared_ptr<Word> > words);
std::map<string, std::shared_ptr<Word> > create_name_to_word(std::vector<std::shared_ptr<Word> > words);

std::shared_ptr<Word> parseToken(MachineState * ms, string token);
std::shared_ptr<Word> nameToWord(string name);

extern std::vector<std::shared_ptr<Word> > words;
extern std::map<int, std::shared_ptr<Word> > character_code_to_word;
extern std::map<string, std::shared_ptr<Word> > name_to_word;

void initializeWords();

void renderCoreView(MachineState * ms);

/*
WORD()
virtual void execute(MachineState * ms)
{
    ms->pushWord("");
}
END_WORD
REGISTER_WORD()
*/

#define CONSOLE(ms, args) \
    { \
  std::stringstream __publish__console__message__stream__ss__; \
  __publish__console__message__stream__ss__ << args; \
  publishConsoleMessage(ms, __publish__console__message__stream__ss__.str()); \
    }

#define CONSOLE_ERROR(ms, args) \
    { \
  std::stringstream __publish__console__message__stream__ss__; \
  __publish__console__message__stream__ss__ << "\033[1;31m" << args << "\033[0m"; \
  publishConsoleMessage(ms, __publish__console__message__stream__ss__.str()); \
    }


#endif /* _WORD_H_ */
