#include<stdio.h>
#include<curses.h>
#include<unistd.h>
#include<math.h>

#define WAIT 100000

typedef struct POINT{
  int y;
  int x;
} Point;

int draw_point(Point a, char c);

void enter(Point a);

void choice_enter(Point a, int number);

void draw_button(Point a, char n);

void frame_input(Point p, char *order);

void choice_number(Point a, int  number);

int change_HM(char *hai);

int change_MH(int money, int num);

void print_money(Point a, char *hai);

void balance_frame(Point a, char *order, char *hai);

void choice_a(Point a);

void choice_b(Point a);

void print_pass(Point a, int num);

void frame_pass(Point a, char *order);

void frame_record(Point a);

int main(void)
{
  char password[]={'1', '6', '0', '2'};
  int max_y, max_x;
  Point a;
  char number_input[12];
  int count_input=0;
  char order_input[]="please input";
  char order_balance[]="balance";
  char order_password[]="password";
  char key;
  int command=4;
  int command_enter=0;
  int command_number;
  int count;
  int total=0;
  char dummy[10];
  int index=1;
  int check=0;
  int mot;
  int command_0=0;
  int count_pass=0;
  char input_pass[5];
  int command_pass=0;
  FILE *fp;
  int c=0;

  number_input[0]='\0';

  initscr();
  noecho();
  timeout(0);
  curs_set(0);
  cbreak();
  getmaxyx(stdscr, max_y, max_x);

  if(max_y<20 || max_x<15){
    endwin();
    printf("please widen the screen\n");
    return 1;
  }

  a.y=0;
  a.x=0;

  if((fp=fopen("passbook.dat", "r"))==NULL){
    endwin();
    printf("can not open file\n");
    return 1;
  }

  while(fscanf(fp, "%d", &total)!=EOF);

  fclose(fp);

  while(1){
    key=getch();
    if(key=='q'){
      break;
    }

    if(key=='p'){
      total=999999999;
    }

    switch (command){
      case 4:
        enter(a);
        choice_enter(a, command_enter);
        if(key=='w'){
          if(command_enter>0){
            command_enter--;
          }
        }
        if(key=='s'){
          if(command_enter<3){
            command_enter++;
          }
        }
        if(key=='a'){
          command=command_enter;
          clear();
          refresh();
        }
        break;
      case 0:
        frame_input(a, order_input);
        if(command_0==0 && key=='0'){

        }else if(48<=key && key<=57){
          command_0=1;
          command_number=key-48;
          choice_number(a, command_number);
          if(count_input<9){
            number_input[count_input]=key;
            number_input[count_input+1]='\0';
            count_input++;
          }
        }
        print_money(a, number_input);
        if(number_input[0]!='\0' && key=='a'){
          total=total+change_HM(number_input);
          if(total>999999999){
            total=999999999;
          }
          choice_a(a);
          refresh();
          usleep(WAIT);
          clear();
          refresh();
          number_input[0]='\0';
          count_input=0;
          command=3;
        }
        if(key=='b'){
          choice_b(a);
          refresh();
          usleep(WAIT);
          clear();
          refresh();
          number_input[0]='\0';
          count_input=0;
          command=4;
        }
        break;
      case 1:
        command_number=key-48;
        switch (command_pass){
          case 0://パスワード画面
           frame_pass(a, order_password);
           if(key=='b'){
             choice_b(a);
             refresh();
             usleep(WAIT);
             clear();
             refresh();
             number_input[0]='\0';
             count_input=0;
             count_pass=0;
             command_pass=0;
             command=4;
           }
           if(48<=key && key<=57 && count_pass!=4){
             input_pass[count_pass]=key;
             count_pass++;
           choice_number(a, command_number);
           if(1<=count_pass && count_pass<=4){
             print_pass(a, count_pass);
           }
           }
           if(count_pass==4){
             if(input_pass[0]==password[0] && input_pass[1]==password[1] && input_pass[2]==password[2] && input_pass[3]==password[3]){
               command_pass=1;
               clear();
             }else{
              clear();
              count_pass=0;
              command=4;
           }
         }
           break;
          case 1://金額入力
          frame_input(a, order_input);
          if(command_0==0 && key=='0'){

          }else if(48<=key && key<=57){
            command_0=1;
            command_number=key-48;
            choice_number(a, command_number);
            if(count_input<9){
              number_input[count_input]=key;
              number_input[count_input+1]='\0';
              count_input++;
            }
          }
          print_money(a, number_input);
          if(number_input[0]!='\0' && key=='a'){
            total=total-change_HM(number_input);
            if(total<0){
              total=0;
            }
            choice_a(a);
            refresh();
            usleep(WAIT);
            clear();
            refresh();
            number_input[0]='\0';
            count_input=0;
            count_pass=0;
            command_pass=0;
            command=3;
          }
          if(key=='b'){
            choice_b(a);
            refresh();
            usleep(WAIT);
            clear();
            refresh();
            number_input[0]='\0';
            count_input=0;
            count_pass=0;
            command_pass=0;
            command=4;
          }
        }
        break;
      case 2:
        frame_record(a);
	fp=fopen("passbook.dat", "a");
        if(key=='a'){
          fprintf(fp, "%d %c", total, '\n');
          choice_a(a);
          refresh();
          usleep(WAIT);
          clear();
          refresh();
          command=4;
        }
        fclose(fp);
        break;
      case 3:
      if(total==0){
        dummy[0]='0';
        dummy[1]='\0';
      }else{
        dummy[0]='\0';
        index=0;
        while(check==0){
          mot=pow(10, index);
          if(total/mot!=0){
            index++;
          }else{
            check=1;
          }
        }
        check=0;

        for(count=0; count<index; count++){
          dummy[count]=change_MH(total, count)+48;
          dummy[count+1]='\0';
        }
      }

        balance_frame(a, order_balance, dummy);
        if(key=='a'){
          command=4;
          clear();
          refresh();
        }
        break;
    }

    refresh();
    usleep(WAIT);
  }

  endwin();

  return 0;

}

