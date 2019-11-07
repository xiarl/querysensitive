#include "sensitivewordquery.h"
#include <vector>
#include <string>

class SensitiveQuery44 : public SensitiveQuery{
public:
  virtual bool HasSensitiveWord(
    const std::string & word, 
    std::string * word_replace);
};
