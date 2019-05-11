#!/bin/sh
# manage.py是进程名称，修改为自己的即可
while true
do
    v_num=`ps -ef|grep "main"|grep -v more|grep -v tail|grep -v grep |wc -l`
    if [ $v_num -lt 2 ]
    then 
        date
        echo "进程不存在"
        ps -ef|grep "main"|grep -v more|grep -v tail|grep -v grep |awk '{ print $2 }'|xargs kill -9
        # 应用启动
        cd /home/robomaster/5.11-Vision2.6/
        ./main  

        sleep 1
        v_num=`ps -ef|grep "main"|grep -v more|grep -v tail|grep -v grep |wc -l`
        if [ $v_num -eq 2 ]
        then 
            date
            echo "已恢复正常"
        fi

    else 
    date
    echo "程序正常运行，不需要重启!"
    fi
done
