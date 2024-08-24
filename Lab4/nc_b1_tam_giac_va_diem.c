#include <stdio.h>
#include <math.h>

typedef struct Point{
    char *pt_name;
    double x, y;
}Point;

void input_point(Point *point);
double area(Point A, Point b, Point c);
double dis(Point A, Point b);

int main() {
    Point A = {"A"}, B = {"B"}, C = {"C"}, D = {"D"};
    double S;

    input_point(&A);
    input_point(&B);
    input_point(&C);
    input_point(&D);

    S = area(A, B, C);

    if (S != 0) {
        printf("ABC la tam giac va ");
        if (dis(A, D) + dis(D, B) == dis(A, B)) printf("D nam tren AB.\n");
        else if (dis(A, D) + dis(D, C) == dis(A, C)) printf("D nam tren AC.\n");
        else if (dis(B, D) + dis(D, C) == dis(B, C)) printf("D nam tren BC.\n");
        else if (S == (area(A, B, D) + area(A, C, D) + area(B, C, D))) printf("va D nam ben trong ABC.\n");
        else printf("D nam ben ngoai ABC.\n");
    } else printf("ABC khong phai tam giac.\n");

    return 0;
}

void input_point(Point *pt) {
    printf("Nhap x, y cua %s: ", pt->pt_name);
    scanf("%lf%lf", &pt->x, &pt->y);
}

double area(Point a, Point b, Point c) {
    return fabs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2;
}

double dis(Point a, Point b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}