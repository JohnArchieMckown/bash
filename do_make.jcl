//BASHCMPL JOB CLASS=K,MSGCLASS=H,NOTIFY=&SYSUID                                
//BPXBATCH EXEC PGM=BPXBATCH,REGION=0M                                          
//STDOUT DD SYSOUT=*                                                            
//STDIN DD PATH='/dev/null',PATHOPTS=(ORDONLY)                                  
//STDERR DD SYSOUT=*                                                            
//STDPARM DD *                                                                  
SH cd /u/joarmc/projects/bash;make                                              
/*                                                                              
//                                                                              
