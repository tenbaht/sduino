#!/usr/bin/gawk -f

#   text	   data	    bss	    dec	    hex	filename
#      0	   4673	      0	   4673	   1241	build-stm8sblue/libmake.ihx


/  [ls]_/ {
    v=strtonum("0x"$1);
    type=substr($2,1,1);
    name=substr($2,3);
    arr[name][type]=v;
#    print v,type,name,$0
}

END {
    bss = arr["DATA"]["l"]+arr["INITIALIZED"]["l"];
    rom = arr["CODE"]["l"]+arr["INITIALIZER"]["l"];

    for (n in arr)
        print arr[n]["l"] "\t" n;
#        print n, arr[n]["s"],arr[n]["l"];
    print "memory usage statics"
    print "--------------------"
    print "RAM:\t\t" arr["DATA"]["l"]+arr["INITIALIZED"]["l"];
    print "Flash:\t\t" arr["CODE"]["l"]+arr["INITIALIZER"]["l"];
    for (n in arr) printf("%s\t",n);
    for (n in arr) print arr[n]["l"] "\t";

    print "   text\t   data\t    bss\t    dec\t    hex\tfilename";
    printf ("%7i\t%7i\t%7i\t%7i\t%7x\t%s\n",0,rom,bss,rom,rom,FILENAME);

}
