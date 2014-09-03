/*
 * TestModel.cpp
 *
 *  Created on: 2014年8月25日
 *      Author: Administrator
 */

#include"TestModel.h"
/*
 * 以下是测试数据
 */

void TestModel::ReadBayeModel(ClassificationInfo &classification, std::string filename){
  std::ifstream infile(filename.c_str());

  int flag=0;
  std::string temp1;
  std::string wordfre;
  std::string tfidf;
  double value;
  double tfidfvalue;
  if(!infile){
    std::cout<<"file read error"<<std::endl;
  }
  while(!infile.eof()){
    if(flag==0){
//      std::cout<<"file reading"<<std::endl;
      infile>>temp1;
      classification.article_num=atoi(temp1.c_str());
      infile>>temp1;
      classification.word_num=atoi(temp1.c_str());
      flag=1;
    }
    infile>>temp1;
//    std::cout<<"temp1: "<<temp1<<std::endl;
    infile>>wordfre;
    infile>>tfidf;
    value=atof(wordfre.c_str());
    tfidfvalue=atof(tfidf.c_str());
    WeiboWord weiboword(temp1,"",value);
    weiboword.SetTfidf( tfidfvalue);
    classification.GetWrodFre()->insert(make_pair(temp1,weiboword));
  }
//  std::cout<<"read model"<<std::endl;

  infile.close();
}
void TestModel::InitIsPolitics(){
      ReadBayeModel(this->other_classification,"other_classification.txt");
      ReadBayeModel(this->politic_classification, "politic_classification.txt");
      std::cout<<"classification.word_fre.size :"<<this->other_classification.word_fre.size()<<std::endl;
      std::cout<<"classification.word_fre.size :"<<this->politic_classification.word_fre.size()<<std::endl;
}
std::string TestModel::intTostring(int a){
  char tem[10];
  sprintf(tem,"%d",a);
  std::string res(tem);
  return res;
}
void TestModel::init_ICTCAL(void) {
  if (!ICTCLAS_Init()) {
    printf("Init fails\n");
    return;
  }
}
std::string TestModel::WidetoString2(std::wstring &wsm){
    std::string sToMatch;

    int iLen=wsm.size();//1024;
    char lpsz[iLen];
    wcstombs( lpsz, wsm.c_str(), iLen-2 ); // 转换。（没有结束符）
    lpsz[iLen-2] = '\0';
    sToMatch=lpsz;
    return sToMatch;
}
std::wstring TestModel::StringToWide2(std::string &sToMatch){
  std::setlocale(LC_ALL,"zh_CN.UTF-8");
  int iWLen=sToMatch.size();
  wchar_t lpwsz[iWLen*2];
  mbstowcs( lpwsz, sToMatch.c_str(), iWLen-2 );
  lpwsz[iWLen-2]='\0';
  std::wstring wsToMatch(lpwsz);
  return wsToMatch;
}
std::string TestModel::RegexReplace(std::string input){
  std::string output;
  std::wstring put;
  if(input.empty())
  {
     std::cout<<"input RegexReplace error"<<std::endl;
     return "";
  }
  //去掉表情
  input.append(" ");
  boost::wregex reg(L"[^%0-9a-zA-Z\u4e00-\u9fa5]*", boost::regex::perl);
  put=boost::regex_replace(StringToWide2(input),reg,std::wstring(L""));
  input=WidetoString2(put);
  return input;
}
void TestModel::Readstopwordset(){
  std::ifstream ifile("stopwords.txt");
  while(!ifile.eof()){
    std::string temp;
    ifile>>temp;
    boost::trim(temp);
    this->stopwords.insert(temp);
  }
}
std::string TestModel::ICTspilt(const char * sinput,int property) {
  std::string result;
  int nRstLen=0;
  unsigned int nPaLen;
  nPaLen=strlen(sinput); // 需要分词的长度
  if(nPaLen<2) {
    result="";
    return result;
  }
//  std::cout<<"npalen:  "<<nPaLen<<std::endl;
  char* sRst=(char *)malloc(nPaLen*2*sizeof(char)); //建议长度为字符串长度的倍。
  if(sRst==NULL)
  return NULL;
  nRstLen = ICTCLAS_ParagraphProcess(sinput,nPaLen,sRst,CODE_TYPE_UTF8,property);//字符串处理
//  std::cout<<"分词： "<<nRstlen<<std::endl;
  result=sRst;
  free(sRst);
  sRst=NULL;
  return result;
}
void TestModel::GetWordInOneArticle(std::string content, Article &one_article) {
  std::vector<std::string> splitedWords;
  std::string allword;
  std::cout<<content.size()<<std::endl;
  std::string result = ICTspilt(content.c_str(), 0);
  boost::split(splitedWords, result, boost::is_any_of(" ")); //分割
  std::cout<<splitedWords.size()<<std::endl;
  for (int spi = 0; spi < splitedWords.size(); ++spi) {
    if(spi==0)std::cout<<splitedWords[spi]<<std::endl;
    std::string word = splitedWords[spi];
    allword+=word;
    one_article.articlewordnum++;
    if (this->stopwords.count(word) && !word.empty()) {
      continue;
    }
    std::map<std::string, double>::iterator it;
    it = one_article.all_word.find(word);
    if (it != one_article.all_word.end()) {
      it->second++;
    } else {
      one_article.all_word.insert(make_pair(word, 1.0));
    }
  }
  std::cout<<allword<<std::endl;
}
void TestModel::CalArticleBayePro(int &belong_to_other_num,int &belong_to_politic_num, Article &one_article,std::list<std::string>&goodfilename){
    std::map<std::string,WeiboWord>::iterator other_it;
    std::map<std::string,WeiboWord>::iterator politic_it;
    std::string key;
    double other_classification_pro=1.0;
    double politic_classification_pro=1.0;
    int belong_counter=0;
    //文章的概率
    double all_article_num=(double)this->other_classification.article_num+this->politic_classification.article_num;
    double other_article_pro=(this->other_classification.article_num/all_article_num);
    double politic_article_pro= (this->politic_classification.article_num/all_article_num);
    //两种方案：1.直接计算词的后验概率  2.计算后验概率的同时将词的权值（词频等）也计算进去
    int articlewordnum=one_article.articlewordnum;
    std::cout<<"文章长度： "<<one_article.all_word.size()<<std::endl;
     std::map<std::string, double>::iterator it = one_article.all_word.begin();
     for (; it != one_article.all_word.end(); ++it) {
      key = it->first;
//      if(key.compare("腐败")==0||key.compare("贪污")==0||key.compare("党风")==0||
//          key.compare("廉政")==0||key.compare("廉洁")==0||
//          key.compare("暴力")==0||key.compare("恐怖")==0||key.compare("袭击")==0||key.compare("暴恐")==0)
//      {
//        belong_counter++;
//      }
      other_it = this->other_classification.word_fre.find(key);
      if (other_it != this->other_classification.word_fre.end()) {
         other_classification_pro += log(it->second/one_article.articlewordnum)+
             log(other_it->second.wordfre/this->other_classification.word_num);//*it->second.wordfre/100//由于词频太大会导致乘完之后后验概率失效;
      }
      //计算在政治类中的后验概率
      politic_it = this->politic_classification.word_fre.find(key);
      if (politic_it != this->politic_classification.word_fre.end()) {
        politic_classification_pro += log(it->second/one_article.articlewordnum)+
            log(politic_it->second.wordfre/this->politic_classification.word_num);//同上
      }
    }
    //判断属于哪一个类别,按照后验概率
     politic_classification_pro=-politic_classification_pro;
     other_classification_pro=-other_classification_pro;
     std::cout<<"poli and other概率值： "<<politic_classification_pro<<"  "<<other_classification_pro<<std::endl;
    if(politic_classification_pro > other_classification_pro){//||belong_counter>=2
      std::cout<<"politic_classification_pro*politic_article_pro  "<<politic_classification_pro*politic_article_pro<<std::endl;
      std::cout<<"other_classification_pro*other_article_pro "<<other_classification_pro*other_article_pro<<std::endl;
      one_article.ispolitic=1;

      std::cout<<"isPolitics: "<<one_article.ispolitic<<std::endl;
      goodfilename.push_back(one_article.filename);
      std::cout<<"belong_counter: "<<belong_counter<<std::endl;
      if(one_article.orignclass==1)belong_to_politic_num++;
    }else{
      if(one_article.orignclass==0)
        belong_to_other_num++;
      std::cout<<"isPolitics: "<<one_article.ispolitic<<std::endl;
    }
}
void TestModel::SaveUsefulText(std::list<std::string>&filenamelist){
  std::string inputfloder="../BayeData/C000010/";
  std::string outputfloder="../BayeData/goodnews/";

  std::list<std::string >::iterator f_it= filenamelist.begin();
  for(;f_it!=filenamelist.end();++f_it){
//    std::cout<<"run here2  "<<*f_it<<std::endl;
    std::string filename=inputfloder+*f_it;
    std::ifstream infile(filename.c_str());
    std::string temp;
    std::string onearticlewords;
    while (!infile.eof()) {
        infile >> temp;
        onearticlewords += temp;
    }
    std::string outfilename = outputfloder+*f_it;
    std::ofstream  outfile(outfilename.c_str());
    outfile<<onearticlewords+"\n";
//    std::cout<<"run here3 "<<std::endl;
    outfile.close();
    infile.close();
  }
}
void TestModel::ProcessTestFile(std::list < FilenameAndContent> &politic_article_list){
  init_ICTCAL();
  InitIsPolitics();
   Readstopwordset();
   std::list<std::string>goodfilename;
   std::string result;
   std::string temp;

   int wordnum=0;
   int articlenum=0;
   std::list<FilenameAndContent>::iterator it= politic_article_list.begin();
   int belong_to_other_num=0;//本来为政治类，被分配到证之类的文章总数
   int belong_to_politic_num=0;//被分配为政治类的文章总数
   for (;it != politic_article_list.end(); ++it) {
     articlenum++;
     temp=it->content;
     Article one_article;
     one_article.orignclass=it->ispolitic;
     one_article.filename=it->filename;
     one_article.setpolitic();
     GetWordInOneArticle(temp, one_article);
     CalArticleBayePro(belong_to_other_num,belong_to_politic_num,one_article,goodfilename);
   }
   std::cout<<"run here1"<<std::endl;
//   SaveUsefulText(goodfilename);
   std::cout<<"政治中被分为政治类的："<<belong_to_politic_num<<"  文章总数："<<articlenum<<std::endl;
   std::cout<<"正确率为： "<<(double)(belong_to_other_num+belong_to_politic_num)/articlenum<<std::endl;
   std::cout<<"覆盖率为： "<<(double)belong_to_politic_num/4600<<std::endl;
}
void TestModel::ReadTest(){
  std::list<FilenameAndContent >politic_article_list;
  std::string politic_folder_name[]= {"politicTrain","C000010"};
  int length2 = 2;
  int len[]={4600,1999};
    //读取政治类文章
  std::string filename_prefix="../BayeData";
    for (int i = 0; i < length2; ++i) {
      for (int fi = 10; fi <=len[i] ; fi++) {
        std::string res = intTostring(fi);
        std::string everyfilename=filename_prefix+"/" + politic_folder_name[i] + "/" + res + ".txt";

        std::ifstream infile(everyfilename.c_str());
        std::string temp;
        std::string onearticlewords;
        while (!infile.eof()) {
          infile >> temp;
          onearticlewords += temp;
        }
        onearticlewords=RegexReplace(onearticlewords);
        if(onearticlewords=="")continue;
        FilenameAndContent fc;
        if(i==0)fc.ispolitic=1;else fc.ispolitic=0;
        fc.filename=res+".txt";
        fc.content=onearticlewords;
        politic_article_list.push_back(fc);
        infile.close();
      }
    }
    ProcessTestFile(politic_article_list);
}
