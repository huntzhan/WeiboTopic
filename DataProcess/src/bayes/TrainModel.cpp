/*
 * TrainModel.cpp
 *
 *  Created on: 2014年8月22日
 *      Author: hogachen
 */

#include"TrainModel.h"
#include<fstream>
#include"ReadDirTool.h"
bool SortCmp(const PAIR3 &key1, const PAIR3 &key2) {
  if (key1.second.tfidf > key2.second.tfidf)
    return true;
  return false;
}
void TrainModel::Readstopwordset(){
  std::ifstream ifile("stopwords.txt");
  while(!ifile.eof()){
    std::string temp;
    ifile>>temp;
    boost::trim(temp);
    this->stopwords.insert(temp);
  }
}
std::string WidetoString2(std::wstring &wsm){
    std::string sToMatch;

    int iLen=wsm.size();//1024;
    char lpsz[iLen];
    wcstombs( lpsz, wsm.c_str(), iLen-2 ); // 转换。（没有结束符）
    lpsz[iLen-2] = '\0';
    sToMatch=lpsz;
    return sToMatch;
}
std::wstring StringToWide2(std::string &sToMatch){
  std::setlocale(LC_ALL,"zh_CN.UTF-8");
  int iWLen=sToMatch.size();
  wchar_t lpwsz[iWLen*2];
  mbstowcs( lpwsz, sToMatch.c_str(), iWLen-2 );
  lpwsz[iWLen-2]='\0';
  std::wstring wsToMatch(lpwsz);
  return wsToMatch;
}
std::string RegexReplace(std::string input){
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


//提取TFIDF最高的10000个词
void PoliticsWordSort(ClassificationInfo &classinfo) {
  int train_word_num=20000;
  std::map<std::string, WeiboWord>::iterator mapIt;
  std::vector<PAIR3> sort_vec;
  for (mapIt = classinfo.GetWrodFre()->begin(); mapIt != classinfo.GetWrodFre()->end(); ++mapIt) {
    sort_vec.push_back(make_pair(mapIt->first, mapIt->second));
  }

  std::sort(sort_vec.begin(), sort_vec.end(), SortCmp);
  std::vector<PAIR3>::iterator  sort_vec_it = sort_vec.begin();

  int counter=0;
  for(;sort_vec_it!=sort_vec.end();++sort_vec_it){
    std::cout<<sort_vec_it->first<<"    "<<sort_vec_it->second.tfidf<<"   "<<
        sort_vec_it->second.wordfre<<"   "<<sort_vec_it->second.docfre<<std::endl;
    if(counter++>10000)break;
  }
  //提取指定的特征词数量（20000）个
  int topicWordNum=train_word_num;
  classinfo.GetWrodFre()->clear();
  if(sort_vec.size()<train_word_num){
    topicWordNum=sort_vec.size();
  }
  std::vector<PAIR3>::iterator v_it = sort_vec.begin();
  int count = 0;
  while (v_it != sort_vec.end() && count < topicWordNum) {
    std::string key = v_it->first;
    WeiboWord weiboword = v_it->second;
    classinfo.GetWrodFre()->insert(make_pair(key, weiboword));
    ++v_it;
    ++count;
  }
}


std::string ICTspilt(const char * sinput,int property) {
  std::string result;
  int nRstLen=0;
  unsigned int nPaLen;
  nPaLen=strlen(sinput); // 需要分词的长度
  if(nPaLen<2) {
    result="";
    return result;
  }
  char* sRst=(char *)malloc(nPaLen*2*sizeof(char)); //建议长度为字符串长度的倍。
  if(sRst==NULL)
  return NULL;
  nRstLen = ICTCLAS_ParagraphProcess(sinput,nPaLen,sRst,CODE_TYPE_UTF8,property);//字符串处理
  result=sRst;
  free(sRst);
  sRst=NULL;
  return result;
}


void TrainModel::TrainClassModel(std::string otherdir,std::string politicdir){
  Readstopwordset();
  std::list < std::string > articlelist;
  std::list < std::string > politic_article_list;

  ReadArticle(articlelist,politic_article_list, otherdir, politicdir);
  SplitWord(articlelist,politic_article_list);
}


//分词初始化函数
void init_ICTCAL(void) {
  if (!ICTCLAS_Init()) {
    printf("Init fails\n");
    return;
  }
}
//double转为string
std::string doubletostring(double d){
  char tem[20];

  sprintf(tem,"%lf",d);
  std::string res(tem);
  return res;
}
//int转为string
std::string intTostring(int a){
  char tem[10];
  sprintf(tem,"%d",a);
  std::string res(tem);
  return res;
}

//将结果保留到硬盘
void SaveModelToTxt(std::string filename, ClassificationInfo &classinfo){

  std::ofstream  outfile(filename.c_str());
  std::string arnum=intTostring(classinfo.article_num)+"\n";
  outfile<<arnum;
  std::string wordnum=intTostring(classinfo.word_num)+"\n";
  outfile<<wordnum;
  std::map<std::string,WeiboWord>::iterator map_it=classinfo.word_fre.begin();
  for(; map_it != classinfo.word_fre.end(); ++map_it){
    std::string word = map_it->first;
    double value = map_it->second.wordfre;
    std::string res = doubletostring(value);

    double tfidf = map_it->second.tfidf;
    std::string restfidf = doubletostring(tfidf);
    std::string line=word+" "+res+" "+restfidf+"\n";
    outfile<<line;

  }
  outfile.close();
}

//设置每个词是否在一条微博中重复出现标志位为初始值
void SetWeiboWordIsOccuredFalse(std::map<std::string, WeiboWord> &mymap) {
  std::map<std::string, WeiboWord>::iterator it;
  it = mymap.begin();
  for (;it != mymap.end();++it) {
    it->second.isoccured=false;
  }
}
//计算每个词的TFIDF值
void CalWordTFIDF(std::map<std::string, WeiboWord> &mymap) {
  std::map<std::string, WeiboWord>::iterator it;
  it = mymap.begin();
  for (; it != mymap.end(); ++it) {
    it->second.tfidf = it->second.wordfre*it->second.docfre;
  }
}
void TrainModel::AddWordToMap(std::list<std::string> &articlelist, ClassificationInfo &classinfo) {
  std::string result;
  std::string temp;
  std::vector < std::string > splitedWords;
  int wordnum=0;
  int articlenum=0;
  std::list<std::string>::iterator it= articlelist.begin();
  for (;it != articlelist.end(); ++it) {
    articlenum++;
    temp=*it;
    result = ICTspilt(temp.c_str(), 0);
    boost::split(splitedWords, result, boost::is_any_of(" ")); //分割
    for (int spi = 0; spi < splitedWords.size(); ++spi) {

      std::string word = splitedWords[spi];
      if(this->stopwords.count(word)&&!word.empty()){
        continue;
      }
      wordnum++;
      std::map<std::string, WeiboWord>::iterator it;
      it = classinfo.word_fre.find(word);
      if (it != classinfo.word_fre.end()) {
        it->second.wordfre++;

        //计算文档频率，设置标志位为true（已经在本篇文章中出现过了）
        if(it->second.isoccured==false){
          it->second.isoccured=true;
          it->second.docfre+=1;
        }

      } else {
        //这里的初始化时，文档频率的标志为就置为true了
        WeiboWord weiboword(word,"",1.0);
        classinfo.word_fre.insert(make_pair(word, weiboword));
      }
    }
    SetWeiboWordIsOccuredFalse( classinfo.word_fre);
  }
  classinfo.article_num=articlenum;
  classinfo.word_num=wordnum;
  //计算词的tdidf和按照tfidf排序
  CalWordTFIDF(classinfo.word_fre);
  PoliticsWordSort(classinfo);

}

void TrainModel::SplitWord(std::list < std::string > &articlelist,
    std::list < std::string > &politic_article_list){

  init_ICTCAL();
  AddWordToMap(articlelist,this->other_classification);
  AddWordToMap(politic_article_list, this->politic_classification);

  SaveModelToTxt("normal.txt",this->other_classification);
  SaveModelToTxt("trash.txt", this->politic_classification);

}



void TrainModel::ReadArticle(std::list<std::string > &articlelist,
		std::list<std::string>&politic_article_list, std::string otherdir ,std::string politicdir){
	ReadDirTool readdirtool;
	readdirtool.listDir(otherdir,articlelist);

	readdirtool.listDir(politicdir,politic_article_list);


}
/*
void TrainModel::ReadArticle(std::list < std::string > &articlelist,
    std::list < std::string > &politic_article_list) {


  std::string foldername[]= {"C000010","C000013","C000014","C000016","C000022","C000023"};
  std::string politic_folder_name[]= {"TrashFilter"};
//  std::string foldername[]= {"filterother"};
//  std::string politic_folder_name[]= {"filterpolitic"};
  int length1 = 6;int flag=1;
  std::string filename_prefix="../BayeData";
  //读取非政治类文章
  for (int i = 0; i < length1; ++i) {
    std::cout<<i<<std::endl;
    for (int fi = 10; fi < 1000; fi++) {
//      std::cout<<"have read "<<fi<<" articles"<<std::endl;
      std::string res = intTostring(fi);
      std::string everyfilename=filename_prefix+"/" + foldername[i] + "/" + res + ".txt";

      std::ifstream infile(everyfilename.c_str());
      std::string temp;
      std::string onearticlewords;
      if(!infile){
         std::cout<<"read other file error"<<std::endl;
      }
      while (!infile.eof()) {
        infile >> temp;
        onearticlewords += temp;

      }
//      std::cout<<everyfilename<<std::endl;
      onearticlewords=RegexReplace(onearticlewords);
      if(onearticlewords=="")continue;
      articlelist.push_back(onearticlewords);
      infile.close();
    }
  }
  int length2 = 1;
  int len[]={83};
  //读取政治类文章
  for (int i = 0; i < length2; ++i) {
    std::cout<<len[i]<<std::endl;
    for (int fi = 1; fi <= len[i]; fi++) {
//      std::cout<<"have read "<<fi<<" articles"<<std::endl;
      std::string res = intTostring(fi);
      std::string everyfilename=filename_prefix+"/" + politic_folder_name[i] + "/" + res + ".txt";
      std::ifstream infile(everyfilename.c_str());
      std::string temp;
      std::string onearticlewords;
      if(!infile){
        std::cout<<"read politic file error"<<std::endl;
      }
      while (!infile.eof()) {
        infile >> temp;
        onearticlewords += temp;
      }
      onearticlewords=RegexReplace(onearticlewords);
      if(onearticlewords=="")continue;
      politic_article_list.push_back(onearticlewords);
      infile.close();
      std::cout<<fi<<std::endl;
    }
  }
}

*/


