#!/bin/sh

cat="cat "
s_21cat="./s21_cat "
SUCCESS=0
FAIL=0
COUNTER=0

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE="\033[36m"
ENDCOLOR='\033[0m'

testing(){
    ./s21_cat $var bytes.txt bytes.txt> test_s21_cat.log 
    cat $var bytes.txt bytes.txt> test_cat.log
    if cmp -s test_s21_cat.log test_cat.log
    then
    (( SUCCESS++ ))
    (( COUNTER++ ))

    echo "${GREEN}Test  №$COUNTER SUCCESS${ENDCOLOR} ${BLUE}$s_21cat $var ${ENDCOLOR}";
    else
    (( FAIL++ ))
    (( COUNTER++ ))

    echo "${RED}Test  №$COUNTER FAIL${ENDCOLOR} ${BLUE}$s_21cat $var ${ENDCOLOR}"
    fi
    rm test_s21_cat.log test_cat.log
}


for var1 in e b n s t v
do  
    var="-$var1"
    testing $var
done

for var1 in e b n s t v
do  
    for var2 in e b n s t v
    do  
        if [ $var1 != $var2 ]
        then
            var="-$var1 -$var2"
            testing $var
        fi
    done
done

for var1 in e b n s t v
do  
    for var2 in e b n s t v
    do  
        for var3 in e b n s t v
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then  
                var="-$var1 -$var2 -$var3"
                testing $var
            fi
        done
    done
done

for var1 in e b n s t v
do  
    for var2 in e b n s t v
    do  
        for var3 in e b n s t v
        do
            for var4 in e b n s t v
            do
                if [ $var1 != $var2 ] && [ $var1 != $var3 ] && [ $var1 != $var4 ] && [ $var2 != $var3 ] && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then  
                    var="-$var1 -$var2 -$var3 -$var4"
                    testing $var
                fi
            done
        done
    done
done

echo "${GREEN}SUCCESS $SUCCESS ${ENDCOLOR} | ${RED} FAILED $FAIL${ENDCOLOR}";