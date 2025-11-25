void send_data_one(void){
    GPIO_PORTF_DATA_R=0X02;
    int j=12;
    while(j>0){
        j--;
        //do nothing
    }
    GPIO_PORTF_DATA_R=0X00;
    j=5;
    while(j>0){
        j--;
        //do nothing
    }
}

void send_data_zero(void){
    GPIO_PORTF_DATA_R=0X02;
    GPIO_PORTF_DATA_R=0X00;
    int j=13;
    while(j>0){
        j--;
        //do nothing
