#include "src/process.h"
#include <iostream>
int main()
{

	std::list<std::string> IDs;
	Process process= Process();
	std::string ID;
	std::list<std::string> vec;
	//std::list<std::string> vec;


	//MakeStopSet(stopwordsSet);
	//process.Get_Stringlist(ID,vec);
	process.Get_MIDs("2013-04-12 08:38:16",27000,IDs);


	for(std::list<std::string>::iterator it=IDs.begin() ; it!=IDs.end();it++){
				std::cout<<*it<<std::endl;
	}

	double total;
	time_t startT,endT;
	startT=time(NULL);
	for(std::list<std::string>::iterator it_ids=IDs.begin();it_ids!=IDs.end();++it_ids){
			ID=*it_ids;

			process.Get_Stringlist(ID,vec);

			for(std::list<std::string>::iterator it=vec.begin() ; it!=vec.end();++it){
				std::cout<<*it<<"|";
				}
			std::cout<<std::endl;


		//	char sql_query[60];
		//	sprintf(sql_query,"select text from weibo where mid=%s ",ID.c_str());
		//	process.set_mysql_query(sql_query);

		//	process.Query_DB();

		//	std::list<std::list<std::string> >	resutlt=process.Get_result();
		//	std::cout<<resutlt.front().front()<<std::endl;

		//	std::cout<<"-----------------------------------------------------------------------"<<std::endl;

			vec.clear();
			vec.resize(1);
		}

	endT=time(NULL);
	total=difftime(endT,startT);
	std::cout<<"程序运行时间"<<total<<"   num"<<IDs.size()<<std::endl;













//	/**
//	 * example for get the fenci分词
//	 */
//	std::list<std::string> IDs;
//	std::list<std::string> vec;
//	std::string ID;
//	Process process;
//
//	//this function is to get the IDs between 2013-04-12 08:38:16  and  2013-04-12 08:38:16+1600second
//	process.Get_MIDs("2013-04-12 08:38:16",1600,IDs);
//
//	for(std::list<std::string>::iterator it_ids=IDs.begin();it_ids!=IDs.end();it_ids++){
//			ID=*it_ids;
//			//this function is to get the fenci where mid=ID
//			process.Get_Stringlist(ID,vec);
//			//here is to dispaly the fenci result
//			for(std::list<std::string>::iterator it=vec.begin() ; it!=vec.end();it++){
//					std::cout<<*it<<" | "<<std::endl;
//				}
//			std::cout<<std::endl;
//			vec.clear();
//		}
//
//	/**
//	 * example for mysql_query
//	 */
//	process.set_mysql_query("select * from weibo limit 2");
//	process.Query_DB();    //to query the DB
//	std::list<std::list<std::string> > result=process.Get_result();
//	process.display(result);  //to display the mysql_query result
//

	return 1;
}	

