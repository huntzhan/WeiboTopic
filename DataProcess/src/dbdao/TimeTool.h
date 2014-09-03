/*
 * TimeTool.h
 *
 *  Created on: 2014年8月29日
 *      Author: Administrator
 */

#ifndef TIMETOOL_H_
#define TIMETOOL_H_
class TimeTool{
  /*@description
   *  产生各种形式的human时间字符串
   *@param
   *  format:"%Y%m%d_%H",human形式的格式化字符串
   *@return
   *  human形式的字符串
   */
  std::string GenDayAndTimestamp(std::string format){
    time_t t;
    t = time(0);
    char now[64];
    struct tm *ttime;

    ttime = localtime(&t);
    strftime(now, 64, format.c_str(), ttime);
    string strtime(now);
    return strtime;
  }
  /*
   * @description
   *  将"%Y%m%d_%H"形式的字符串转换为time_t格式的时间戳
   * @param
   *  strtime:"%Y%m%d_%H"形式的字符串
   * @return
   *  time_t形式的时间戳
   */
  time_t str2time(std::string strtime){
    tm tm_;
    time_t t_;
    char buf[128]= {0};

    strcpy(buf, strtime.c_str());
    strptime(buf, "%Y-%m-%d %H:%M:%S", &tm_); //将字符串转换为tm时间
    tm_.tm_isdst = -1;
    t_  = mktime(&tm_); //将tm时间转换为秒时间
    return t_;
  }
  std::string formatTimeAtHour(){
    char mytime[64];
    sprintf(mytime,"%d",str2time(GenDayAndTimestamp("%Y-%m-%d %H:00:00")));
    std::cout<<mytime<<std::endl;
    std::string strtime(mytime);
    return strtime;
  }
  std::string formatTimeAtDay(){
    char mytime[64];
    sprintf(mytime,"%d",str2time(GenDayAndTimestamp("%Y-%m-%d 00:00:00")));
    std::cout<<mytime<<std::endl;
    std::string strtime(mytime);
    return strtime;
  }
/*
 * @description
 *  将输入的时间戳字符串去掉小时，只保留日期，同时转换为时间戳字符串
 * @param
 *  时间戳字符串
 * @return
 *  少了小时的时间戳
 */
std::string RemoveHourTimestamp(std::string dayandhour){
  if(dayandhour.compare("")==0)return 0;
  char buf[128]={0};
  strcpy(buf,dayandhour);
  struct  tm tm_;
  //先将时间戳转为时间,%小s表示的是你buf里面是“秒”的字符串时间
  strptime(buf,"%s",&tm_);
  //再将时间转为字符串
  time_t t_ = mktime(&tm_);
  struct tm *tt=localtime(&t_);

  strftime(buf,128,"%Y-%m-%d 00:00:00",tt);
  struct tm tm2;
  strptime(buf, "%Y-%m-%d %H:%M:%S", &tm2);
  time_t t_2  = mktime(&tm2);
  char mytime[128];
  sprintf(mytime,"%d",t_2);
  std::string ti(mytime);
  std::cout<<ti<<std::endl;
  return ti;
}

};




#endif /* TIMETOOL_H_ */
