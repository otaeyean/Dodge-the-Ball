#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

// 색상 정의
#define BLACK   0
#define BLUE1   1
#define GREEN1   2
#define CYAN1   3
#define RED1   4
#define MAGENTA1 5
#define YELLOW1   6
#define GRAY1   7
#define GRAY2   8
#define BLUE2   9
#define GREEN2   10
#define CYAN2   11
#define RED2   12
#define MAGENTA2 13
#define YELLOW2   14
#define WHITE   15

#define ESC 0x1b //  ESC 누르면 종료

#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define WIDTH 79  //가로
#define HEIGHT 28 //세로

typedef struct Object
{
    int x, y;
}Object;

//유저의 x,y좌표
int userX;
int userY;

// 장애물들의 위치 선언. 각 배열의 인덱스마다 장애물의 위치를뜻함. (X축 장애물 3개, Y축 장애물 3개) 
int Up_objectX[3] = { 10,10,10 }, Up_objectY[3] = { 10,10,10 };
int Left_objectX[3] = { 3,3,3 }, Left_objectY[3] = { 10,10,10 };
int Right_objectX[3] = { 3,3,3 }, Right_objectY[3] = { 10,10,10 };
int Down_objectX[3] = { 10,10,10 }, Down_objectY[3] = { 10,10,10 };

// 폭탄,골드,하트,무적의 x,y좌표
int goldX, goldY;
int heartX, heartY;
int unbeatableX, unbeatableY;
int boomX, boomY;


int m = 1; //1이면 무적아님 , 0이면 무적
int m_time = 200; //무적 유효시간
int frame_count = 0;

int hp = 3; //하트
int score = 0;
char name[100];

int golds[WIDTH][HEIGHT] = { 0 }; // 1이면 Gold 있다는 뜻
int hearts[WIDTH][HEIGHT] = { 0 }; // 1이면 heart 있다는 뜻
int unbeatables[WIDTH][HEIGHT] = { 0 }; // 1이면 무적 있다는 뜻
int booms[WIDTH][HEIGHT] = { 0 }; // 1이면 무적 있다는 뜻

//사용한 함수
void removeCursor(void);   //커서 지우는 함수
void showCursor(void);     //커서 드러내는 함수
void gotoxy(int x, int y);     //x,y축으로 이동
void textcolor(int fg_color, int bg_color);     //텍스트와 텍스트배경 컬러
void cls(int bg_color, int text_color);     //배경 색 설정
void background_intro(void); //배경 색칠하기
void draw_box(int x1, int y1, int x2, int y2, char* ch); //상자그리기
void draw_box2(int x1, int y1, int x2, int y2); //상자그리기
void rule(void);   //2번 눌렀을때 나오는 룰
void clean(void);//화면 싹 날리는 함수
void print_gamename(void);
void print_score(void);
void print_life(void);
void print_rule(void);
void intro(void); //WATCH OUT 인트로
void game_intro();

int check(int userX, int userY, int objectX, int objectY); //충돌처리
int check_gold(int x, int y);  //주인공 좌표를 입력받고
int check_heart(int x, int y);  //주인공 좌표를 입력받고
int check_unbeatable(int x, int y);  //주인공 좌표를 입력받고

void show_heart();
void show_unbeatable();
void show_hp(int hp);
void show_gold();

void Up_object_position(int x); //장애물 위치변경
void Down_object_position(int x);  //장애물 위치변경
void Left_object_position(int x); //장애물 위치변경
void Right_object_position(int x); //장애물 위치변경

void Right_putobj(int x);  //장애물 생성
void Left_putobj(int x);  //장애물 생성
void Down_putobj(int x);  // 장애물 생성 
void Up_putobj(int x);   // 장애물 생성 

void Right_eraseobj(int x);   //장애물 지우기
void Left_eraseobj(int x);   //장애물 지우기
void Down_eraseobj(int x);   //장애물 지우기
void Up_eraseobj(int x);    //장애물 지우기
void heart_eraseobj(int x, int y);
void gold_eraseobj(int x, int y);
void unbeatable_eraseobj(int x, int y);
void user_eraseobj(int x, int y);

void player(int x, int y);
void elaseplayer(int x, int y);
void putplayer(int x, int y);

