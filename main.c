


typedef enum {TEM, CDM, TSM} game_state_t;
game_state_t game_state = TEM;

void game_task()
{
    switch (game_state)
    {
    case TEM:
        break;

    case CDM:
        break;

    case TSM:
        break;
    
    default:
        break;
    }
}



void input_task()
{
    
}

int main()
{


    init_vars();
    init_ports();

    while(1)
    {
        sev_seg_task();
        input_task();
        game_task();
        lcd_task();

    }
    return 0;
}