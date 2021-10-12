#include <iostream>
#include <winbgim.h>
#include <fstream>
#include <cstring>
using namespace std;

ifstream f("map.txt");

struct patratel
{
    int val;
    bool ok;
};


struct matriceMare // 600 x 600 -> latura = 90
{
    int x, y;
    patratel patrat[3][3];
}M[4];

bool m[4][3][3] =
{//piesele
    1, 0, 1,
    1, 1, 1,
    1, 0, 1,

    0, 1, 1,
    1, 1, 1,
    1, 1, 0,

    1, 1, 1,
    1, 0, 0,
    1, 1, 1,

    1, 1, 0,
    1, 1, 1,
    1, 0, 1
};
bool init, gata;
colors cul = YELLOW;

int mapa[5], piesa[5];
int nr = 0;
int sol[10], lung;
int provocari[10][7] =
{
    {5, 2, 2, 2, 1, 1}, // primu element e lungimea solutiei, NIV 1
    {4, 3, 1, 1, 1},//NIV2
    {4, 1, 1, 1, 7},//NIV 3
    {5, 3, 3, 6, 6, 4},//NIV 4
    {4, 1, 1, 1, 4},//NIV 5
    {5, 3, 3, 1, 1, 4},//NIV 6
    {5, 7, 7, 6, 6, 1},//NIV 7
    {6, 2, 2, 2, 1, 1, 1},//NIV 8
    {5, 2, 3, 3, 5, 5}//NIV 9

      /*
    0 -> apa
    1 -> barca buna
    2 -> barca rea
    3 -> epava
    4 -> turn
    5 -> comoara
    6 -> copil
    7 -> tentacula
    8 -> piatra
    */

};
int fereastra = 0;
struct butonel
{
    int x, y;
    int lungime, latime;
    char nume[100];
}start, instr, setari, iesire, inapoi;
int limba = 0;
char cuvinte[5][3][20] =
{
    {"Joaca", "Play", "Jouer"},
    {"Instructiuni", "Instructions","Instructions"},
    {"Setari", "Settings", "Parametres"},
    {"Iesire", "Exit", "Sortir"},
    {"Inapoi", "Back", "Dos"}
};
int xMouse, yMouse;
int nivel = 1, nrNivele = 9;

void patrat(float x, float y, float L);
void initMatMare();
void afisareMatMare(matriceMare M);
void chenarDreaptaJos();
void chenarDreaptaSus(colors cul);
void afisareSolutie();
void afisareMat_m(bool a[3][3], int x, int y, colors cul);
void desenare();
void mutare();
void rotate_(bool a[3][3]);
void punePiesa(int x, int y);
void scoatePiesa(int x, int y);
void refaceVector(int v[5]);
void solutie();
void parcurgereMatriceMare(matriceMare M, int v[10]);
void initButonase();
void afisareButonel(butonel but);
void meniu();
bool peButon(butonel but);
void apasaButoane();
void alegeNivel();

int main()
{
    initwindow(1250, 700);
    initButonase();

    //afisareButonel(inapoi);

    do
    {
        switch(fereastra)
        {
            case 0:
                meniu();
                break;
            case 1:
                do
                {
                    desenare();
                    solutie();
                    mutare();
                    getchar();
                }while(!gata);
                break;
            case 2:
                //instructiuni();
                afisareButonel(inapoi);
                break;
            case 3:
                //setari();
                afisareButonel(inapoi);
                break;
            default:
                break;
        }
         apasaButoane();
    }while(!gata);
    //getchar();
    closegraph();
    return 0;
}

void desenare()
{
    void cleardevice(void);
    patrat(350, 350, 600);
    if(!init)
    {
        initMatMare();
        afisareSolutie();
        for(int i = 0; i < 4; ++i)
            mapa[i] = piesa[i] = 0;
        init = true;
    }
    for(int i = 0; i < 4; ++i)
        afisareMatMare(M[i]);
    chenarDreaptaSus(cul);
    chenarDreaptaJos();
}

void patrat(float x, float y, float L)
{
    float l = L / 2;
    line(x - l, y - l, x + l, y - l);
    line(x + l, y - l, x + l, y + l);
    line(x + l, y + l, x - l, y + l);
    line(x - l, y + l, x - l, y - l);
}

void initMatMare()
{
    M[0].x = M[2].x = 70;  // 50 + 20
    M[1].x = M[3].x = 360;  // 50 + 20 + 3 * 90 + 20
    M[0].y = M[1].y = 70;
    M[2].y = M[3].y = 360;
    for (int k = 0; k < 4; ++k)
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                f >> M[k].patrat[i][j].val;
    /*
    0 -> apa
    1 -> barca buna
    2 -> barca rea
    3 -> epava
    4 -> turn
    5 -> comoara
    6 -> copil
    7 -> tentacula
    8 -> piatra
    */
}

