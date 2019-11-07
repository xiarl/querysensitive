# querysensitive
自己写的一个敏感词查询 类、接口，主要用的是多叉树的实现

以字节为树的节点。

使用的时候，比如：

//定义
boost::shared_ptr<IQuerySensitive> query_sensitive_;

//赋值初始化
query_sensitive_ = boost::make_shared < SensitiveQueryDFA >();


//使用
bool HasSensitiveWord(
  const std::string & word,
  std::string * word_replace){
    return query_sensitive_->HasSensitiveWord(word, word_replace);
}