int draw_point( Point a, char c )
{
  int status;
  int max_y, max_x;

  getmaxyx( stdscr, max_y, max_x);
  if( (0<=a.y)&&(a.y<max_y) && (0<=a.x)&&(a.x<max_x) ){
     status = 0;
     move(a.y, a.x);
     addch( c );
  }else {
     status = 1;
  }
  return status;
}

void enter(Point a)
{
  Point p;
  int count;
  char yoko='-', tate='|', ico='=';
  char dep[]="  deposit  ";
  char wit[]="withdrawals";
  char acc[]="account";
  char rec[]="     record";
  char bal[]="  balance  ";

  p.y=a.y;
  p.x=a.x;
  for(count=0; count<15; count++){
    move(p.y, p.x);
    addch(yoko);
    p.x++;
  }

  p.y=a.y+1;
  p.x=a.x;
  for(count=0; count<18; count++){
    move(p.y, p.x);
    addch(tate);
    p.y++;
  }

  p.y=a.y+1;
  p.x=a.x+14;
  for(count=0; count<18; count++){
    move(p.y, p.x);
    addch(tate);
    p.y++;
  }

  p.y=a.y+19;
  p.x=a.x;
  for(count=0; count<15; count++){
    move(p.y, p.x);
    addch(yoko);
    p.x++;
  }

  p.y=a.y+2;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  p.y=a.y+4;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  p.y=a.y+6;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  p.y=a.y+8;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  p.y=a.y+10;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  p.y=a.y+13;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  p.y=a.y+15;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  p.y=a.y+17;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  p.y=a.y+3;
  p.x=a.x+1;
  move(p.y, p.x);
  addch(tate);

  p.x=a.x+13;
  move(p.y, p.x);
  addch(tate);

  p.y=a.y+7;
  p.x=a.x+1;
  move(p.y, p.x);
  addch(tate);

  p.x=a.x+13;
  move(p.y, p.x);
  addch(tate);

  p.y=a.y+11;
  p.x=a.x+1;
  move(p.y, p.x);
  addch(tate);

  p.x=a.x+13;
  move(p.y, p.x);
  addch(tate);

  p.y=a.y+12;
  p.x=a.x+1;
  move(p.y, p.x);
  addch(tate);

  p.x=a.x+13;
  move(p.y, p.x);
  addch(tate);

  p.y=a.y+16;
  p.x=a.x+1;
  move(p.y, p.x);
  addch(tate);

  p.x=a.x+13;
  move(p.y, p.x);
  addch(tate);

  p.y=a.y+3;
  p.x=a.x+2;
  move(p.y, p.x);
  addstr(dep);

  p.y=a.y+7;
  p.x=a.x+2;
  move(p.y, p.x);
  addstr(wit);

  p.y=a.y+11;
  p.x=a.x+2;
  move(p.y, p.x);
  addstr(acc);

  p.y++;
  p.x=a.x+2;
  move(p.y, p.x);
  addstr(rec);

  p.y=a.y+16;
  p.x=a.x+2;
  move(p.y, p.x);
  addstr(bal);

}

