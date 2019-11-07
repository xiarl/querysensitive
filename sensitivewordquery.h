#ifndef __SENSITIVEWORDQUERY_H__
#define __SENSITIVEWORDQUERY_H__

#include "queryinterface.h"
#include <vector>
#include <string>

class SensitiveQuery : public IQuerySensitive {
public:
  SensitiveQuery();
public:
//virtual bool HasSensitiveWord(
//  const std::string & word, 
//  std::string * word_replace) = 0;
  virtual void SetReplaceCharacter(char chSet);
private:
  bool LoadNotWord();
protected:
  char replace_char_;
  std::vector<std::string> notwords_;
private:
  SensitiveQuery(const SensitiveQuery &);
  SensitiveQuery & operator=(const SensitiveQuery &);
};
#endif // __SENSITIVEWORDQUERY_H__