void afisareMatMare(matriceMare M)
{
   int dx, dy, i, j;
   dy = M.y;
   for(i = 0; i < 3; ++i)
   {
        dx = M.x;
        for(j = 0; j < 3; ++j)
        {
            patrat(dx + 90 / 2, dy + 90 /2, 90);
            if(!M.patrat[i][j].ok)
                setfillstyle(SOLID_FILL, M.patrat[i][j].val);
            else
                setfillstyle(SOLID_FILL, YELLOW);
            floodfill(dx + 90 / 2, dy + 90 / 2, WHITE);
            dx += 90;
        }
        dy += 90;
   }
}

void afisareSolutie()
{
    // x = 1100;
    int y = 40, i;
    for(i = 0; i < lung; ++i)
    {
        patrat(1145, y + 90 / 2, 90);
        setfillstyle(SOLID_FILL, sol[i]);
        floodfill(1145, y + 90 / 2, WHITE);
        y += 100;
    }
}

void chenarDreaptaJos()
{
    line(900, 400, 900, 650);
    line(775, 525, 1025, 525);
    outtextxy(830, 455, "1");
    outtextxy(940, 455, "2");
    outtextxy(830, 550, "3");
    outtextxy(940, 550, "4");
}

void chenarDreaptaSus(colors cul)
{
    if(piesa[0])
        afisareMat_m(m[0], 740, 40, RED);
    else
        afisareMat_m(m[0], 740, 40, cul);
    if(piesa[1])
        afisareMat_m(m[1], 910, 40, RED);
    else
        afisareMat_m(m[1], 910, 40, cul);
    if(piesa[2])
        afisareMat_m(m[2], 740, 210, RED);
    else
        afisareMat_m(m[2], 740, 210, cul);
    if(piesa[3])
        afisareMat_m(m[3], 910, 210, RED);
    else
        afisareMat_m(m[3], 910, 210, cul);
}

void afisareMat_m(bool a[3][3], int x, int y, colors cul) // lat -> 140 / 3
{
    float dx, dy;
    dy = y;
    int i, j;
    for(i = 0; i < 3; i++)
    {
        dx = x;
        for(j = 0; j < 3; ++j)
        {
            if(a[i][j] || cul == BLACK)
            {
                patrat(dx + 50 / 2, dy + 50 / 2, 50);
                setfillstyle(SOLID_FILL, cul);
                floodfill(dx + 50 / 2, dy + 50 / 2, WHITE);
            }
            dx += 50;
        }
        dy += 50;

    }
}

void mutare()
{
    // (R x) -> rotire x
    // (x y) -> la chenarul x pune/ia piesa x
    char s[100];
    //cin >> s;
    cin.get(s, 99);
    int k0 = (int)s[0] - '0' - 1; // x
    int k2 = (int)s[2] - '0' - 1;// y
    //cout << "X, Y: " << k0 << ' ' << k2 << '\n';
    if(strlen(s) == 3)
        if(s[0] == 'R'){// rotatie
            if(!piesa[k2])
                rotate_(m[k2]);
            else
                cout << "INVALID \n";
            //cout << k2 << ' ' << piesa[k2] << '\n';
        }
        else
        {
            if(!mapa[k0] && !piesa[k2])
            {
                punePiesa(k0, k2);
                mapa[k0] = piesa[k2] = nr;
            }
            else
                if(mapa[k0] == piesa[k2])
                {
                    mapa[k0] = piesa[k2] = 0;
                    scoatePiesa(k0 ,k2);
                }
                else
                    cout << "INVALID\n";
            /*for(i = 0; i < 4; ++i)
                cout << mapa[i] << ' ';
            cout << '\n';
            for(i = 0; i < 4; ++i)
                cout << piesa[i] << ' ';
            cout << '\n';*/
        }
    else
        cout << "INVALID\n";
    //s[0]='\n';

}