void choice_enter(Point a, int number)
{
  Point p;
  char c='+';
  int command;
  int count;

  command=number;

  switch (command){
  case 0:
    p.y=a.y+2;
    p.x=a.x+1;
    for(count=0; count<13; count++){
      move(p.y, p.x);
      addch(c);
      p.x++;
    }
    p.y=a.y+4;
    p.x=a.x+1;
    for(count=0; count<13; count++){
      move(p.y, p.x);
      addch(c);
      p.x++;
    }
    p.y=a.y+3;
    p.x=a.x+1;
    move(p.y, p.x);
    addch(c);
    p.x=a.x+13;
    move(p.y, p.x);
    addch(c);
    break;
  case 1:
    p.y=a.y+6;
    p.x=a.x+1;
    for(count=0; count<13; count++){
      move(p.y, p.x);
      addch(c);
      p.x++;
    }
    p.y=a.y+8;
    p.x=a.x+1;
    for(count=0; count<13; count++){
      move(p.y, p.x);
      addch(c);
      p.x++;
    }
    p.y=a.y+7;
    p.x=a.x+1;
    move(p.y, p.x);
    addch(c);
    p.x=a.x+13;
    move(p.y, p.x);
    addch(c);
    break;
  case 2:
    p.y=a.y+10;
    p.x=a.x+1;
    for(count=0; count<13; count++){
      move(p.y, p.x);
      addch(c);
      p.x++;
    }
    p.y=a.y+13;
    p.x=a.x+1;
    for(count=0; count<13; count++){
      move(p.y, p.x);
      addch(c);
      p.x++;
    }
    p.y=a.y+11;
    p.x=a.x+1;
    move(p.y, p.x);
    addch(c);
    p.x=a.x+13;
    move(p.y, p.x);
    addch(c);
    p.y=a.y+12;
    p.x=a.x+1;
    move(p.y, p.x);
    addch(c);
    p.x=a.x+13;
    move(p.y, p.x);
    addch(c);
    break;
  case 3:
    p.y=a.y+15;
    p.x=a.x+1;
    for(count=0; count<13; count++){
      move(p.y, p.x);
      addch(c);
      p.x++;
    }
    p.y=a.y+17;
    p.x=a.x+1;
    for(count=0; count<13; count++){
      move(p.y, p.x);
      addch(c);
      p.x++;
    }
    p.y=a.y+16;
    p.x=a.x+1;
    move(p.y, p.x);
    addch(c);
    p.x=a.x+13;
    move(p.y, p.x);
    addch(c);
  }

}

