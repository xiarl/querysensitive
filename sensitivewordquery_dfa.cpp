#include "sensitivewordquery_dfa.h"

#include <set>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "log.h"

class byteCharTree {
public:
  typedef 
    std::map<unsigned char, boost::shared_ptr<byteCharTree> >
    ChildrenMapType;
  byteCharTree();
  void SetData(unsigned char data);
  virtual bool IsGuard()const{
    return false;
  }
  void AddString2Children(const std::string & strAdd);
  bool MatchStr(
    const std::string & strcmp,
    std::size_t index, 
    std::string & MatchGet) const;
private:
  unsigned char currdata_;
  ChildrenMapType children_;
};

class byteCharGuardNode : public byteCharTree {
public:
  virtual bool IsGuard()const {
    return true;
  }
};

namespace{
  void MakeByteTree(
    byteCharTree * treeMakeRoot, 
    const std::vector<std::string> & notwords_){
      for (std::size_t index = 0; index < notwords_.size(); ++index){
        treeMakeRoot->AddString2Children(notwords_[index]);
      }
  }
  bool IsOneByteString(const std::string & str_check){
    return str_check.length() == 1;
  }
}

byteCharTree::byteCharTree()
  :currdata_('\0'){
}

void byteCharTree::SetData(unsigned char data){
  currdata_ = data;
}

void byteCharTree::AddString2Children(
  const std::string & strAdd) {
    if (strAdd.empty()) {
      return;
    }
    char chAdd = strAdd.front();
    if(children_.end() == children_.find(chAdd)){
      boost::shared_ptr<byteCharTree> childAdd;
      if (IsOneByteString(strAdd)) {
        childAdd = boost::make_shared<byteCharGuardNode>();
      } else {
        childAdd = boost::make_shared<byteCharTree>();
      }    
      childAdd->SetData(chAdd);
      children_.insert(std::make_pair(chAdd, childAdd));
    }
    children_[chAdd]->AddString2Children(strAdd.substr(1));  
}

bool byteCharTree::MatchStr(
  const std::string & strcmp, 
  std::size_t index,
  std::string & MatchGet) const {
    if (index >= strcmp.length()) {
      return false;
    }
    unsigned char currCh = strcmp.at(index);
    ChildrenMapType::const_iterator iterFind =
      children_.find(currCh);
    if (children_.end() == iterFind){
      return false;
    } else{
      MatchGet.resize(MatchGet.length() +1,
        iterFind->second->currdata_);
      if (iterFind->second->IsGuard()){
        return true;
      }
      if (iterFind->second->children_.empty()){
        return true;
      }
      return iterFind->second->MatchStr(strcmp, ++index, MatchGet);
    }
}


SensitiveQueryDFA::SensitiveQueryDFA(){
  treeCmpData_ = new byteCharTree;
  QLOG(QLOG_INFO, "begin MakeByteTree");
  MakeByteTree(treeCmpData_, notwords_);
  QLOG(QLOG_INFO, "end MakeByteTree");
}

SensitiveQueryDFA::~SensitiveQueryDFA(){
  delete treeCmpData_;
}

namespace StringOperate{
  extern std::string ReplaceString(
    std::string strSour, 
    const std::string & checkstr, 
    char chrep);
}
bool SensitiveQueryDFA::HasSensitiveWord(
  const std::string &word, 
  std::string * word_replace){
    std::string word_check = word;
    for (std::string::size_type index = 0; index < word_check.size();){
      std::string MatchGet;
      MatchGet.reserve(word_check.length());
      if(treeCmpData_->MatchStr(word_check, index, MatchGet)){
        word_check = 
          StringOperate::ReplaceString(word_check, MatchGet, replace_char_);
        index += MatchGet.length();//跳过（一个）被替换的字符串长度
      }else {
        ++index;
      }
    }
    if (word_replace){
      *word_replace = word_check;
    }
    return word != word_check;
}
