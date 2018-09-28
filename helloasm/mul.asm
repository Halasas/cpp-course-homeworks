				section 		.text
				global 			_start
_start:

				sub 			rsp, 2 * 128 * 2
				mov 			rdi, rsp
				mov 			rcx, 128
				call			read_long
				lea 			rdi, [rsp + 128 * 8]
				call 			read_long
				mov 			rsi, rdi
				mov 			rdi, rsp

				call			mul_long_long
				
				call            write_long

                mov             al, 0x0a
                call            write_char

                jmp             exit
; multiplies two long numbers
;    rdi -- address of multiplier #1 (long number)
;    rsi -- address of multiplier #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    product is written to rsi:rdi
mul_long_long:
                push            rdi ; #1
                push            rsi ; #2
                push            rcx ; length
                push            rax ; multiplication registers
                push            rdx
                push            rbp ; stores position of rsp
                push            rbx ; carry
                push            r8 ; first  cycle counter
                push            r9 ; second cycle counter
                push            r10 ; copy of rdi
                push            r11 ; copy of rsi

                clc               
                mov             r8, rcx ; set first cycle counter
                shl             rcx, 4   ; increase size of num
                sub             rsp, rcx ; make memory for result
                shr             rcx, 4   ; reduce size of num
                mov             rbp, rsp ; store position of rsp
                mov             r10, rdi ; store rdi
                mov             r11, rsi ; store rsi
.first_loop
                mov             r9, rcx ; set second cycle counter 
                mov             rsi, r11 ; restore original rsi
                xor             rbx, rbx ; set carry to zero

                .second_loop:
                                ; multiply current qwords in rdi and rsi 
                                ; and write result in rax:rdx
                                mov             rax, [rdi]
                                mul             qword [rsi]
                                ; add carry from last iteration to rax:rdx  
                                add             rax, rbx 
                                adc             rdx, 0
                                ; add rax to memory at rsp
                                add             [rsp], rax
                                ; add carry to rdx
                                adc             rdx, 0
                                ; set rbx to rdx as carry
                                mov             rbx, rdx
                                ; move rsp to next qword
                                add             rsp, 8
                                ; move rsi to next qword
                                add             rsi, 8
                                dec             r9
                                jnz             .second_loop
                add             [rsp], rbx ; add carry from last iteration
                shl             rcx, 3
                sub             rsp, rcx ; return rsp to initial position
                shr             rcx, 3
                add             rsp, 8 ; shift rsp
                add             rdi, 8 ; move rdi to next qword
                dec             r8
                jnz             .first_loop

                mov             rsp, rbp ; move rsp back to initial position
                mov             rdi, r10 ; restore initial values of rdi and rsi
                mov             rsi, r11
                mov             r8, rcx ; set counter to store ans in rdi
                .store_ans_in_rdi:
                                ; put answer to the next qword in rdi
                                pop             qword [rdi]
                                ; move rdi to next qword in answer
                                add             rdi, 8
                                dec             r8
                                jnz             .store_ans_in_rdi
                mov             r8, rcx ; set counter to store ans in rsi
                .store_ans_in_rsi:
                                ; put answer to the next qword in rsi
                                pop             qword [rsi]
                                ; move rsi to next qword in answer
                                add             rsi, 8
                                dec             r8
                                jnz             .store_ans_in_rsi

                ; restore data in registers
                pop             r11
                pop             r10
                pop             r9
                pop             r8
                pop             rbx
                pop             rbp
                pop             rdx
                pop             rax
                pop             rcx
                pop             rsi
                pop             rdi
                ret




; subs two long number
;    rdi -- address of #1 (long number)
;    rsi -- address of #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    sub is written to rdi
sub_long_long:
                push            rdi
                push            rsi
                push            rcx

                clc
.loop:
                mov             rax, [rsi]
                sbb             [rdi], rax
				lea             rsi, [rsi + 8]
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rsi
                pop             rdi
                ret
; adds two long number
;    rdi -- address of summand #1 (long number)
;    rsi -- address of summand #2 (long number)sa
;    rcx -- length of long numbers in qwords
; result:
;    sum is written to rdi
add_long_long:
                push            rdi
                push            rsi
                push            rcx

                clc