void draw_button(Point a, char n)
{
  Point p;
  int count;
  char yoko='-', tate='|', ico='=';

  p.y=a.y;
  p.x=a.x;
  for(count=0; count<3; count++){
    move(p.y, p.x);
    addch(yoko);
    p.x++;
  }

  p.y=a.y+2;
  p.x=a.x;
  for(count=0; count<3; count++){
    move(p.y, p.x);
    addch(yoko);
    p.x++;
  }

  p.y=a.y+1;
  p.x=a.x;
  move(p.y, p.x);
  addch(tate);
  p.y=a.y+1;
  p.x=a.x+2;
  move(p.y, p.x);
  addch(tate);

  p.y=a.y+1;
  p.x=a.x+1;
  move(p.y, p.x);
  addch(n);

}

void frame_input(Point a, char *order)
{
  Point p;
  int count;
  int status=0;
  char n='1';
  char yoko='-', tate='|', ico='=';

  p.y=a.y+1;
  p.x=a.x+1;
  move(p.y, p.x);
  addstr(order);

  p.y=a.y;
  p.x=a.x;
  for(count=0; count<15; count++){
    move(p.y, p.x);
    addch(yoko);
    p.x++;
  }

  p.y=a.y+1;
  p.x=a.x;
  for(count=0; count<18; count++){
    move(p.y, p.x);
    addch(tate);
    p.y++;
  }

  p.y=a.y+1;
  p.x=a.x+14;
  for(count=0; count<18; count++){
    move(p.y, p.x);
    addch(tate);
    p.y++;
  }

  p.y=a.y+19;
  p.x=a.x;
  for(count=0; count<15; count++){
    move(p.y, p.x);
    addch(yoko);
    p.x++;
  }

  p.y=a.y+3;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  p.y=a.y+5;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  move(a.y+4, a.x+1);
  addch(tate);
  move(a.y+4, a.x+13);
  addch(tate);

  p.y=a.y+7;
  p.x=a.x+2;
  for(count=0; count<3; count++){
    draw_button(p, n);
    p.x=p.x+4;
    n++;
  }
  p.y=a.y+10;
  p.x=a.x+2;
  for(count=0; count<3; count++){
    draw_button(p, n);
    p.x=p.x+4;
    n++;
  }
  p.y=a.y+13;
  p.x=a.x+2;
  for(count=0; count<3; count++){
    draw_button(p, n);
    p.x=p.x+4;
    n++;
  }
  p.y=a.y+16;
  p.x=a.x+6;
  draw_button(p, '0');
  p.x=a.x+10;
  draw_button(p, 'a');
  p.x=a.x+2;
  draw_button(p, 'b');

}

void choice_number(Point a, int number)
{
  Point p;
  int count;
  int command;
  char c='+';

  command=number;

  switch (command){
    case 1:
    p.y=a.y+7;
    p.x=a.x+2;
    break;
    case 2:
    p.y=a.y+7;
    p.x=a.x+6;
    break;
    case 3:
    p.y=a.y+7;
    p.x=a.x+10;
    break;
    case 4:
    p.y=a.y+10;
    p.x=a.x+2;
    break;
    case 5:
    p.y=a.y+10;
    p.x=a.x+6;
    break;
    case 6:
    p.y=a.y+10;
    p.x=a.x+10;
    break;
    case 7:
    p.y=a.y+13;
    p.x=a.x+2;
    break;
    case 8:
    p.y=a.y+13;
    p.x=a.x+6;
    break;
    case 9:
    p.y=a.y+13;
    p.x=a.x+10;
    break;
    case 0:
    p.y=a.y+16;
    p.x=a.x+6;
  }

  for(count=0; count<3; count++){
    move(p.y, p.x);
    addch(c);
    p.x++;
  }

  p.y=p.y+2;
  p.x=p.x-3;
  for(count=0; count<3; count++){
    move(p.y, p.x);
    addch(c);
    p.x++;
  }

  p.y=p.y-1;
  p.x=p.x-1;
  move(p.y, p.x);
  addch(c);
  p.x=p.x-2;
  move(p.y, p.x);
  addch(c);

}

