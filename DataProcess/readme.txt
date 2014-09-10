抓取新浪微博热门话题系统
在/tmp/JavaProjct-jinfa/HopTopic
运行run.sh自动会抓取，并把整点话题放到hourtopic文件夹下，启动要在x窗口下，否则selenium会报错

测评输出文件在/tmp/JavaProjct-jinfa/output文件夹


相关微博搜索系统
调用Ccoverage.SearchCountSort()函数就可以，首先要运行java程序run.sh先
SearchCountSort()—>触发java程序->抓取主题的搜索量->SearchCountSort()输出