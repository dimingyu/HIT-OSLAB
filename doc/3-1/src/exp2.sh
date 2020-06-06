#!/bin/bash
NAME=$1
#判断传递的第一个参数是否为空
if [ "$NAME" == "" ]
then
#为空则打印并结束
	echo "usage:prog name"
	exit
fi
#以冒号分割/etc/passwd 中的字符串，再提取第一个域，并遍历
for user in $(cut -d : -f 1 /etc/passwd)
do
#包含了传递的第一个参数
    if [ "$user" == "$NAME" ]
    then
#则打印 已找到
	    echo founud $NAME in /etc/passwd
#并退出循环
	    break
    fi
done
