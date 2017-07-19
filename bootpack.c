
//void myprintf(int addr, int data);
void myprintf(char* msg, int len);

int main(void)
{
	int i; 
	char *p=0;
	for (i = 0xa0000; i <= 0xaffff; i++) {
		//write_memory(i, i&0x0f); 
		if(i<=0xa1440){
			p = i;
			*p = 5;
		}
		else
		{
			p = i;
			*p = 0x0f;
		}
	}
  //myprintf("UNICAOS",7);
	for (;;) {
//		io_hlt();
	}

}
