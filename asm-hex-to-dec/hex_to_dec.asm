		global hex_to_dec
		section .data
		section .text
hex_to_dec:
		mov edi, [esp + 4] ; out
		mov esi, [esp + 8] ; hex
		pushad
		sub esp, 8; 
		mov [esp], dword 0
		mov [esp + 4], dword 0
		mov cl, 16 ; counter = 16

		mov ch, [esi] ; ch = hex[0]
		cmp ch, '-' 
		jne first_loop 
					   ; if hex[0] == '-'
		inc esi

first_loop:
		mov ch, [esi] ; ch = hex[ecx]
		inc esi

		cmp ch, '0'
		jl not_0_9
		cmp ch, '9'
		jg not_0_9 
		; ch >= '0' && ch <= '9'
		sub ch, '0'
		jmp ch_calculated

		not_0_9:
		cmp ch, 'A'
		jl not_A_F 
		cmp ch, 'F'
		jg not_A_F 
		; ch >= 'A' && ch <= 'F'
		sub ch, 'A'
		add ch, 10
		jmp ch_calculated

		not_A_F:		
		cmp ch, 'a'
		jl not_a_f 
		cmp ch, 'f'
		jg not_a_f 
		; ch >= 'a' && ch <= 'f'
		sub ch, 'a'
		add ch, 10
		jmp ch_calculated

		not_a_f:
		jmp first_loop_end		
		ch_calculated:
		; (64-bit)buffer *= 16
		mov eax, [esp]
		mov ebx, dword 16
		mul ebx
		mov [esp], eax
		mov ebp, edx
		mov eax, [esp + 4]
		mul ebx
		add eax, ebp
		mov [esp + 4], eax
		; (64-bit)buffer += ch
		add [esp], ch
		mov ebx, 0
		adc [esp + 4],  ebx

		mov eax, [esp]


		dec cl			
		cmp cl, 0
		jne first_loop
first_loop_end:
		mov esi, [esp + 8 + 8 + 32] ; reset esi to hex[0]
		mov ch, [esi]
		cmp ch, '-'
		jne not_negative
		;if(hex[0] == '-')
		mov [edi], ch
		inc edi
		not dword[esp + 4]
		not dword[esp]
not_negative:
		mov esi, esp
		xor ecx, ecx
second_loop:
		; (64-bit)buffer /= 10
		mov ebx, 10
		xor edx, edx

		mov eax, [esi + 4]
		div ebx
		mov [esi + 4], eax
		
		mov eax, [esi]
		div ebx
		mov [esi], eax

		add edx, '0'
		push edx
		inc ecx

		mov edx, [esi + 4]		
		cmp edx, 0
		jne second_loop
		mov edx, [esi]
		cmp edx, 0
		jne second_loop

print_out:
		pop edx
		mov [edi], edx
		inc edi
		dec ecx
		cmp ecx, 0
		jne print_out

		add esp, 8
		popad
		ret


