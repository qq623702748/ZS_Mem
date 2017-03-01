#!/bin/bash
modify=`git status | grep "modified:\|新增:" | awk '{print $2}'`

if [ -z modify ]
then
    git pull
    echo "nothing change!"
else
    echo '修改的文件是:'$modify
    git add $modify
    git commit -m $1
    git pull
    git push
fi

