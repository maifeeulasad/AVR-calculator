#include<stdio.h>
#include<stdlib.h>
//#include<avr/io.h>
//#include<util/delay.h>
//#include <bits/stdc++.h>
//using namespace std;
struct node
{
    int data;
    struct node *next;
};

int seven_segment[]={
    0x7E, // 0
    0x30, // 1
    0x6D,
    0x79,
    0x33,
    0x5B,
    0x5F,
    0x70,
    0x7F,
    0x7B, // 9
    0x80, // .
    0x01, // -
    0x00, // clear
};

int get_seven_segment(char x,int dot)
{
    //printf("%c %d\n",x,dot);
    if(x=='-')
        return seven_segment[11];
    else if(x==' ')
        return seven_segment[12];
    if(dot)
        return seven_segment[x-'0'] | seven_segment[10];
    else
        return seven_segment[x-'0'];
}


int result=0;
char display[]={' ',' ',' '};
struct node *start = NULL;

int is_number(char x)
{
    return x>='0' && x<='9' ;
}

int is_operator(char x)
{
    return x=='+' || x=='-' || x=='*' || x=='/';
}

void insert_at_end(int x)
{
    struct node *t, *temp;

    t = (struct node*)malloc(sizeof(struct node));

    if (start == NULL)
    {
        start = t;
        start->data = x;
        start->next = NULL;
        return;
    }

    temp = start;

    while (temp->next != NULL)
        temp = temp->next;

    temp->next = t;
    t->data    = x;
    t->next    = NULL;
}

void traverse()
{
    struct node *t;

    t = start;

    printf("List : ");

    if (t == NULL)
    {
        printf("---\n");
        return;
    }


    while (t->next != NULL)
    {
        printf("%d ", t->data);
        t = t->next;
    }
    printf("%d\n", t->data);
}


void delete_from_end()
{
    struct node *t, *u;
    int n;

    if (start == NULL)
    {
        //printf("Linked list is already empty.\n");
        return;
    }


    if (start->next == NULL)
    {
        n = start->data;
        free(start);
        start = NULL;
        //printf("%d deleted from end successfully.\n", n);
        return;
    }

    t = start;

    while (t->next != NULL)
    {
        u = t;
        t = t->next;
    }

    n = t->data;
    u->next = NULL;
    free(t);

    //printf("%d deleted from end successfully.\n", n);
}
void clear_node()
{
    while(start!=NULL)
        delete_from_end();
}
int last_element()
{
    if(start==NULL)
    {
        return -1;
    }
    struct node *t=start;
    while(t->next!=NULL)
    {
        t=t->next;
    }
    return t->data;
}
int size_of_node()
{
    int c=0;
    struct node *t=start;
    if(t==NULL)
        return 0;
    while(t->next!=NULL)
    {
        ++c;
        t=t->next;
    }
    return ++c;
}
void calculate()
{
    if(size_of_node()!=3)
    {
        return;
    }
    struct node *t=start;

    int a=t->data - '0';
    int operato=t->next->data;
    int b=t->next->next->data - '0';

    //printf("%d %d %d %d\n",'+','-','*','/');

    //cout << "Calculate : " << a << " " << operato << " "  << b << endl;

    if(operato=='+')
        result = a+b;
    else if(operato=='-')
        result = a-b;
    else if(operato=='*')
        result = a*b;
    else if(operato=='/' && b!=0)
    {
        if(a%b==0)
            result = (int)a/b;
        else
            result= 1000*((double)a/(double)b); //100~4500
    }
    else
        result = -100;
    //printf("Result : %d\n",result);
    clear_node();
    //cout << result << endl;
}

char *display_from_int(int x)
{
    //printf("%d --- \n",x);
    if(x>99)
    {
        //printf("floating points\n");
        x/=100;
        if(x/10)
            display[0]=' ';
        else
            display[0]='0'+x/10;
        display[1]='.';
        display[2]=(x%10)+'0';
    }
    else if(x==-100)
    {
        //printf("error\n");
        display[0]=display[2]='-';
        display[1]=' ';
    }
    else if(x<0)
    {
        //printf("negative number\n");
        display[0]='-';
        display[1]=' ';
        display[2]=(-x)+'0';
    }
    else if(x==0)
    {
        //printf("zero\n");
        display[0]=' ';
        display[1]=' ';
        display[2]='0';
    }
    else
    {
        //printf("positive number\n");
        x-='0';
        display[0]='0'+x/10;
        display[1]=' ';
        display[2]='0'+x%10;
    }
    return display;
}
void insert_node(int x)
{
    //printf("Is operator - %d\n",is_operator(x));
    //printf("Is operator - %d\n",is_operator(last_element()));
    if(is_number(x))
        display_from_int(x);
    if(start==NULL)
    {
        if(!is_operator(x))
        {
            //printf("Adding at null node");
            insert_at_end(x);
        }
    }
    else
    {
        if(is_number(x)==is_number(last_element()))
        {
            //printf("Match - number");
            delete_from_end();
            //traverse();
            insert_at_end(x);
            //traverse();
        }
        else if(is_operator(x)==is_operator(last_element()) )
        {
            //printf("Match - operator");
            delete_from_end();
            //traverse();
            insert_at_end(x);
            //traverse();
        }
        else
        {
            //printf("Adding at else block");
            insert_at_end(x);
        }
    }
}
int main(int argc, char const *argv[])
{


    printf("MIT License\nCopyright (c) 2019 Maifee Ul Asad\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal \nin the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\nThe above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\nTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");

    printf("\n\n\n1 2 3 +\n");
    printf("4 5 6 -\n");
    printf("7 8 9 *\n");
    printf("C 0 = /\n\n\n");
    for(;;)
    {
        char xx;
        scanf("%c",&xx);
        if(xx==10)
            continue;
        //printf("%d\n", xx);
        //printf("%d\n", is_number(xx));
        //printf("%d\n", is_operator(xx));

        if(xx=='C')
        {
            clear_node();
        }
        else if(xx=='=')
        {
            calculate();
            display_from_int(result+'0');
            //printf("%s\n",display_from_int(result));
        }
        else
        {
            insert_node(xx);
        }
        //printf("Display : %s\n",display);
        printf("SS      : %X %X\n",get_seven_segment(display[0],display[1]=='.'),get_seven_segment(display[2],0));
        //printf("SS      : %X %X\n",get_seven_segment(display[0],display[1]=='.'),get_seven_segment(display[2],0));
        //printf("%s\n",display_from_int(last_element()));
        //traverse();
        //cout << size_of_node() << endl;
        //printf("%d\n", last_element());
    }
    return 0;
}
