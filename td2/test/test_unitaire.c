#include "util.h"
#include "particule.h"

void test_parseur(){
  printf("TEST_PARSEUR");
  int nb = 4;
  particule ps[nb];
  parse_particules("particules.txt", ps);
  for(int i=0; i<nb; ++i){
    printf("Particule n°%d ", i);
    printf("masse : %d ", ps[i].m);
    printf("position n°%d %d", ps[i].p.x, ps[i].p.y);
    printf("vitesse n°%d %d\n", ps[i].v.x, ps[i].v.y);
  }
  return;
}


int main(){
  test_parseur();
}