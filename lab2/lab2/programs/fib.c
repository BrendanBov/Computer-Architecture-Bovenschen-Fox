#include <stdio.h>
int main() {

  int i, n;

  // initialize first and second terms
  int t1 = 0, t2 = 1;

  // initialize the next term (3rd term)
  int nextTerm = t1 + t2;

  // get no. of terms from user
  printf("Enter the number of terms: ");
  scanf("%d", &n);

  // print the first two terms t1 and t2
  printf("Fibonacci Series:\n", t1, t2);
  printf("%d = 0x%x\n", t1, t1);
  printf("%d = 0x%x\n", t2, t2);

  // print 3rd to nth terms
  for (i = 3; i <= n; ++i) {
    t1 = t2;
    t2 = nextTerm;
    nextTerm = t1 + t2;
    printf("%d = 0x%x\n", nextTerm, nextTerm);    
  }

  return 0;
}
