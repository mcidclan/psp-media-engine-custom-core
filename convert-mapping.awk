#!/usr/bin/awk -f
BEGIN { 
 first = 1
 t2Col = ""
 ogCol = ""
 count = 0
}

/\{.*,.*\}/ {
 gsub(/[{} \t]/, "")
 gsub(/,$/, "")
 n = split($0, pair, ",")
 if (n >= 2) {
   if (!first) {
     t2Col = t2Col ", "
     ogCol = ogCol ", "
   }
   t2Col = t2Col pair[1]
   ogCol = ogCol pair[2]
   first = 0
   count++
 }
}

END {
 print "const u32 meCoreMapping0[" count "] __attribute__((aligned(64))) = {" t2Col "};"
 print "const u32 meCoreMapping1[" count "] __attribute__((aligned(64))) = {" ogCol "};"
}