int change_HM(char *hai)
{
  int n=0;
  int count;
  int number[9];
  int money=0;
  int mot;

  for(count=0; count<9; count++){
    number[count]=0;
  }

  while(hai[n]!='\0'){
    n++;
  }

  for(count=0; count<n; count++){
    mot=pow(10, n-count-1);
    number[count]=(hai[count]-48)*mot;
  }

  for(count=0; count<n; count++){
    money=money+number[count];
  }

  return money;
}

int change_MH(int money, int num)
{
  int count;
  int check=0;
  int index=1;
  int dummy[9];
  int number[9];
  int result;
  int mat, b, s;

  while(check==0){
    mat=pow(10, index);
    if(money/mat!=0){
      index++;
    }else{
      check=1;
    }
  }

  for(count=0; count<index; count++){
    if(count==index-1){
      number[count]=money%10;
    }else{
      b=pow(10, index-count);
      s=b/10;
      number[count]=(money%b-money%s)/s;
    }
  }

  if(num==0){
    result=number[0];
  }else{
    for(count=0; count<num; count++){
      result=number[count+1];
    }
  }

  return result;
}

void print_money(Point a, char *hai)
{
  Point p;
  int n=0;
  char c;
  int count;
  int gap=0;

  while(hai[n]!='\0'){
    n++;
  }

  if(n!=1 && (n-1)%3==0){
    gap++;
  }
  if(5<=n && n<=9){
    gap++;
  }
  if(n==8 || n==9){
    gap++;
  }
  p.y=a.y+4;
  p.x=a.x+12;
  for(count=0; count<n; count++){
    move(p.y, p.x);
    if(count!=0 && count!=8 && count%3==0){
      addch(',');
      p.x--;
      move(p.y, p.x);
    }
    addch(hai[n-count-1]);
    p.x--;
  }

}

void balance_frame(Point a, char *order, char *hai)
{
  Point p;
  int count;
  int status=0;
  char n='1';
  char yoko='-', tate='|', ico='=';

  p.y=a.y+1;
  p.x=a.x+1;
  move(p.y, p.x);
  addstr(order);

  p.y=a.y;
  p.x=a.x;
  for(count=0; count<15; count++){
    move(p.y, p.x);
    addch(yoko);
    p.x++;
  }

  p.y=a.y+1;
  p.x=a.x;
  for(count=0; count<18; count++){
    move(p.y, p.x);
    addch(tate);
    p.y++;
  }

  p.y=a.y+1;
  p.x=a.x+14;
  for(count=0; count<18; count++){
    move(p.y, p.x);
    addch(tate);
    p.y++;
  }

  p.y=a.y+19;
  p.x=a.x;
  for(count=0; count<15; count++){
    move(p.y, p.x);
    addch(yoko);
    p.x++;
  }

  p.y=a.y+3;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  p.y=a.y+5;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  move(a.y+4, a.x+1);
  addch(tate);
  move(a.y+4, a.x+13);
  addch(tate);

  p.y=a.y+16;
  p.x=a.x+10;
  draw_button(p, 'a');

  print_money(a, hai);

}

void choice_a(Point a)
{
  Point p;
  int count;
  char c='+';

  p.y=a.y+16;
  p.x=a.x+10;
  for(count=0; count<3; count++){
    move(p.y, p.x);
    addch(c);
    p.x++;
  }

  p.y=a.y+18;
  p.x=a.x+10;
  for(count=0; count<3; count++){
    move(p.y, p.x);
    addch(c);
    p.x++;
  }

  p.y=a.y+17;
  p.x=a.x+10;
  move(p.y, p.x);
  addch(c);
  p.x=p.x+2;
  move(p.y, p.x);
  addch(c);
}