void rotate_(bool a[3][3])
{
    int i, j;
    bool b[3][3];
    for(i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            b[j][3 - i -1] = a[i][j];
    for(i = 0; i < 3; ++i)
        for(j = 0; j < 3; ++j)
            a[i][j] = b[i][j];
    chenarDreaptaSus(BLACK);
}

void punePiesa(int x, int y)
{
    nr++;
    int i, j;
    for(i = 0; i < 3; ++i)
        for(j = 0; j < 3; ++j)
            M[x].patrat[i][j].ok = m[y][i][j];
}

void scoatePiesa(int x, int y)
{
    nr--;
    if(nr < 0)
        nr = 0;
    int i, j;
    for(i = 0; i < 3; ++i)
        for(j = 0; j < 3; ++j)
            M[x].patrat[i][j].ok = false;
    refaceVector(mapa);
    refaceVector(piesa);
}

void refaceVector(int v[5])
{
    int i, k;
    for(k = 1; k <= nr; ++k)
    {
        bool gasit = false;
        for(i = 0; i < 4; ++i)
            if(v[i] == k)
                gasit = true;
        if(!gasit)
            for(i = 0; i < 4; ++i)
                if(v[i] > k)
                    v[i]--;
    }
}

void solutie()
{
    if(nr == 4)
    {
        int v1[10], v2[10], i;
        for(i = 1; i < 9; ++i)
            v1[i] = v2[i] = 0;
        for(i = 0; i < 5; ++i)
            v1[sol[i]]++;
        for(i = 0; i < 4; ++i)
            parcurgereMatriceMare(M[i], v2);
        bool ok = true;
        for(i = 1; i < 9; ++i)
            if(v1[i] != v2[i])
                ok = false;
        if(ok)
        {
            gata = true;
            cout << "BRAVO";
            cleardevice();
            outtextxy(400, 300, "FELICITARI");
        }
    }
}

void parcurgereMatriceMare(matriceMare M, int v[10])
{
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
            if(M.patrat[i][j].ok == 0)
                v[M.patrat[i][j].val]++;
}

void meniu()
{
	settextstyle(1, 0, 7);
	outtextxy(150, 50, "Pirates Hide and Seek");
	afisareButonel(start);
	afisareButonel(instr);
	afisareButonel(setari);
	afisareButonel(iesire);
    alegeNivel();
}

void initButonase()
{
    start.y = 150;
    instr.y = 350;
    setari.y = 450;
    iesire.y = 550;
    inapoi.x = 1100;
    inapoi.y = 600;
    start.latime = instr.latime = setari.latime = inapoi.latime = iesire.latime = 60;
    start.lungime = strlen(cuvinte[0][limba]) * 33;
    start.x = 1250 / 2 - start.lungime / 2;
    strcpy(start.nume, cuvinte[0][limba]);
    strcpy(instr.nume, cuvinte[1][limba]);
    strcpy(setari.nume, cuvinte[2][limba]);
    strcpy(iesire.nume, cuvinte[3][limba]);
    strcpy(inapoi.nume, cuvinte[4][limba]);
    instr.lungime = strlen(cuvinte[1][limba]) * 29;
    setari.lungime = strlen(cuvinte[2][limba]) * 32;
    iesire.lungime = strlen(cuvinte[3][limba]) * 31;
    inapoi.lungime = strlen(cuvinte[4][limba]) * 31;
    instr.x = 1250 / 2 - instr.lungime / 2;
    setari.x = 1250 / 2 - setari.lungime / 2;
    iesire.x = 1250 / 2 - iesire.lungime / 2;
    inapoi.x = 1000;

}

void afisareButonel(butonel but)
{
    line(but.x, but.y, but.x + but.lungime, but.y);
    line(but.x, but.y, but.x, but.y + but.latime);
    line(but.x + but.lungime, but.y, but.x + but.lungime, but.y + but.latime);
    line(but.x, but.y + but.latime, but.x + but.lungime, but.y + but.latime);
    settextstyle(1, 0, 5);
    outtextxy(but.x + 5, but.y + 10 , but.nume);
    setfillstyle(SOLID_FILL,GREEN);
    floodfill(but.x + 1, but.y + 1 ,WHITE);
}

bool peButon(butonel but)
{
    if(but.x <= xMouse && xMouse <= but.x + but.lungime && but.y <= yMouse && yMouse <= but.y + but.latime)
        return true;
    return false;
}

void apasaButoane()
{
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        getmouseclick(WM_LBUTTONDOWN, xMouse, yMouse);
        if(fereastra == 0)
        {
            if(peButon(start))
            {
                lung = provocari[nivel - 1][0];
                for(int i = 1; i <= lung; i++)
                    sol[i - 1] = provocari[nivel - 1][i];
                fereastra = 1;
                cleardevice();
            }
            if(peButon(instr)){
                fereastra = 2;
                cleardevice();
            }
            if(peButon(setari)){
                fereastra = 3;
                cleardevice();
            }
            if(peButon(iesire))
                exit(0);
            if(520 <= xMouse && xMouse <= 580 && 240 <= yMouse && yMouse <= 300){
                nivel--;
                cleardevice();
            }
            if(680 <= xMouse && xMouse <= 740 && 240 <= yMouse && yMouse <= 300){
                nivel++;
                cleardevice();
            }
            if(nivel < 1)
                nivel = 1;
            if(nivel > nrNivele)
                nivel = nrNivele;

        }
        if(peButon(inapoi) && fereastra > 1)
        {
            fereastra = 0;
            cleardevice();
        }
    }
}

void alegeNivel()
{
    char s[100];
    itoa(nivel, s, 10); // pune valoarea nivelului in sir, ca sa poata fi afisat
    outtextxy(615, 250, s);
    patrat(550, 270, 60);
    outtextxy(538, 250, "-");
    settextstyle(1, 0, 5);
    patrat(710, 270, 60);
    outtextxy(690, 250, "+");

}













