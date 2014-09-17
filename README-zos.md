BASH 4.2 for z/OS
=================
port by John A. McKown
----------------------
mailto:john.archie.mckown@gmail.com

This is my attempt to port the GNU BASH shell, release 4.2 to z/OS. The
source was obtained by doing a

git clone git://git.savannah.gnu.org/bash.git

on a Linux/Intel system. Curiously, the BASH developers did not use a
git branch to separate the releases.  In order to do a fork from the 4.2
position, I did:

git branch bash-4.2-zos bash-4.2
git checkout bash-4.2-zos

At the time of the above, this forked the new branch from
commit 495aee441b75276e38c75694ccb455bb6463fdb9

All changes were made in the bash-4.2-zos branch. The simple way to see
what the changes are is to:  git diff bash-4.2 bash-4.2-zos

The file dist.pax.Z was created after a successful configure, make, and
make install on z/OS 2.1. It contains only the files needed to run BASH.
It does not include any source.

Required ./configure parameters
-------------------------------
The following options were used to address one or more problems that I
ran into doing the BASH port. Someone who is smarter than I might be
able to figure out how to get around them.
* --without-bash-malloc was required because without it bash never
  worked. It got out-of-memory errors during initialization.
* --disable-mem-scramble Not really required due to the above, but I
  don't think that z/OS has the same exploitation problems as other
  architectures. I could well be wrong.
* --prefix= is set to a non-GNU subdirectory in my bash source
  subdirectory. This simply made it easier for me to do a "make install"
  and be able to use pax to bundle the results into a single pax archive
  file.

Manual changes after ./configure
--------------------------------
* None at present

Source code availability
------------------------
BASH is licensed under the GPL. This requires that, if distributed, all changes
to the source must be available. In order to not force a source download
for people who only want the executable, the source is not included in this
PDS. The source is available at https://github.com/JohnArchieMckown/bash
and can most easily be downloaded by doing the command:

git clone git@github.com:JohnArchieMckown/bash.git

However, it can also be downloaded from your browser in "zip" format
from the web site. The source on github is in ASCII, not EBCDIC! And it
uses UNIX LF line endings, not Windows CRLF line endings.  It is your
responsibility to do the ASCII to EBCDIC translation as well as changing
the line endings if you use Windows. I am not a Windows power user, so I don't
really know how to do all of this. Using Linux, the simpliest thing to do is:

1. Go to the web site above
2. Make sure that you are on the branch "bash-4.2-zos" and
   not the default of JohnArchieMcKown (an error on my part)
3. Download the zip file into an empty subdirectory (cd ~/emptydir)
4. unzip bash-bash-4.2-zos.zip
5. This creates the directory bash-bash-4.2-zos under ~/emptydir (example)
6. tar cf bash-4.2-zos.tar bash-bash-4.2-zos
7. use ftp to do a BINary upload to a z/OS UNIX subdirectory
8. Use pax to unwind the tar file and translate to IBM-1047 via:
   pax -ofrom=ISO8859-1,to=IBM-1047 -rf bash-4.2-zos.tar

As an alternative to the above, if you have Java installed on z/OS, you
should be able to do something like:
1. create an empty directory as in: mkdir ~/emptydir
2. ftp the zip file into this directory, using BINary mode
3. Use the jar command to "unzip" the zip file.
   jar xf bash-bash-4.2-zos.zip
   This creates the bash-bash-4.2-zos subdirectory.
4. create another subdirectory in ~/emptydir, perhaps like:
   mkdir bash-4.2-zos
5. Use pax to copy and translate as in:
   pax -ofrom=ISO8859-1,to=IBM-1047 -rw bash-bash-4.2-zos bash-4.2-zos
6. cd bash-4.2-zos/bash-bash-4.2-zos
7. mv * ..
8. cd ..
9. rm bash-bash-4.2-zos
   I don't know why pax insists on the weird move, but it does. So I simply
   move everything up, then remove the extra subdirectory
10. At this time, in ~/emptydir (example), there is the bash-bash-4.2-zos and
    the bash-4.2-zos subdirectories. The former has the ASCII. The latter has
    the EBCDIC. You can remove the former with the command:
    rm -rf bash-bash-4.2-zos
