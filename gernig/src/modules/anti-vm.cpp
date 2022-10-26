bool sidt() {
	unsigned char	idtr[6];
	unsigned long	idt	= 0;

	_asm sidt idtr
	idt = *((unsigned long *)&idtr[2]);

	if 	((idt >> 24) == 0xff)
		return true;
	else
		return false;
		
}