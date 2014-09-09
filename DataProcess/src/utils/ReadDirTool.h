/*************************************************************************
	> File Name: ReadDirTool.h
	> Author: hogachen
	> Mail: 1107402232@qq.com 
	> Created Time: 2014年09月06日 星期六 09时53分45秒
 ************************************************************************/
#include<iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include<fstream>
using namespace std;
class ReadDirTool{
public:
void readContent(std::string fullname, std::list<std::string> &articlelist){
        std::ifstream out(fullname.c_str());
        if(!out){
                std::cout<<"open file failed！"<<std::endl;
                return ;
        }
		std::string all_line;
        while(!out.eof()){
                std::string line;
                out>>line;
				all_line+=line;
        }

		articlelist.push_back(all_line);
        out.close();
}
void listDir(std::string path, std::list<std::string> &articlelist)
{
        DIR              *pDir ;
        struct dirent    *ent  ;
        int               i=0  ;
        char              childpath[512];
		int  count = 0;
        pDir=opendir(path.c_str());
        memset(childpath,0,sizeof(childpath));
        int flag=1;
        while((ent=readdir(pDir))!=NULL)
        {

                if(ent->d_type & DT_DIR)
                {

                        if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
                                continue;
			            sprintf(childpath,"%s%s",path.c_str(),ent->d_name);                       
						std::string cpath(childpath);
                        listDir(cpath,articlelist );

                }
                else
				{		count++;
						if(count==10000000)return;
                        char fullname[512];
                        sprintf(fullname,"%s%s",path.c_str(),ent->d_name);
                        std::string filename(fullname);
                        readContent(filename, articlelist);
                }
        }

}
};