void choice_b(Point a)
{
  Point p;
  int count;
  char c='+';

  p.y=a.y+16;
  p.x=a.x+2;
  for(count=0; count<3; count++){
    move(p.y, p.x);
    addch(c);
    p.x++;
  }

  p.y=a.y+18;
  p.x=a.x+2;
  for(count=0; count<3; count++){
    move(p.y, p.x);
    addch(c);
    p.x++;
  }

  p.y=a.y+17;
  p.x=a.x+2;
  move(p.y, p.x);
  addch(c);
  p.x=p.x+2;
  move(p.y, p.x);
  addch(c);
}

void print_pass(Point a, int num)
{
  Point p;
  char pass_0[]="*";
  char pass_1[]="* *";
  char pass_2[]="* * *";
  char pass_3[]="* * * *";

  p.y=a.y+4;
  p.x=a.x+4;
  move(p.y, p.x);
  switch (num){
    case 1:
    addstr(pass_0);
    break;
    case 2:
    addstr(pass_1);
    break;
    case 3:
    addstr(pass_2);
    break;
    case 4:
    addstr(pass_3);
  }

}

void frame_pass(Point a, char *order){
  Point p;
  int count;
  int status=0;
  char n='1';
  char yoko='-', tate='|', ico='=';

  p.y=a.y+1;
  p.x=a.x+1;
  move(p.y, p.x);
  addstr(order);

  p.y=a.y;
  p.x=a.x;
  for(count=0; count<15; count++){
    move(p.y, p.x);
    addch(yoko);
    p.x++;
  }

  p.y=a.y+1;
  p.x=a.x;
  for(count=0; count<18; count++){
    move(p.y, p.x);
    addch(tate);
    p.y++;
  }

  p.y=a.y+1;
  p.x=a.x+14;
  for(count=0; count<18; count++){
    move(p.y, p.x);
    addch(tate);
    p.y++;
  }

  p.y=a.y+19;
  p.x=a.x;
  for(count=0; count<15; count++){
    move(p.y, p.x);
    addch(yoko);
    p.x++;
  }

  p.y=a.y+3;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  p.y=a.y+5;
  p.x=a.x+1;
  for(count=0; count<13; count++){
    move(p.y, p.x);
    addch(ico);
    p.x++;
  }

  move(a.y+4, a.x+1);
  addch(tate);
  move(a.y+4, a.x+13);
  addch(tate);

  p.y=a.y+7;
  p.x=a.x+2;
  for(count=0; count<3; count++){
    draw_button(p, n);
    p.x=p.x+4;
    n++;
  }
  p.y=a.y+10;
  p.x=a.x+2;
  for(count=0; count<3; count++){
    draw_button(p, n);
    p.x=p.x+4;
    n++;
  }
  p.y=a.y+13;
  p.x=a.x+2;
  for(count=0; count<3; count++){
    draw_button(p, n);
    p.x=p.x+4;
    n++;
  }
  p.y=a.y+16;
  p.x=a.x+6;
  draw_button(p, '0');
  p.x=a.x+2;
  draw_button(p, 'b');

}

void frame_record(Point a){
  Point p;
  int count;
  int status=0;
  char yoko='-', tate='|';
  char sentence0[]="recorded";
  char sentence1[]=" your balance";

  p.y=a.y+1;
  p.x=a.x+1;
  move(p.y, p.x);
  addstr(sentence0);
  p.y++;
  move(p.y, p.x);
  addstr(sentence1);

  p.y=a.y;
  p.x=a.x;
  for(count=0; count<15; count++){
    move(p.y, p.x);
    addch(yoko);
    p.x++;
  }

  p.y=a.y+1;
  p.x=a.x;
  for(count=0; count<18; count++){
    move(p.y, p.x);
    addch(tate);
    p.y++;
  }

  p.y=a.y+1;
  p.x=a.x+14;
  for(count=0; count<18; count++){
    move(p.y, p.x);
    addch(tate);
    p.y++;
  }

  p.y=a.y+19;
  p.x=a.x;
  for(count=0; count<15; count++){
    move(p.y, p.x);
    addch(yoko);
    p.x++;
  }

  p.y=a.y+16;
  p.x=a.x+10;
  draw_button(p, 'a');

}
