#include "sensitivewordquery.h"
#include <fstream>
#include <iostream>
#include "log.h"

namespace{
  const std::string notwordFileName = "not_word.txt";

  //ȥ��һ���ַ���������ܵ����д�trim�ַ���Ĭ��ȥ��\r��\n
  class TrimStringEndTool {
  public:
    TrimStringEndTool()
      :trim_chars_("\x0d\x0a") {
    }
    std::string TrimStr(const std::string & checkstr) {
      int index = checkstr.length();
      for(; index > 0; --index){
        std::string strCh = checkstr.substr(index-1, 1);
        if (std::string::npos == trim_chars_.find(strCh)){
          break;
        }
      }
      std::string strGet = checkstr.substr(0, index);
      return strGet;
    }
  private:
    std::string trim_chars_;
  public:
    //���Ժ���
    static void TestFun() {
      TrimStringEndTool obj;
      obj.trim_chars_.append("\t");

      std::string strTest;
      std::string strNew = obj.TrimStr(strTest);//���
      if (!strNew.empty()) {
      //  std::cerr<<"wrong, line"<<__LINE__<<std::endl;
        return;
      }
      strTest = "x";
      strNew = obj.TrimStr(strTest);//1����

      strTest = "\xd";
      strNew = obj.TrimStr(strTest);//1����

      strTest = "123456789abcdefghijklmnopq";
      strNew = obj.TrimStr(strTest);//����
      if (strNew != strTest){
     //   std::cerr<<"wrong, line"<<__LINE__<<std::endl;
        return;
      }
      strTest[5] = '\r';
      strNew = obj.TrimStr(strTest);//����
      if (strNew != strTest){
      //  std::cerr<<"wrong, line"<<__LINE__<<std::endl;
        return;
      }

      strTest.append("\r\t");
      strNew = obj.TrimStr(strTest);//���
      strTest = "123456789abcdefghijklmnopq";
      strTest.append("\n");
      strNew = obj.TrimStr(strTest);//��һ

      obj.trim_chars_.clear();
      strNew = obj.TrimStr(strTest);//�ղ�
    }
  };
}

namespace StringOperate{
  std::string ReplaceString(
    std::string strSour, 
    const std::string & checkstr, char chrep) {
      if (checkstr.empty() || strSour.empty()) {
        return "";//find �մ�ʼ�շ��� 0
      }
      std::string strRep(checkstr.length(), chrep);
      std::string::size_type posfind = strSour.find(checkstr);
      while(posfind != strSour.npos){
        strSour.replace(posfind, checkstr.length(), strRep);
        posfind = strSour.find(checkstr, posfind + checkstr.length());
      }
      return strSour;
  }
}

namespace FileOperate{
  void GetLineStrFromFile(
    std::vector<std::string> & notwords,
    const std::string & filename){
      notwords.clear();
      std::ifstream iff(filename);
      if (!iff){
        QLOG(QLOG_ERROR,"open file fail: %s", filename.c_str());
        return;
      }
      TrimStringEndTool trimObj;
      std::string strLine;
      while(getline(iff, strLine)) {
        strLine = trimObj.TrimStr(strLine);
        if (strLine.empty()){
          continue;
        }
        notwords.push_back(strLine);
      }
      iff.close();
      iff.clear();
  }
}

SensitiveQuery::SensitiveQuery()
  :replace_char_('*'){
    LoadNotWord();
}

bool SensitiveQuery::LoadNotWord(){
  QLOG(QLOG_INFO, "begin GetNotWordStrFromFile");
  notwords_.clear();
  FileOperate::GetLineStrFromFile(notwords_, notwordFileName);
  QLOG(QLOG_INFO, "GetNotWordStrFromFile size:%u", notwords_.size());
  return true;
}

void SensitiveQuery::SetReplaceCharacter(char chSet){
  replace_char_ = chSet;
}


////////////////////////test code//////////////////////////////////////////////////

void TestFun_ReplaceString(){
  using namespace StringOperate;
  char chrep = '*';
  std::string strSour, checkStr, strNewGet;
  strNewGet = ReplaceString(strSour, checkStr, chrep);//����

  checkStr = "xia";
  strNewGet = ReplaceString(strSour, checkStr, chrep);//һ��

  strSour = "rl";
  strNewGet = ReplaceString(strSour, checkStr, chrep);//��

  strSour = "xiarl";
  strNewGet = ReplaceString(strSour, checkStr, chrep);// ��

  strSour = "abcdefg";
  strNewGet = ReplaceString(strSour, checkStr, chrep);// ��

  strSour = "xiarlxiarl";
  strNewGet = ReplaceString(strSour, checkStr, chrep);//�����ڵĶ��Ҫ�滻

  strSour = "xiaxiarl";
  strNewGet = ReplaceString(strSour, checkStr, chrep);//���ڵĶ��Ҫ�滻
}

void TestFun_sensitivewordquery_cpp(){
  //TrimStringEndTool::TestFun();
  //TestFun_ReplaceString();
}
