#include "util.h"
#include "particule.h"

#include <assert.h>
#include <math.h>

void test_parseur(){
  printf("TEST_PARSEUR...");
  int nb = 2;
  particule ps[nb];
  parse_particules("test/test.txt", ps);
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

void test_somme(){
  printf("TEST_SOMME...");
  vecteur v1 = {1,2}, v2 = {1.4,1.6};
  vecteur res;
  somme(&v1,&v2,&res);
  assert(res.x == 2.4 && res.y == 3.6);
  printf("ok\n");
  return;
}

void test_norme(){
  printf("TEST_NORME...");
  vecteur v = {3,4};
  assert(norme(&v) == 5.0);
  printf("ok\n");
  return;
}

void test_diff(){
  printf("TEST_DIFF...");
  vecteur v1 = {1,2}, v2 = {1.4,1.6};
  vecteur res;
  diff(&v2,&v1,&res);
  assert(fabs(res.x-0.40) < EPS && fabs(res.y - (-0.400)) < EPS);
  printf("ok\n");
  return;
}

void test_scal_prod(){
  printf("TEST_SCAL_PROD...");
  vecteur v = {3,4};
  double alpha = 2.0;
  vecteur res;
  scal_prod(&v, alpha, &res);
  assert(equal_double(res.x, 6.0) && equal_double(res.y, 8.0));
  printf("ok\n");
  return;
}

void test_distance(){
  printf("TEST_DISTANCE...");
  particule p1 = {8,{2.0,4.0}, {1.0,1.0}, {1.0,1.0}, {1.0,1.0},0.0};
  particule p2 = {8,{6.0,4.0}, {1.0,1.0}, {1.0,1.0}, {1.0,1.0}, 0.0};
  assert(equal_double(4.0, distance(&p1, &p2)));
  printf("ok\n");
  return;
}
int main(){
  test_parseur();
  test_somme();
  test_norme();
  test_diff();
  test_scal_prod();
  test_distance();
}
