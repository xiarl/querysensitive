#include "sensitivewordquery_loopfind.h"

namespace StringOperate {
  extern std::string ReplaceString(
    std::string strSour, 
    const std::string & checkstr,
    char chrep);
}

bool SensitiveQuery44::HasSensitiveWord(
  const std::string &word,
  std::string * word_replace){
    if (word.empty()){
      return false;
    }
    std::string word_check = word;
    for (std::size_t index = 0; index < notwords_.size(); ++index){
      if (word_check.npos != word_check.find(notwords_[index])) {
        if (word_replace){
          word_check = StringOperate::ReplaceString(
            word_check, 
            notwords_[index],
            replace_char_);
          *word_replace = word_check;
        } 
        else{
          return true;
        }
      }
    }
    return word != word_check;
}
