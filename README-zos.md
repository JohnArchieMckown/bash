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
