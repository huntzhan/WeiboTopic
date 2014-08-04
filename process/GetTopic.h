/*
 * GetTopic.h
 *
 *  Created on: 2014��8��3��
 *      Author: hogachen
 */

#ifndef GETTOPIC_H_
#define GETTOPIC_H_

/*
 * GetTopic.cpp
 *
 *  Created on: 2014��7��31��
 *      Author: hogachen
 */


/*
 * ���ԸĽ��ĵط������Բ���ÿ�ζ������߸�Сʱ��Ƶ�ʣ�ֻҪ��ȥ��ǰһ��Сʱ�Ϳ�����
 */

#include "Weibo.h"
#include "TopicWord.h"
#include<list>
#include<set>
#include<map>
#include<iostream>
using namespace std;

typedef pair<string,double> PAIR;



class GetTopic{
	int TOPIC_WORD_NUM = 1000;									//��Ҫ�õ�ǰ��������ʣ����Ĭ����ǰ1000��
	int K_WINDOW = 7;											//ʱ�䴰�ڵĴ�С
	list<string> m_current_messageList;							//����ʱ���ȡ����΢��ID�б�
	list<string> m_k_messageList;
	map<string,double> m_topic_word;							//��ȡ����û���ظ����б��������ʺʹʵ�Ȩֵ
	map<string,double> k_hour_topic_word;						//ǰK��Сʱ�ĵ��ʵĳ���Ƶ�ʣ��ʸ�m_topicword����һ��,����֮��Ҫ�ͷŵ�
public:

	void GetCurrentHourWeiboList(string time);          		//���ݽ������Ľӿ����ɵ�ǰһСʱ��΢���б�
	void GetKHourWeiboList(string time_from,string time_to);	//����getMessageList��ȡǰ�߸�Сʱ��΢��ID�б�

	void GetEveryWordInCurrentHour();



    Weibo GetEveryWeiboFromDatabase(string weiboID); 			//������������������ݿ�߶�ȡǰ�߸�Сʱ��΢���������ȡ��ȥ�ڴ档
    															//��Ҫ������ǰ�߸�Сʱ��΢��ID���ٸ���IDȥ��ѯ���ݿ�

    void TopicWordSort();								        //���ݻ�õ�������б���֮ǰ��K��ʱ�䴰���ڵĴʵ�Ȩ�رȽ�֮����Ȩ�����������������ָ��

    void AddKeyToMap(map<string,double>&filterMap,string key);

    void CalTopicWordInKhours();
    int SortCmp (const PAIR & key1,const PAIR & key2);


    void GenTopicWord();
};

#endif /* GETTOPIC_H_ */
