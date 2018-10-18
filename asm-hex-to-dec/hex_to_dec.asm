		global hex_to_dec
		section .data
		section .text
hex_to_dec:
		mov edi, [esp + 4] ; out
		mov esi, [esp + 8] ; hex
		pushad
		sub esp, 16; 
		mov [esp], dword 0
		mov [esp + 4], dword 0
		mov [esp + 8], dword 0
		mov [esp + 12], dword 0
		mov cl, 32 ; counter = 32

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
		mov eax, [esp + 12]
		mov edx, [esp + 8]
		shld eax, edx, 4
		mov [esp + 12], eax
		mov eax, [esp + 8]
		mov edx, [esp + 4]
		shld eax, edx, 4
		mov [esp + 8], eax
		mov eax, [esp + 4]
		mov edx, [esp]
		shld eax, edx, 4
		mov [esp + 4], eax
		shl edx, 4
		mov [esp], edx

		; (64-bit)buffer += ch
		add [esp], ch
		mov ebx, 0
		adc [esp + 4],  ebx
		adc [esp + 8],  ebx
		adc [esp + 12],  ebx

		dec cl			
		cmp cl, 0
		jne first_loop
first_loop_end:
		mov esi, [esp + 8 + 16 + 32] ; reset esi to hex[0]
		mov ch, [esi]
		cmp ch, '-'
		jne minus_applied
		;if(hex[0] == '-')
		not dword[esp + 12]
		not dword[esp + 8]
		not dword[esp + 4]
		not dword[esp]
		mov ecx, 1
		add [esp], ecx
		mov ebx, 0
		adc [esp + 4],   ebx
		adc [esp + 8],   ebx
		adc [esp + 12],  ebx
minus_applied:
		mov eax, 1
		shl eax, 31
		mov ebx, [esp + 12]
		and eax, ebx
		cmp eax, 0
		je not_negative
		;if(sign_bit != 0)
		mov eax, 1
		shl eax, 31
		sub eax, 1
		or [esp + 12], eax ; sign_bit = 0
		mov ch, '-'
		mov [edi], ch
		inc edi
not_negative:
		mov esi, esp
		xor ecx, ecx
second_loop:
		; (64-bit)buffer /= 10
		mov ebx, 10
		xor edx, edx

		mov eax, [esi + 12]
		div ebx
		mov [esi + 12], eax
		
		mov eax, [esi + 8]
		div ebx
		mov [esi + 8], eax

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

		mov ch, 0x00
		mov [edi], ch
		inc edi

		add esp, 16
		popad
		ret


