#!/usr/bin/gawk -f

/  [ls]_/ {
    v=strtonum("0x"$1);
    type=substr($2,1,1);
    name=substr($2,3);
    arr[name][type]=v;
#    print v,type,name,$0
}

END {
    for (n in arr)
        print arr[n]["l"] "\t" n;
#        print n, arr[n]["s"],arr[n]["l"];
    print "memory usage statics"
    print "--------------------"
    print "RAM:\t\t" arr["DATA"]["l"]+arr["INITIALIZED"]["l"];
    print "Flash:\t\t" arr["CODE"]["l"]+arr["INITIALIZER"]["l"];
    for (n in arr) printf("%s\t",n);
    for (n in arr) print arr[n]["l"] "\t";

}