////////////////////////////////////////
void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
    COORD pos; // Windows.h 에 정의
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int fg_color, int bg_color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

void removeCursor(void) { // 커서를 안보이게 한다
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void showCursor(void) { // 커서를 보이게 한다
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void cls(int text_color, int bg_color) // 화면 지우기
{
    char cmd[100];
    system("cls");
    sprintf(cmd, "COLOR %x%x", bg_color, text_color);
    system(cmd);
}

void background_intro(void) { //배경 색칠하기
    int i, j;
    textcolor(0, 0);
    gotoxy(1, 1);
    for (i = 0; i < 130; i++) {
        for (j = 0; j < 30; j++) {
            gotoxy(i, j);
            printf(" ");
        }
    }

}

void draw_box(int x1, int y1, int x2, int y2, char* ch)
{
    int x, y;
    for (x = x1; x <= x2; x += 2) {
        gotoxy(x, y1);
        printf("%s", ch);
        gotoxy(x, y2);
        printf("%s", ch);
    }
    for (y = y1; y <= y2; y++) {
        gotoxy(x1, y);
        printf("%s", ch);
        gotoxy(x2, y);
        printf("%s", ch);
    }
}
void draw_box2(int x1, int y1, int x2, int y2)
{
    int x, y;

    for (x = x1; x <= x2; x += 2) { // 한글은 2칸씩 차지한다.
        gotoxy(x, y1);
        printf("%s", "─"); // ㅂ 누르고 한자키 누르고 선택
        gotoxy(x, y2);
        printf("%s", "─");
    }
    for (y = y1; y <= y2; y++) {
        gotoxy(x1, y);
        printf("%s", "│");
        gotoxy(x2, y);
        printf("%s", "│");
    }
    gotoxy(x1, y1); printf("┌");
    gotoxy(x1, y2); printf("└");
    gotoxy(x2, y1); printf("┐");
    gotoxy(x2, y2); printf("┘");
}


void rule(void) {   //2번 눌렀을때 나오는 룰

    draw_box2(10, 2, 50, 20);

    gotoxy(11, 5);
    printf("1. 무작위로 나오는 총알을 피해라!");
    gotoxy(11, 7);
    printf("2. 움직이는 방향키는: →  ←  ↑  ↓ ");
    gotoxy(11, 9);
    printf("3. 시간이 지날수록 점수는 쌓인다.");
    gotoxy(11, 11);
    printf("4. U 먹으면 7초간 무적 ");
    gotoxy(11, 13);
    printf("5. G 먹으면 점수 추가 ");
    gotoxy(11, 15);
    printf("6. H 먹으면 목숨 추가  ");
    gotoxy(11, 17);
    printf("6. B 먹으면 게임 over  ");
    gotoxy(11, 19);
    printf("키보드 아무 키나 누르시오 ...  ");

}



void clean(void) { //화면 싹 날리는 함수

    int i, j;
    textcolor(0, 2);
    for (i = 3; i < 79; i++) {
        for (j = 2; j < 28; j++)
        {
            gotoxy(i, j);
            printf(" ");
        }
    }
}
void print_gamename(void)
{
    gotoxy(85, 5);
    printf(" Watch OUT!");
    gotoxy(85, 6);
    printf(" 제작자 :오태연");
}
void print_score(void) {
    gotoxy(85, 9);
    printf("score:");
}
void print_life(void) {
    gotoxy(85, 14);
    printf("목숨:");
}
void print_rule(void) {
    gotoxy(85, 20);
    printf("방향 : ← → ↑ ↓\n");
    gotoxy(85, 21);
    printf("G ▶점수 추가\n");
    gotoxy(85, 22);
    printf("H ▶목숨 추가 \n");
    gotoxy(85, 23);
    printf("U ▶7초 무적상태\n");
    gotoxy(85, 24);
    printf("B ▶게임 over\n");

}
void intro(void)  //WATCH OUT 인트로 ,
{

    int n;

    draw_box(1, 1, 109, 29, "◆");
    gotoxy(25, 4);
    printf("■");
    gotoxy(25, 5);
    printf("■");
    gotoxy(25, 6);
    printf("■");
    gotoxy(25, 7);
    printf("■");
    gotoxy(25, 8);
    printf("■");
    gotoxy(27, 7);
    printf("■");
    gotoxy(29, 6);
    printf("■");
    gotoxy(31, 7);
    printf("■");
    gotoxy(33, 8);
    printf("■");
    gotoxy(33, 7);
    printf("■");
    gotoxy(33, 6);
    printf("■");
    gotoxy(33, 5);
    printf("■");
    gotoxy(33, 4);
    printf("■");


    gotoxy(39, 5);
    printf("■ ");
    gotoxy(37, 6);
    printf("■ ");
    gotoxy(37, 7);
    printf("■ ");
    gotoxy(37, 8);
    printf("■ ");

    gotoxy(39, 6);
    printf("■ ");
    gotoxy(41, 4);
    printf("■ ");
    gotoxy(41, 6);
    printf("■ ");

    gotoxy(43, 6);
    printf("■ ");

    gotoxy(43, 6);
    printf("■ ");

    gotoxy(43, 5);
    printf("■ ");
    gotoxy(45, 6);
    printf("■ ");
    gotoxy(45, 7);
    printf("■ ");
    gotoxy(45, 8);
    printf("■ ");    //A완성

    gotoxy(49, 4);
    printf("■ ");
    gotoxy(51, 4);
    printf("■ ");
    gotoxy(53, 4);
    printf("■ ");
    gotoxy(55, 4);
    printf("■ ");
    gotoxy(57, 4);
    printf("■ ");
    gotoxy(53, 5);
    printf("■ ");
    gotoxy(53, 6);
    printf("■ ");
    gotoxy(53, 7);
    printf("■ ");
    gotoxy(53, 8);
    printf("■ ");

    //T완성
    gotoxy(61, 4);
    printf("■ ");
    gotoxy(63, 4);
    printf("■ ");
    gotoxy(65, 4);
    printf("■ ");
    gotoxy(67, 4);
    printf("■ ");
    gotoxy(69, 4);
    printf("■ ");
    gotoxy(61, 5);
    printf("■ ");
    gotoxy(61, 6);
    printf("■ ");
    gotoxy(61, 7);
    printf("■ ");
    gotoxy(61, 8);
    printf("■ ");
    gotoxy(63, 8);
    printf("■ ");
    gotoxy(65, 8);
    printf("■ ");
    gotoxy(67, 8);
    printf("■ ");
    gotoxy(69, 8);
    printf("■ ");
    //c완성
    gotoxy(73, 4);
    printf("■ ");
    gotoxy(73, 5);
    printf("■ ");
    gotoxy(73, 6);
    printf("■ ");
    gotoxy(73, 7);
    printf("■ ");
    gotoxy(73, 8);
    printf("■ ");
    gotoxy(75, 6);
    printf("■ ");
    gotoxy(77, 6);
    printf("■ ");
    gotoxy(79, 6);
    printf("■ ");
    gotoxy(81, 4);
    printf("■ ");
    gotoxy(81, 5);
    printf("■ ");
    gotoxy(81, 6);
    printf("■ ");
    gotoxy(81, 7);
    printf("■ ");
    gotoxy(81, 8);
    printf("■ ");
    //h완성
    gotoxy(37, 13);
    printf("■ ");
    gotoxy(37, 14);
    printf("■ ");
    gotoxy(37, 15);
    printf("■ ");
    gotoxy(37, 16);
    printf("■ ");
    gotoxy(37, 17);
    printf("■ ");
    gotoxy(39, 13);
    printf("■ ");
    gotoxy(41, 13);
    printf("■ ");
    gotoxy(43, 13);
    printf("■ ");
    gotoxy(45, 13);
    printf("■ ");
    gotoxy(45, 14);
    printf("■ ");
    gotoxy(45, 15);
    printf("■ ");
    gotoxy(45, 16);
    printf("■ ");
    gotoxy(45, 17);
    printf("■ ");
    gotoxy(39, 17);
    printf("■ ");
    gotoxy(41, 17);
    printf("■ ");
    gotoxy(43, 17);
    printf("■");
    //o완성

    gotoxy(49, 13);
    printf("■ ");
    gotoxy(49, 14);
    printf("■ ");
    gotoxy(49, 15);
    printf("■ ");
    gotoxy(49, 16);
    printf("■ ");
    gotoxy(49, 17);
    printf("■ ");
    gotoxy(51, 17);
    printf("■ ");
    gotoxy(53, 17);
    printf("■ ");
    gotoxy(55, 17);
    printf("■ ");
    gotoxy(57, 17);
    printf("■ ");
    gotoxy(57, 16);
    printf("■ ");
    gotoxy(57, 15);
    printf("■ ");
    gotoxy(57, 14);
    printf("■ ");
    gotoxy(57, 13);
    printf("■ ");  //u완

    gotoxy(61, 13);
    printf("■ ");
    gotoxy(63, 13);
    printf("■ ");
    gotoxy(65, 13);
    printf("■ ");
    gotoxy(67, 13);
    printf("■ ");
    gotoxy(69, 13);
    printf("■ ");
    gotoxy(65, 14);
    printf("■ ");
    gotoxy(65, 15);
    printf("■ ");
    gotoxy(65, 16);
    printf("■ ");
    gotoxy(65, 17);
    printf("■ ");
    //t완
    textcolor(RED1, BLACK);

    gotoxy(73, 13);
    printf("■ ");
    gotoxy(73, 14);
    printf("■ ");
    gotoxy(73, 15);
    printf("■ ");
    gotoxy(73, 17);
    printf("■ ");

    gotoxy(39, 23);
    textcolor(15, 0);
    printf("player의 이름을 입력하시오:");
    gotoxy(67, 23);
    showCursor();
    scanf("%s", name);
    gotoxy(39, 25);
    printf("게임시작 : (1)\n");
    gotoxy(39, 26);

    printf("설명서   : (2)");

    scanf("%d", &n);
    if (n == 2) {
        clean();
        rule();
        _getch();

    }
    removeCursor();
    //인트로 끝

}


int check(int userX, int userY, int objectX, int objectY)  //충돌처리
{
    if (objectX == userX && objectY == userY)
    {
        return 1;  //충돌 했을 때 1 return
    }
    return 0;  //충돌하지 않는 경우
}


void game_intro() {
    removeCursor(); // 커서를 안보이게 한다

    cls(BLACK, RED1);
    background_intro();


    textcolor(BLUE1, BLACK);
    intro();
    gotoxy(21, 15); // 끝나는 화면 위치
    clean();

    cls(BLUE1, YELLOW2);
    textcolor(BLACK, YELLOW2);


    draw_box(1, 1, 79, 28, "▩"); //게임 실행창 박스
    draw_box2(81, 1, 106, 28); // 오른쪽 창 박스 
    draw_box2(83, 4, 104, 7); //제작자
    draw_box2(83, 8, 104, 12); //score
    draw_box2(83, 13, 104, 17); //목숨
    draw_box2(83, 19, 104, 26); //간단한 룰
    print_gamename();
    print_score();
    print_life();
    print_rule();

}
void show_hp(int hp)  //hp는 전역변수로 초기값은 3, 장애물과 부딪힐시 --hp; 통해서 하트 한 개 감소
{
    int i, j;

    gotoxy(90, 14);
    for (i = hp; i > 0; i--) {
        printf("♥ ");
    }
    for (j = 3 - hp; j > 0; j--) {
        printf("♡ ");
    }
}

// 임의의 위치에 gold를 표시한다.
#define GOLD   "G"
void show_gold()
{

    goldX = rand() % (WIDTH - 5) + 3;

    goldY = rand() % (HEIGHT - 3) + 2;  // 제일 상단은 피한다
    textcolor(YELLOW2, GRAY1);
    gotoxy(goldX, goldY);
    printf(GOLD);
    golds[goldX][goldY] = 1;
    textcolor(BLACK, YELLOW2);
}
void gold_eraseobj(int x, int y)    //장애물 지우기
{
    gotoxy(x, y);
    golds[x][y] = 0;
    putchar(' ');
}
#define BOOM   "B"
void show_boom()
{

    boomX = rand() % (WIDTH - 5) + 3;

    boomY = rand() % (HEIGHT - 3) + 2;  // 제일 상단은 피한다
    textcolor(YELLOW2, BLACK);
    gotoxy(boomX, boomY);
    printf(BOOM);
    booms[boomX][boomY] = 1;
    textcolor(BLACK, YELLOW2);
}
void boom_eraseobj(int x, int y)    //장애물 지우기
{
    gotoxy(x, y);
    booms[x][y] = 0;
    putchar(' ');
}

//하트 아이템
#define heart   "H"

void show_heart()  //H 출력
{
    int x, y;
    x = rand() % (WIDTH - 5) + 3;
    y = rand() % (HEIGHT - 3) + 2;
    textcolor(YELLOW2, RED1);
    gotoxy(x, y);
    printf("H");
    hearts[x][y] = 1;
    textcolor(BLACK, YELLOW2);
}
void heart_eraseobj(int x, int y)    //장애물 지우기
{
    gotoxy(x, y);
    hearts[x][y] = 0;

    putchar(' ');
}
//무적아이템
#define unbeatable   "U"
void show_unbeatable()
{
    int x, y;
    x = rand() % (WIDTH - 5) + 3;
    y = rand() % (HEIGHT - 3) + 2;  // 제일 상단은 피한다
    textcolor(YELLOW2, BLUE1);
    gotoxy(x, y);
    printf(unbeatable);
    unbeatables[x][y] = 1;
    textcolor(BLACK, YELLOW2);
}
void unbeatable_eraseobj(int x, int y)    //장애물 지우기
{
    gotoxy(x, y);
    unbeatables[x][y] = 0;

    putchar(' ');
}
int check_gold(int x, int y)  //주인공 좌표를 입력받고
{
    if (golds[x][y] == 1)
        return 1;
    else return 0;
}
int check_heart(int x, int y)  //주인공 좌표를 입력받고
{
    if (hearts[x][y] == 1)
        return 1;
    else return 0;
}
int check_unbeatable(int x, int y)  //주인공 좌표를 입력받고
{
    if (unbeatables[x][y] == 1)
        return 1;
    else return 0;
}
int check_boom(int x, int y)  //주인공 좌표를 입력받고
{
    if (booms[x][y] == 1)
        return 1;
    else return 0;
}
// Up_objectX , Up_objectY 각 배열에 존재하는 x번째 인덱스 값에 있는 위치정보를 변경하여 장애물의 움직임을 구현한다. 
void Up_putobj(int x)   // 장애물 생성 
{
    gotoxy(Up_objectX[x], Up_objectY[x]);
    putchar('*');
}

void Up_eraseobj(int x)    //장애물 지우기
{
    gotoxy(Up_objectX[x], Up_objectY[x]);
    putchar(' ');
}


void Up_object_position(int x) { //장애물 위치변경
    Up_eraseobj(x);
    if (Up_objectY[x] < HEIGHT - 1)
        Up_objectY[x]++;

    else {
        Up_objectX[x] = rand() % WIDTH + 3;
        if (Up_objectX[x] >= 78) Up_objectX[x] = 78;
        Up_objectY[x] = 2;
    }
    Up_putobj(x);
}
void Down_putobj(int x)   // 장애물 생성 
{
    gotoxy(Down_objectX[x], Down_objectY[x]);
    putchar('*');
}

void Down_eraseobj(int x)    //장애물 지우기
{
    gotoxy(Down_objectX[x], Down_objectY[x]);
    putchar(' ');
}


void Down_object_position(int x) { //장애물 위치변경
    Down_eraseobj(x);
    if (Down_objectY[x] > 2)
        Down_objectY[x]--;

    else {
        Down_objectX[x] = rand() % WIDTH + 3;
        if (Down_objectX[x] >= 78) Down_objectX[x] = 78;
        Down_objectY[x] = 27;
    }
    Down_putobj(x);
}

void Left_putobj(int x)   //장애물 생성
{
    gotoxy(Left_objectX[x], Left_objectY[x]);
    putchar('*');
}

void Left_eraseobj(int x)    //장애물 지우기
{
    gotoxy(Left_objectX[x], Left_objectY[x]);
    putchar(' ');
}


void Left_object_position(int x) { //장애물 위치변경
    Left_eraseobj(x);
    if (Left_objectX[x] < WIDTH - 1)
        Left_objectX[x]++;
    else {
        Left_objectY[x] = rand() % HEIGHT + 3;
        if (Left_objectY[x] > 27) Left_objectY[x] = 27;
        Left_objectX[x] = 3;

    }
    Left_putobj(x);
}
void Right_putobj(int x)   //장애물 생성
{
    gotoxy(Right_objectX[x], Right_objectY[x]);
    putchar('*');
}

void Right_eraseobj(int x)    //장애물 지우기
{
    gotoxy(Right_objectX[x], Right_objectY[x]);
    putchar(' ');
}


void Right_object_position(int x) { //장애물 위치변경
    Right_eraseobj(x);
    if (Right_objectX[x] > 3)
        Right_objectX[x]--;
    else {
        Right_objectY[x] = rand() % HEIGHT + 3;
        if (Right_objectY[x] > 27) Right_objectY[x] = 27;
        Right_objectX[x] = 78;

    }
    Right_putobj(x);
}

void player(int x, int y) {
    gotoxy(x, y);
    printf("@");
}

void putplayer(int x, int y) {
    gotoxy(x, y);
    textcolor(MAGENTA2, YELLOW2);
    printf("@");
    textcolor(BLACK, YELLOW2);

}
void eraseplayer(int x, int y) {
    gotoxy(x, y);
    printf(" ");
}

int main()
{
    int i;
    time_t start_time; // 아이템을 먹은 시간 저장 변수

    srand(time(NULL));
    game_intro();
    // 게임 시작
    show_hp(hp);
    char ch = 0;
    userX = 30;
    userY = 15;

    while (1)
    {
        if (hp <= 0)  //hp<=0 게임 종료
        {
            clean();
            while (1) {
                int c1, c2;
                do { // 색을 변경하면서 Game Over 출력
                    c1 = rand() % 16;
                    c2 = rand() % 16;
                } while (c1 == c2);
                textcolor(c1, c2);
                Sleep(100);
                gotoxy(36, 12);
                printf("**game over!**");
                gotoxy(30, 14);
                textcolor(BLACK, GREEN2);

                printf("%s player의 score is %d!", name, score);

                //끝
                gotoxy(29, 16);
                printf("종료하려면 ESC를 입력하세요!");

                if (_kbhit() == 1) {
                    ch = _getch();
                    if (ch == ESC) {
                        gotoxy(2, 30);
                        break;

                    }
                }
            }
            break;

        }

        if ((score % 3) == 0) {
            //점수가 쌓일수록 점점 난이도 증가 시키기위해 장애물 개수 늘림
            if (score < 10000) {
                Up_object_position(0);
                Down_object_position(0);

            }
            if (score > 10000 && score < 15000) {
                Up_object_position(0);
                Down_object_position(0);

                Left_object_position(0);
                Right_object_position(0);

            }
            if (score > 15000 && score < 30000) {
                Up_object_position(0);
                Up_object_position(1);
                Left_object_position(0);
                Left_object_position(1);
                Right_object_position(0);
                Right_object_position(1);
                Down_object_position(0);
                Down_object_position(1);


            }
            if (score > 30000) {
                Up_object_position(0);
                Up_object_position(1);
                Up_object_position(2);
                Right_object_position(0);
                Right_object_position(1);
                Right_object_position(2);
                Down_object_position(0);
                Down_object_position(1);
                Down_object_position(2);

                Left_object_position(0);
                Left_object_position(1);
                Left_object_position(2);
            }
            if (check(userX, userY, Up_objectX[0], Up_objectY[0]) && m == 1) {

                show_hp(--hp);
                Up_eraseobj(0);
                continue;
            }
            else if (check(userX, userY, Up_objectX[1], Up_objectY[1]) && m == 1) {
                show_hp(--hp);
                Up_eraseobj(1);
                continue;
            }
            else if (check(userX, userY, Up_objectX[2], Up_objectY[2]) && m == 2) {
                show_hp(--hp);
                Up_eraseobj(2);
                continue;
            }
            // Y축이동 장애물 충돌 감지 (HP 중복 감소 버그방지를 위해 반복문 미사용.) 
            if (check(userX, userY, Down_objectX[0], Down_objectY[0]) && m == 1) {
                show_hp(--hp);
                Down_eraseobj(0);
                continue;
            }
            else if (check(userX, userY, Down_objectX[1], Down_objectY[1]) && m == 1) {
                show_hp(--hp);
                Down_eraseobj(1);
                continue;
            }
            else if (check(userX, userY, Down_objectX[2], Down_objectY[2]) && m == 2) {
                show_hp(--hp);
                Down_eraseobj(2);
                continue;
            }
            // X축 이동 장애물 충돌감지 (HP 중복 감소 버그방지를 위해 반복문 미사용.)
            if (check(userX, userY, Left_objectX[0], Left_objectY[0]) && m == 1) {
                show_hp(--hp);
                Left_eraseobj(0);
                continue;
            }
            else if (check(userX, userY, Left_objectX[1], Left_objectY[1]) && m == 1) {
                show_hp(--hp);
                Left_eraseobj(1);
                continue;
            }
            else if (check(userX, userY, Left_objectX[2], Left_objectY[2]) && m == 1) {
                show_hp(--hp);
                Left_eraseobj(2);
                continue;
            }
            // X축 이동 장애물 충돌감지 (HP 중복 감소 버그방지를 위해 반복문 미사용.)
            if (check(userX, userY, Right_objectX[0], Right_objectY[0]) && m == 1) {
                show_hp(--hp);
                Right_eraseobj(0);
                continue;
            }
            else if (check(userX, userY, Right_objectX[1], Right_objectY[1]) && m == 1) {
                show_hp(--hp);
                Right_eraseobj(1);
                continue;
            }
            else if (check(userX, userY, Right_objectX[2], Right_objectY[2]) && m == 1) {
                show_hp(--hp);
                Right_eraseobj(2);
                continue;
            }
            Sleep(30);
        }


        int randitem;
        if ((score % 500) == 0) {
            randitem = rand() % 2 + 1;  //1~2
            int u;
            for (u = 0; u < randitem; u++) {
                show_gold();     //랜덤 개수로 골드 떨어짐
            }

        }
        if ((score % 30000) == 0) {
            show_heart();     //하트 떨어짐
        }
        if ((score % 15000) == 0) {
            show_unbeatable();  //무적 떨어짐
        }
        if ((score % 10000) == 0) {

            for (i = 0; i < 3; i++) {
                show_boom(); //폭탄 떨어짐

            }
        }
        gotoxy(30, 20);
        gotoxy(userX, userY);
        printf("@");
        // Y축이동 장애물 충돌 감지 (HP 중복 감소 버그방지를 위해 반복문 미사용.) 

        //폭탄 충돌 처리
        if (check_boom(userX, userY))
        {
            if (m == 1)
            {
                if (hp == 3) {
                    show_hp(--hp);
                    show_hp(--hp);
                    show_hp(--hp);
                }
                else if (hp == 2) {
                    show_hp(--hp);
                    show_hp(--hp);
                }
                else show_hp(--hp);
            }
            boom_eraseobj(userX, userY);
        }
        //골드 충돌 처리
        if (check_gold(userX, userY))
        {
            score += 5000;
            gold_eraseobj(userX, userY);
        }
        // 하트 충돌 처리
        if (check_heart(userX, userY)) {

            if (hp < 3) { //hp가 3일땐 더 채워지지 않음.
                show_hp(++hp);  //에너지 회복 아이템
                heart_eraseobj(userX, userY);
            }
        }
        //무적 충돌 처리
        if (check_unbeatable(userX, userY))
        {
            m = 0;
            start_time = time(NULL);
            unbeatable_eraseobj(userX, userY);
        }
        if (m == 0) {
            // 현재 시간이 무적 시작 시간부터 7초 이내인지 확인
            time_t current_time = time(NULL);
            if (current_time - start_time < 7) {
                eraseplayer(userX, userY);
                putplayer(userX, userY);
                // 플레이어가 무적인 동안 처리할 로직 작성

            }
            else {
                m = 1; // 7초가 지나면 무적 상태 해제
                eraseplayer(userX, userY);
                player(userX, userY);

            }
        }

        //방향키로 user 좌표 움직이기
        if (_kbhit())
        {
            gotoxy(userX, userY);
            printf("  ");

            ch = _getch();
            switch (ch)
            {
            case LEFT:
                userX--;
                if (userX <= 3) userX = 3;

                break;
            case RIGHT:
                userX++;
                if (userX > 77) userX = 77;
                break;

            case UP:
                userY--;
                if (userY < 2) userY = 2;
                break;

            case DOWN:
                userY++;
                if (userY > 27) userY = 27;
                break;
            }

        }

        //점수
        gotoxy(93, 9);
        printf("%d", score);
        score++;
    }
}
