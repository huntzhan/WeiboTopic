/*
 * cluster.cpp
 *
 *  Created on: 2014年7月31日
 *      Author: hogachen
 */


#include"Cluster.h"
#include"TopicWord.h"
#include<iostream>
#include<string>
using namespace std;
typedef map<string,double> MAP
#define THROD 10.0
#define MAXVALUE 100000.0
class ClusterProcess{
public:

	double Cal_Words_Cluster_Distance(Cluster cluster,TopicWord t_word);
	void Singlepass(map<string,double> m_topicWord);
};
void Cluster::Singlepass(map<string,double> &m_topicWord){

	vector<Cluster> v_clusterList;

	MAP::iterator topic_w_it = m_topicWord.begin();
	TopicWord t_word(topic_w_it->first,topic_w_it->second);

	Cluster cluster(t_word);
	topic_w_it++;
	for(;topic_w_it != m_topicWord.end();++topic_w_it){

		TopicWord t_words(topic_w_it->first,topic_w_it->second);


		vector<Cluster>::iterator vec_clu_it=v_clusterList.begin();
		double minDistance=MAXVALUE;
		vector<Cluster>::iterator belong_clus_it=vec_clu_it;//这里应该是赋值还是指针？？？？
		for(;vec_clu_it != v_clusterList.end();++vec_clu_it){
			double words_distance=cal_Words_Cluster_Distance(vec_clu_it,t_words);
			if(minDistance > words_distance){
				minDistance = words_distance;
				belong_clus_it=vec_clu_it;
			}
		}
		if(minDistance > THROD){
			Cluster newCluster(t_words);
			v_clusterList.push_back(newCluster);
		}else{
			belong_clus_it->addTopicWord(t_words);
		}
	}
}
double ClusterProcess::Cal_Words_Cluster_Distance(Cluster *cluster,TopicWord t_word){
	vector<TopicWord>::iterator clu_it=cluster->getsTopic().begin();
	for(;clu_it!=)
}
