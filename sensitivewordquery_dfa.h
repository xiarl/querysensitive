#include "sensitivewordquery.h"
#include <vector>
#include <string>

class byteCharTree;
class SensitiveQueryDFA : public SensitiveQuery{
public:
  SensitiveQueryDFA();
  virtual ~SensitiveQueryDFA();
public:
  virtual bool HasSensitiveWord(
    const std::string & word,
    std::string * word_replace);
private:
  byteCharTree * treeCmpData_;
};
