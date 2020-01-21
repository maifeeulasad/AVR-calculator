# include <avr/io.h>
# include <util/delay.h>




uint8_t r = 0, c = 0;
int res=0;
uint8_t pos = 0;





uint8_t ou = 0;

unsigned char key  = ' ';


const unsigned char keypad[4][4] =
{
    {'1','2','3','+'},
    {'4','5','6','-'},
    {'7','8','9','*'},
    {'C','0','=','/'},
};


unsigned char data[] =
{
    '0','+','0'
};

unsigned char display[] =
{
    '0',' ','0'
};

const uint8_t ss[] =
{
    0x3f, //0
    0x06,
    0x5b,
    0x4f,
    0x66,
    0x6d,
    0x7d,
    0x07,
    0x7f,
    0x6f, //9
    0x00,
    0x80, //.
    0x40, //-
};

void display_from_int(int x);
uint8_t get_ss(unsigned char x, uint8_t dot);
uint8_t is_operator(unsigned char x);
void add_data(unsigned char key);

void output();
void process();
void calc();
void reset_data();
void input();
void reset_display();



int main()
{
    DDRD = 0xFF;
    DDRC = 0x03;

    while (1)
    {

        input();
        process();
        output();


    }
    return 0;
}



void process()
{
    if (key == '\n' || key==' ')
    {
        return;
    }
    if (key == 'C')
    {
        reset_data();
        reset_display();
    }
    else if (key == '=')
    {
        calc();
        display_from_int(res);
        reset_data();
    }
    else
    {
        add_data(key);
    }
}


void add_data(unsigned char key)
{
    if(!is_operator(key))
    {
        display_from_int(key-'0');
    }
    if(pos==0)
    {
        if(!is_operator(key))
        {
            data[pos++]=key;
        }
    }
    else if(pos==1)
    {
        if(is_operator(key))
        {
            data[pos++]=key;
        }
        else
        {
            data[pos-1]=key;
        }
    }
    else if(pos==2)
    {
        if(!is_operator(key))
        {
            data[pos++]=key;
        }
        else
        {
            data[pos-1]=key;
        }
    }
    else if(pos==3)
    {
        if(!is_operator(key))
        {
            data[pos-1]=key;
        }
    }
}



void calc()
{

    int a = data[0] - '0';
    unsigned char operato = data[1];
    int b = data[2] - '0';



    if (operato == '+')
        res = a + b;
    else if (operato == '-')
        res = a - b;
    else if (operato == '*')
        res = a * b;
    else if (operato == '/' && b != 0)
    {
        if (a % b == 0)
            res = (int) a / b;
        else
            res = (int)(1000 * ((double) a / (double) b));
    }
    else
        res = -100;
    pos=0;

}

void reset_data()
{
    pos = 0;
}
void reset_display()
{

    display[0] =display[2]='0';
    display[1]=' ';
}

void output()
{
    PORTD = ss[10];
    ++ou;
    ou%=2;
    if (ou)
    {
        PORTC = 0x01;
        PORTD = get_ss(display[0], display[1] == '.');
    }
    else
    {
        PORTC = 0x02;
        PORTD = get_ss(display[2], 0);
    }

}

uint8_t get_ss(unsigned char x, uint8_t dot)
{
    if (x == ' ')
        return ss[10];
    if (x == '-')
        return ss[12];
    if (dot)
        return ss[x - '0'] | ss[11];
    return ss[x - '0'];
}

void display_from_int(int x)
{
    if (x > 99)
    {
        x /= 100;
        display[0] = x / 10 + '0';
        display[1] = '.';
        display[2] = (x % 10) + '0';
    }
    else if (x == -100)
    {
        display[0] = display[2] = '-';
        display[1] = ' ';
    }
    else if (x < 0)
    {
        display[0] = '-';
        display[1] = ' ';
        display[2] = (-x) + '0';
    }
    else
    {
        display[0] = '0' + x / 10;
        display[1] = ' ';
        display[2] = '0' + x % 10;
    }

}

void input()
{
    PORTB |= 0X0F;
    for (c = 0; c < 4; c++)
    {
        DDRB &= ~(0XFF);
        DDRB |= (0X80 >> c);
        for (r = 0; r < 4; r++)
        {
            if (!(PINB & (0X08 >> r)))
            {
                key= keypad[3 - r][3 - c];
                return;
            }
        }
    }
    key=' ';
}

uint8_t is_operator(unsigned char x)
{
    return x=='+' || x=='-' || x=='*' || x=='/';
}
