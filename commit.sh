#!/bin/bash
modify=`git status | grep "modified:\|新增:" | awk '{print $2}'`

if [ -z modify ]
then
    echo '新增的文件是:'$2
    git add $2
    git commit -m $2
    git pull
    git push
else
    echo '修改的文件是:'$modify
    git add $modify
    git commit -m $1
    git pull
    git push
fi