.loop:
                mov             rax, [rsi]
                lea             rsi, [rsi + 8]
                adc             [rdi], rax
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rsi
                pop             rdi
                ret
; adds 64-bit number to long number
;    rdi -- address of summand #1 (long number)
;    rax -- summand #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    sum is written to rdi
add_long_short:
                push            rdi
                push            rcx
                push            rdx

                xor             rdx,rdx
.loop:
                add             [rdi], rax
                adc             rdx, 0
                mov             rax, rdx
                xor             rdx, rdx
                add             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rdx
                pop             rcx
                pop             rdi
                ret

; multiplies long number by a short
;    rdi -- address of multiplier #1 (long number)
;    rbx -- multiplier #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    product is written to rdi
mul_long_short:
                push            rax
                push            rdi
                push            rcx

                xor             rsi, rsi
.loop:
                mov             rax, [rdi]
                mul             rbx
                add             rax, rsi
                adc             rdx, 0
                mov             [rdi], rax
                add             rdi, 8
                mov             rsi, rdx
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rdi
                pop             rax
                ret

; divides long number by a short
;    rdi -- address of dividend (long number)
;    rbx -- divisor (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    quotient is written to rdi
;    rdx -- remainder
div_long_short:
                push            rdi
                push            rax
                push            rcx

                lea             rdi, [rdi + 8 * rcx - 8]
                xor             rdx, rdx

.loop:
                mov             rax, [rdi]
                div             rbx
                mov             [rdi], rax
                sub             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rax
                pop             rdi
                ret

; assigns a zero to long number
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
set_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep stosq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; checks if a long number is a zero
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
; result:
;    ZF=1 if zero
is_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep scasq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; read long number from stdin
;    rdi -- location for output (long number)
;    rcx -- length of long number in qwords
read_long:
                push            rcx
                push            rdi

                call            set_zero
.loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              .done
                cmp             rax, '0'
                jb              .invalid_char
                cmp             rax, '9'
                ja              .invalid_char

                sub             rax, '0'
                mov             rbx, 10
                call            mul_long_short
                call            add_long_short
                jmp             .loop

.done:
                pop             rdi
                pop             rcx
                ret

.invalid_char:
                mov             rsi, invalid_char_msg
                mov             rdx, invalid_char_msg_size
                call            print_string
                call            write_char
                mov             al, 0x0a
                call            write_char

.skip_loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              exit
                jmp             .skip_loop

; write long number to stdout
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
write_long:
                push            rax
                push            rcx

                mov             rax, 20
                mul             rcx
                mov             rbp, rsp
                sub             rsp, rax

                mov             rsi, rbp

.loop:
                mov             rbx, 10
                call            div_long_short
                add             rdx, '0'
                dec             rsi
                mov             [rsi], dl
                call            is_zero
                jnz             .loop

                mov             rdx, rbp
                sub             rdx, rsi
                call            print_string

                mov             rsp, rbp
                pop             rcx
                pop             rax
                ret

; read one char from stdin
; result:
;    rax == -1 if error occurs
;    rax \in [0; 255] if OK
read_char:
                push            rcx
                push            rdi

                sub             rsp, 1
                xor             rax, rax
                xor             rdi, rdi
                mov             rsi, rsp
                mov             rdx, 1
                syscall

                cmp             rax, 1
                jne             .error
                xor             rax, rax
                mov             al, [rsp]
                add             rsp, 1

                pop             rdi
                pop             rcx
                ret
.error:
                mov             rax, -1
                add             rsp, 1
                pop             rdi
                pop             rcx
                ret

; write one char to stdout, errors are ignored
;    al -- char
write_char:
                sub             rsp, 1
                mov             [rsp], al

                mov             rax, 1
                mov             rdi, 1
                mov             rsi, rsp
                mov             rdx, 1
                syscall
                add             rsp, 1
                ret

exit:
                mov             rax, 60
                xor             rdi, rdi
                syscall

; print string to stdout
;    rsi -- string
;    rdx -- size
print_string:
                push            rax

                mov             rax, 1
                mov             rdi, 1
                syscall

                pop             rax
                ret


                section         .rodata
invalid_char_msg:
                db              "Invalid character: "
invalid_char_msg_size: equ             $ - invalid_char_msg
