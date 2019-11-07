#ifndef __QUERYINTERFACE_H__
#define __QUERYINTERFACE_H__

#include <string>

class IQuerySensitive {
public:
  virtual ~IQuerySensitive(){};
  //’“µΩ¡À∑µªÿtrue
  virtual bool HasSensitiveWord
    (const std::string & word, 
    std::string * word_replace) = 0;
  virtual void SetReplaceCharacter(char chSet) = 0;
};

#endif // __QUERYINTERFACE_H__
