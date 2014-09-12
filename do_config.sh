./configure --enable-alias --enable-arith-for-command --enable-array-variables --enable-brace-expansion --enable-casemod-attributes --enable-casemod-expansions \
--enable-cond-command --enable-cond-regexp --enable-coprocesses --enable-directory-stack --enable-disabled-builtins \
--enable-dparen-arithmetic --enable-extended-glob --enable-help-builtin --enable-history --enable-job-control --enable-process-substitution \
--enable-progcomp --enable-readline --enable-select --with-curses \
CC=xlc CFLAGS='-qtarg=zosv1r12 -qskipsrc=hide -qarch=5 -qtune=7 -qlanglvl=extended:longlong -D_XOPEN_SOURCE=600 -D_LARGE_FILES -D_ALL_SOURCE -D_ISOC99_SOURCE -O2'
       CPPFLAGS='-qtarg=zosv1r12 -qskipsrc=hide -qarch=5 -qtune=7 -qlanglvl=extended:longlong -D_XOPEN_SOURCE=600 -D_LARGE_FILES -D_ALL_SOURCE -D_ISOC99_SOURCE -O2'
#C=xlc CFLAGS='-qtarg=zosv1r12 -qskipsrc=hide -qarch=5 -qtune=7 -qlanglvl=extended:longlong -qagg -qsource -qshowmacros -qshow -qexpmac -D_ALL_SOURCE -O2'
