adr 	cmd   arg 	source
0001	01	  21	push 1
0003	08  		dup
0004	03  		add
0005	08  		dup
0006	05  		mul
0007	01	  21	push 3
0009	0e	  0e	ja :1
000b	00  		hlt
000c	07  		out
000d	07  		out
000e	07  		out
000f	lb	  01	1:
0010	01	  21	push 1
0012	00  		hlt
