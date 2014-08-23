/*
 * TrainModel.cpp
 *
 *  Created on: 2014年8月22日
 *      Author: hogachen
 */

#include"TrainModel.h"
#include<fstream>
std::string ICTspilt(const char * sinput,int property) {
  std::string result;
  int nRstLen=0;
  unsigned int nPaLen;
  nPaLen=strlen(sinput); // 需要分词的长度
  if(nPaLen<2) {
    result="";
    return result;
  }
  char* sRst=(char *)malloc(10240); //建议长度为字符串长度的倍。
  if(sRst==NULL)
  return NULL;
  nRstLen = ICTCLAS_ParagraphProcess(sinput,nPaLen,sRst,CODE_TYPE_UTF8,property);//字符串处理
  result=sRst;
  free(sRst);
  sRst=NULL;
  return result;
}
void TrainModel::TrainClassModel(){
  std::list < std::string > articlelist;
  std::list < std::string > politic_article_list;

  ReadArticle(articlelist,politic_article_list);
  SplitWord(articlelist,politic_article_list);
}

void init_ICTCAL(void) {
  if (!ICTCLAS_Init()) {
    printf("Init fails\n");
    return;
  }
}
std::string doubletostring(double d){
  char tem[20];

  sprintf(tem,"%lf",d);
  std::string res(tem);
  return res;
}
void SaveModelToTxt(std::string filename, ClassificationInfo &classinfo){

  std::ofstream  outfile(filename.c_str());
  std::string arnum=classinfo.article_num+"\n";
  outfile<<arnum;
  std::string wordnum=classinfo.word_num+"\n";
  outfile<<wordnum;
  std::map<std::string,double>::iterator map_it=classinfo.word_fre.begin();
  for(; map_it != classinfo.word_fre.end(); ++map_it){
    std::string word = map_it->first;
    double value = map_it->second;
    std::string res = doubletostring(value);
    std::string line=word+" "+res+"\n";
    outfile<<line;
  }
  outfile.close();
}
void AddWordToMap(std::list<std::string> &articlelist, ClassificationInfo &classinfo) {
  std::string result;
  std::string temp;
  std::vector < std::string > splitedWords;
  int wordnum=0;
  int articlenum=0;
  std::list<std::string>::iterator it= articlelist.begin();
  for (;it != articlelist.end(); ++it) {
    articlenum++;
    temp=*it;
    result = ICTspilt(temp.c_str(), 1);
    boost::split(splitedWords, result, boost::is_any_of(" ")); //分割
    for (int spi = 0; spi < splitedWords.size(); ++spi) {
      wordnum++;
      std::string word = splitedWords[spi];
      std::map<std::string, double>::iterator it;
      it = classinfo.word_fre.find(word);
      if (it != classinfo.word_fre.end()) {
        it->second++;
      } else {
        classinfo.word_fre.insert(make_pair(word, 1.0));
      }
    }
  }
  classinfo.article_num=articlenum;
  classinfo.word_num=wordnum;
}

void TrainModel::SplitWord(std::list < std::string > &articlelist,
    std::list < std::string > &politic_article_list){

  init_ICTCAL();
  AddWordToMap(articlelist,this->other_classification);
  AddWordToMap(politic_article_list, this->politic_classification);

  SaveModelToTxt("other_classification.txt",this->other_classification);
  SaveModelToTxt("politic_classification.txt", this->politic_classification);

}

std::string intTostring(int a){
  char tem[10];
  sprintf(tem,"%d",a);
  std::string res(tem);
  return res;
}

void TrainModel::ReadArticle(std::list < std::string > &articlelist,
    std::list < std::string > &politic_article_list) {


  std::string foldername[]= {"C000008","C000010","C000013","C000014","C000016","C000020","C000022","C000023"};
  std::string politic_folder_name[]= {"C000024"};
  int length1 = 8;
  std::string filename_prefix="../BayeData/BayeData";
  //读取非政治类文章
  for (int i = 0; i < length1; ++i) {
    for (int fi = 10; fi < 2000; fi++) {
      std::string res = intTostring(fi);
      std::string everyfilename=filename_prefix+"/" + foldername[i] + "/" + res + ".txt";
      std::ifstream infile(everyfilename.c_str());
      std::string temp;
      std::string onearticlewords;
      while (!infile.eof()) {
        infile >> temp;
        onearticlewords += temp;
      }
      articlelist.push_back(onearticlewords);
      infile.close();
    }
  }
  int length2 = 1;
  //读取政治类文章
  for (int i = 0; i < length2; ++i) {
    for (int fi = 10; fi < 2000; fi++) {
      std::string res = intTostring(fi);
      std::string everyfilename=filename_prefix+"/" + foldername[i] + "/" + res + ".txt";
      std::ifstream infile(everyfilename.c_str());
      std::string temp;
      std::string onearticlewords;
      while (!infile.eof()) {
        infile >> temp;
        onearticlewords += temp;
      }
      politic_article_list.push_back(onearticlewords);
      infile.close();
    }
  }
}
