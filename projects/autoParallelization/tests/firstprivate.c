/*
 * dependence graph:
 */
void foo()
{
  int i,x,y=1;
  int a[100];
  int b[100];
// x should be recognized as a firstprivate variable during parallelization
// Does it introduce any data dependencies ?
// could be (as regular private variables )
  for (i=0;i<=100-1;i+=1) 
  { 
    y=x+1;
    //b[i]=x+1;
    // x=...
    // ... =x
  }  
  x=1;
}  
/*
 
 */
