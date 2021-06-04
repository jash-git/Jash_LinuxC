#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
void SetStdinEcho(int intenable)
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if( intenable==0 )
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);

}
int main()
{
    char show[80],hide[80];
    SetStdinEcho(1);
    printf("input pawssword:\n");
    scanf("%s",show);
    printf("show_pawssword:%s\n",show);
    SetStdinEcho(0);
    printf("input pawssword:\n");
    scanf("%s",hide);
    printf("hide_pawssword:%s\n",hide);
    SetStdinEcho(1);
    return 0;
}