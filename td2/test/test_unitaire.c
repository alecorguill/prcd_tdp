#include "util.h"
#include "particule.h"

#include <assert.h>

void test_parseur(){
  printf("TEST_PARSEUR...");
  int nb = 2;
  particule ps[nb];
  parse_particules("test/particules.txt", ps);
  /*
  for(int i=0; i<nb; ++i){
    printf("Particule n°%d ", i);
    printf("masse : %d ", ps[i].m);
    printf("position n°%lf %lf ", ps[i].p.x, ps[i].p.y);
    printf("vitesse n°%lf %lf\n", ps[i].v.x, ps[i].v.y);
  }
  */
  assert(ps[0].m == 5 && ps[0].p.x == 9.2 && ps[0].p.y == 2.5
	 && ps[0].v.x == 14.1 && ps[0].v.y == 5.2);
  assert(ps[1].m == 7 && ps[1].p.x == 1.2 && ps[1].p.y == 2.6
	 && ps[1].v.x == 14.1 && ps[1].v.y == 5.2);
  printf("ok\n");
  return;
}


int main(){
  test_parseur();
}